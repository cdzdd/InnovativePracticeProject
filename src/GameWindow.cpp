#include "GameWindow.h"
#include "PowerUp.h"
#include "GameUtils.h"
#include "ChineseText.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <filesystem>

GameWindow::GameWindow()
    : window(sf::VideoMode(1440, 900), "Eat Fish"),
      width(1440), height(900), state(STATE_MENU), gameTime(0), isBoss(false),
      currentLevel(1), maxUnlockedLevel(1), levelScore(0),
      isBossDashActive(false), isBossDashWarning(false), bossDashTimer(0), bossDashY(0), bossDashCooldown(0), lastBossDashScore(0) {
    
    srand(static_cast<unsigned>(time(0)));
    
    // Set frame rate limit
    window.setFramerateLimit(25);
    
    // Load game resources with fallback path handling
    std::string assetsDir = "assets/";
    // Try current directory first, then parent directory if assets not found
    if (!std::filesystem::exists(assetsDir + "sea.png")) {
        assetsDir = "../assets/"; // Fallback for running from Release directory
        if (!std::filesystem::exists(assetsDir + "sea.png")) {
            assetsDir = "../../assets/"; // Additional fallback
        }
    }
    if (!GameUtils::loadTextures(assetsDir)) {
        std::cerr << "Warning: Some textures failed to load!" << std::endl;
    }

    myFish = std::make_unique<MyFish>();
    background = std::make_unique<Background>();
    
    // 直接在构造函数中初始化关卡数据
    levels.clear();
    
    // Add 5 levels directly
    for (int i = 0; i < 5; i++) {
        Level level;
        level.levelNumber = i + 1;
        level.difficulty = i;
        level.requiredScore = 20 + i * 30;
        level.enemyTypes[0] = 80 - i * 12;
        level.enemyTypes[1] = 20 + i * 6;
        level.enemyTypes[2] = 0 + i * 6;
        level.enemySpawnRate = 30 - i * 4;
        level.hasBoss = (i == 4);
        level.bossType = "Megashark";
        level.levelName = "Level " + std::to_string(i + 1);
        level.levelDesc = "This is level " + std::to_string(i + 1);
        levels.push_back(level);
    }
    
    // 加载游戏进度
    loadProgress();
    
    // 调试信息
    std::cout << "Levels initialized: " << levels.size() << std::endl;
    std::cout << "Max unlocked level: " << maxUnlockedLevel << std::endl;

    // Debug: create a clearly identifiable moving test enemy when debugVisual is enabled
    if (GameUtils::debugVisual) {
        auto dbg = std::make_shared<Enemy1L>();
        dbg->x = 200; // on-screen
        dbg->y = 200;
        dbg->speed = 2; // slow visible speed
        dbg->dir = 1;   // move right
        GameUtils::enemyList.push_back(dbg);
        std::cout << "DEBUG_TEST_ENEMY created addr=" << dbg.get() << " at " << dbg->x << "," << dbg->y << " speed=" << dbg->speed << "\n";
    }
}

void GameWindow::run() {
    while (window.isOpen()) {
        handleInput();
        update();
        render();
    }
}

void GameWindow::handleInput() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        else if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                
                switch (state) {
                    case STATE_MENU:
                        // Check if clicked on "开始游戏" button
                        if (mousePos.x >= 650 && mousePos.x <= 850 && mousePos.y >= 300 && mousePos.y <= 350) {
                            state = STATE_LEVEL_SELECT;
                        }
                        // Check if clicked on "帮助" button
                        else if (mousePos.x >= 650 && mousePos.x <= 750 && mousePos.y >= 400 && mousePos.y <= 450) {
                            state = STATE_HELP;
                        }
                        // Check if clicked on "设置" button
                        else if (mousePos.x >= 650 && mousePos.x <= 750 && mousePos.y >= 500 && mousePos.y <= 550) {
                            state = STATE_SETTINGS;
                        }
                        break;
                        
                    case STATE_LEVEL_SELECT:
                        // 关卡选择按钮处理
                        for (int i = 0; i < levels.size(); i++) {
                            int levelX = 300 + (i % 2) * 400;
                            int levelY = 200 + (i / 2) * 150;
                            
                            if (mousePos.x >= levelX && mousePos.x <= levelX + 300 && 
                                mousePos.y >= levelY && mousePos.y <= levelY + 100) {
                                if (i + 1 <= maxUnlockedLevel) {
                                    loadLevel(i + 1);
                                }
                                break;
                            }
                        }
                        // 返回菜单按钮
                        if (mousePos.x >= 650 && mousePos.x <= 850 && mousePos.y >= 600 && mousePos.y <= 650) {
                            state = STATE_MENU;
                        }
                        break;
                        
                    case STATE_START:
                        state = STATE_RUNNING;
                        break;
                        
                    case STATE_LOSE:
                    case STATE_WIN:
                        reGame();
                        state = STATE_LEVEL_SELECT;
                        break;
                        
                    case STATE_HELP:
                    case STATE_SETTINGS:
                        // Check if clicked on "返回菜单" button
                        if (mousePos.x >= 650 && mousePos.x <= 850 && mousePos.y >= 600 && mousePos.y <= 650) {
                            state = STATE_MENU;
                        }
                        break;
                }
            }
        }
        else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::W) {
                GameUtils::UP = true;
            } else if (event.key.code == sf::Keyboard::S) {
                GameUtils::DOWN = true;
            } else if (event.key.code == sf::Keyboard::A) {
                GameUtils::LEFT = true;
            } else if (event.key.code == sf::Keyboard::D) {
                GameUtils::RIGHT = true;
            } else if (event.key.code == sf::Keyboard::Space) {
                // Pause/continue game
                if (state == STATE_RUNNING) {
                    state = STATE_PAUSE;
                } else if (state == STATE_PAUSE) {
                    state = STATE_RUNNING;
                }
            } else if (event.key.code == sf::Keyboard::Escape) {
                // 返回菜单
                if (state == STATE_RUNNING || state == STATE_PAUSE) {
                    state = STATE_LEVEL_SELECT;
                }
            }
        }
        else if (event.type == sf::Event::KeyReleased) {
            if (event.key.code == sf::Keyboard::W) {
                GameUtils::UP = false;
            } else if (event.key.code == sf::Keyboard::S) {
                GameUtils::DOWN = false;
            } else if (event.key.code == sf::Keyboard::A) {
                GameUtils::LEFT = false;
            } else if (event.key.code == sf::Keyboard::D) {
                GameUtils::RIGHT = false;
            }
        }
    }
}

void GameWindow::update() {
    gameTime++;

    if (state == STATE_RUNNING) {
        logic();
    }
}

void GameWindow::logic() {
    // Update player logic
    myFish->logic();

    // 根据当前关卡设置玩家等级
    Level& current = levels[currentLevel - 1];
    if (levelScore < current.requiredScore * 0.2) {
        myFish->level = 1; // Level 1: can eat type 1 enemies
    } else if (levelScore < current.requiredScore * 0.4) {
        myFish->level = 2; // Level 2: can eat type 2 enemies
    } else if (levelScore < current.requiredScore * 0.6) {
        myFish->level = 3; // Level 3: can eat type 3 enemies
    } else {
        myFish->level = 4; // Level 4: can eat all enemies
    }

    // Generate power-ups
    if (gameTime % 50 == 0 && rand() % 100 < 50) {
        generatePowerUp();
    }

    // Move power-ups and check collisions
    for (int i = GameUtils::powerUpList.size() - 1; i >= 0; i--) {
        auto powerUp = GameUtils::powerUpList[i];
        powerUp->move();

        bool intersects = myFish->getRect().intersects(powerUp->getRect());
        bool isOutOfScreen = (powerUp->y > height);

        if (intersects || isOutOfScreen) {
            if (intersects) {
                applyPowerUpEffect(*powerUp);
            }
            GameUtils::powerUpList.erase(GameUtils::powerUpList.begin() + i);
        }
    }

    // 根据当前关卡生成敌人
    generateLevelEnemies();

    // Check if boss dash should be triggered
    checkBossDashTrigger();
    
    // Update boss dash logic if active
    if (isBossDashActive) {
        updateBossDash();
    }
    
    // Move enemies
    for (size_t i = 0; i < GameUtils::enemyList.size(); i++) {
        auto enemy = GameUtils::enemyList[i];
        enemy->x += enemy->dir * enemy->speed;
    }
    
    // Check collisions
    checkCollisions();
    
    // 检查关卡是否完成
    checkLevelComplete();
}

void GameWindow::generateEnemies() {
    // Generate enemies based on game difficulty level, not player level
    // This ensures there are always higher level enemies for challenge
    
    double spawnRand = static_cast<double>(rand()) / RAND_MAX;
    
    // Always spawn some basic enemies
    if (gameTime % 30 == 0 && spawnRand > 0.3) {
        double sideRand = static_cast<double>(rand()) / RAND_MAX;
        std::shared_ptr<Enemy> enemy;
        if (sideRand > 0.5) {
            enemy = std::make_shared<Enemy1L>();
        } else {
            enemy = std::make_shared<Enemy1R>();
        }
        GameUtils::enemyList.push_back(enemy);
    }
    
    // Spawn type 2 enemies based on game difficulty
    if (GameUtils::level >= 0 && gameTime % 40 == 0 && spawnRand > 0.5) {
        double sideRand = static_cast<double>(rand()) / RAND_MAX;
        std::shared_ptr<Enemy> enemy;
        if (sideRand > 0.5) {
            enemy = std::make_shared<Enemy2L>();
        } else {
            enemy = std::make_shared<Enemy2R>();
        }
        GameUtils::enemyList.push_back(enemy);
    }
    
    // Spawn type 3 enemies based on game difficulty
    if (GameUtils::level >= 1 && gameTime % 50 == 0 && spawnRand > 0.6) {
        double sideRand = static_cast<double>(rand()) / RAND_MAX;
        std::shared_ptr<Enemy> enemy;
        if (sideRand > 0.5) {
            enemy = std::make_shared<Enemy3L>();
        } else {
            enemy = std::make_shared<Enemy3R>();
        }
        GameUtils::enemyList.push_back(enemy);
    }
    
    // Increase game difficulty over time
    if (gameTime % 100 == 0) {
        // Gradually increase game difficulty
        GameUtils::level = std::min(GameUtils::level + 1, 3);
    }
}

// Initialize levels
void GameWindow::initLevels() {
    // Clear level list
    levels.clear();
    
    // Add 5 levels one by one
    Level level1;
    level1.levelNumber = 1;
    level1.difficulty = 0;
    level1.requiredScore = 20;
    level1.enemyTypes[0] = 80;
    level1.enemyTypes[1] = 20;
    level1.enemyTypes[2] = 0;
    level1.enemySpawnRate = 30;
    level1.hasBoss = false;
    level1.bossType = "";
    level1.levelName = "Level 1";
    level1.levelDesc = "This is level 1";
    levels.push_back(level1);
    
    Level level2;
    level2.levelNumber = 2;
    level2.difficulty = 1;
    level2.requiredScore = 50;
    level2.enemyTypes[0] = 68;
    level2.enemyTypes[1] = 26;
    level2.enemyTypes[2] = 6;
    level2.enemySpawnRate = 26;
    level2.hasBoss = false;
    level2.bossType = "";
    level2.levelName = "Level 2";
    level2.levelDesc = "This is level 2";
    levels.push_back(level2);
    
    Level level3;
    level3.levelNumber = 3;
    level3.difficulty = 2;
    level3.requiredScore = 80;
    level3.enemyTypes[0] = 56;
    level3.enemyTypes[1] = 32;
    level3.enemyTypes[2] = 12;
    level3.enemySpawnRate = 22;
    level3.hasBoss = false;
    level3.bossType = "";
    level3.levelName = "Level 3";
    level3.levelDesc = "This is level 3";
    levels.push_back(level3);
    
    Level level4;
    level4.levelNumber = 4;
    level4.difficulty = 3;
    level4.requiredScore = 110;
    level4.enemyTypes[0] = 44;
    level4.enemyTypes[1] = 38;
    level4.enemyTypes[2] = 18;
    level4.enemySpawnRate = 18;
    level4.hasBoss = false;
    level4.bossType = "";
    level4.levelName = "Level 4";
    level4.levelDesc = "This is level 4";
    levels.push_back(level4);
    
    Level level5;
    level5.levelNumber = 5;
    level5.difficulty = 4;
    level5.requiredScore = 140;
    level5.enemyTypes[0] = 32;
    level5.enemyTypes[1] = 44;
    level5.enemyTypes[2] = 24;
    level5.enemySpawnRate = 14;
    level5.hasBoss = true;
    level5.bossType = "Megashark";
    level5.levelName = "Level 5";
    level5.levelDesc = "This is level 5";
    levels.push_back(level5);
    
    // Debug info
    std::cout << "Added " << levels.size() << " levels" << std::endl;
}

// Load specified level
void GameWindow::loadLevel(int level) {
    if (level < 1 || level > levels.size()) {
        return;
    }
    
    currentLevel = level;
    levelScore = 0;
    gameTime = 0;
    
    // Clear existing enemies and power-ups
    GameUtils::enemyList.clear();
    GameUtils::powerUpList.clear();
    
    // Reset player state
    myFish = std::make_unique<MyFish>();
    
    // Set current level difficulty
    Level& current = levels[level - 1];
    GameUtils::level = current.difficulty;
    
    // Check if has boss capability
    isBoss = current.hasBoss;
    // Don't create boss immediately, wait for dash trigger
    boss = nullptr;
    
    // Reset boss dash state
    isBossDashActive = false;
    isBossDashWarning = false;
    bossDashCooldown = 0;
    lastBossDashScore = 0;
    
    state = STATE_RUNNING;
}

// Check if level is complete
void GameWindow::checkLevelComplete() {
    Level& current = levels[currentLevel - 1];
    
    if (levelScore >= current.requiredScore) {
        // Level completed
        if (currentLevel == maxUnlockedLevel && currentLevel < levels.size()) {
            maxUnlockedLevel++;
            saveProgress();
        }
        state = STATE_WIN;
    }
}

// Save game progress
void GameWindow::saveProgress() {
    std::ofstream saveFile("savegame.txt");
    if (saveFile.is_open()) {
        saveFile << maxUnlockedLevel << std::endl;
        saveFile.close();
    }
}

// Load game progress
void GameWindow::loadProgress() {
    std::ifstream saveFile("savegame.txt");
    if (saveFile.is_open()) {
        saveFile >> maxUnlockedLevel;
        saveFile.close();
    } else {
        // First time playing, only unlock first level
        maxUnlockedLevel = 1;
        saveProgress();
    }
}

// Generate enemies based on current level
void GameWindow::generateLevelEnemies() {
    if (currentLevel < 1 || currentLevel > levels.size()) {
        return;
    }
    
    Level& current = levels[currentLevel - 1];
    double spawnRand = static_cast<double>(rand()) / RAND_MAX;
    
    // Generate enemies based on level configuration
    if (gameTime % current.enemySpawnRate == 0) {
        int enemyType = rand() % 100;
        double sideRand = static_cast<double>(rand()) / RAND_MAX;
        std::shared_ptr<Enemy> enemy;
        
        if (enemyType < current.enemyTypes[0]) {
            // Generate type 1 enemy
            if (sideRand > 0.5) {
                enemy = std::make_shared<Enemy1L>();
            } else {
                enemy = std::make_shared<Enemy1R>();
            }
        } else if (enemyType < current.enemyTypes[0] + current.enemyTypes[1]) {
            // Generate type 2 enemy
            if (sideRand > 0.5) {
                enemy = std::make_shared<Enemy2L>();
            } else {
                enemy = std::make_shared<Enemy2R>();
            }
        } else {
            // Generate type 3 enemy
            if (sideRand > 0.5) {
                enemy = std::make_shared<Enemy3L>();
            } else {
                enemy = std::make_shared<Enemy3R>();
            }
        }
        
        GameUtils::enemyList.push_back(enemy);
    }
}

// Check if boss dash should be triggered
void GameWindow::checkBossDashTrigger() {
    // Check if this level has boss capability and dash is not already active
    if (isBoss && !isBossDashActive) {
        // Trigger boss dash when score reaches 50% of required score initially,
        // then every 15 points after that
        Level& current = levels[currentLevel - 1];
        bool initialTrigger = (lastBossDashScore == 0 && levelScore >= current.requiredScore * 0.5);
        bool subsequentTrigger = (lastBossDashScore > 0 && levelScore - lastBossDashScore >= 15);
        
        if ((initialTrigger || subsequentTrigger) && rand() % 100 < 20) {
            startBossDash();
        }
    }
}

// Start boss dash attack
void GameWindow::startBossDash() {
    // Create boss instance if it doesn't exist
    if (!boss) {
        boss = std::make_shared<EnemyBoss>();
        GameUtils::enemyList.push_back(boss);
    }
    
    // Record current score for next trigger
    lastBossDashScore = levelScore;
    
    // Set dash warning state
    isBossDashWarning = true;
    isBossDashActive = true;
    bossDashTimer = 60; // 2 seconds warning (30 FPS)
    
    // Randomly select dash Y position
    bossDashY = rand() % (height - 100) + 50;
    
    // Initialize warning area
    bossDashWarningArea.setSize(sf::Vector2f(width, 100));
    bossDashWarningArea.setPosition(0, bossDashY - 50);
    bossDashWarningArea.setFillColor(sf::Color(255, 0, 0, 50));
    bossDashWarningArea.setOutlineColor(sf::Color::Red);
    bossDashWarningArea.setOutlineThickness(2);
    
    // Reset boss position for dash
    boss->x = width + boss->width;
    boss->y = bossDashY - boss->height / 2;
    boss->dir = -1;
    boss->speed = 100; // Set to high speed immediately
}

// Update boss dash logic
void GameWindow::updateBossDash() {
    if (isBossDashWarning) {
        // Countdown warning time
        bossDashTimer--;
        
        if (bossDashTimer <= 0) {
            // End warning, start dash
            isBossDashWarning = false;
            bossDashTimer = 30; // Dash duration (1 second at 30 FPS)
        }
    } else {
        // Update dash movement - maintain constant high speed
        boss->x += boss->dir * boss->speed;
        
        // Check if dash is complete
        if (boss->x <= -boss->width) {
            // Reset boss state
            isBossDashActive = false;
            
            // Remove boss from enemy list
            for (int i = 0; i < GameUtils::enemyList.size(); i++) {
                if (GameUtils::enemyList[i] == boss) {
                    GameUtils::enemyList.erase(GameUtils::enemyList.begin() + i);
                    break;
                }
            }
            
            // Clear boss pointer
            boss = nullptr;
        }
    }
}

// Render boss dash warning area
void GameWindow::renderBossDashWarning() {
    if (isBossDashWarning) {
        window.draw(bossDashWarningArea);
    }
}

void GameWindow::checkCollisions() {
    auto now = std::chrono::system_clock::now().time_since_epoch().count() / 1000000;

    // Use reverse iteration to safely remove enemies during collision
    for (int i = GameUtils::enemyList.size() - 1; i >= 0; i--) {
        auto& enemy = GameUtils::enemyList[i];
        // Check collision with player
        if (myFish->getRect().intersects(enemy->getRect())) {
                if (enemy->type <= myFish->level) {
                // Player can eat enemy of same or lower type
                levelScore += enemy->count;
                GameUtils::enemyList.erase(GameUtils::enemyList.begin() + i);
                // Immediately check if the level is complete after gaining score
                checkLevelComplete();
                // If state changed to WIN, break out to avoid further game logic in this tick
                if (state == STATE_WIN) return;
            } else {
                // Enemy is higher level
                if (GameUtils::hasShield) {
                    // Shield blocks one attack regardless of duration
                    GameUtils::hasShield = false;
                    std::cout << "Shield consumed by fish!" << std::endl;
                    // Remove enemy from list
                    GameUtils::enemyList.erase(GameUtils::enemyList.begin() + i);
                } else {
                    // No shield, player is eaten
                    state = STATE_LOSE;
                }
            }
        }
        
        // Check if enemy is in boss dash area during dash
        if (isBossDashActive && !isBossDashWarning) {
            if (enemy->y >= bossDashY - 50 && enemy->y <= bossDashY + 50) {
                // Enemy is in dash path, remove it
                GameUtils::enemyList.erase(GameUtils::enemyList.begin() + i);
            }
        }
    }

    // Boss collision check and attack logic
    if (isBoss && boss && isBossDashActive) {
        // Only check collision when boss is active (dashing)
        if (boss->getRect().intersects(myFish->getRect())) {
            if (GameUtils::hasShield) {
                // Shield blocks one boss attack
                GameUtils::hasShield = false;
                std::cout << "Shield consumed by boss!" << std::endl;
            } else {
                state = STATE_LOSE;
            }
        }
    }
}



void GameWindow::generatePowerUp() {
    int x = rand() % (width - 50);
    PowerUpType type;
    // Random power-up type
    double rand_val = static_cast<double>(rand()) / RAND_MAX;

    if (rand_val < 0.33) {
        type = PowerUpType::SPEED;
    } else if (rand_val < 0.66) {
        type = PowerUpType::SHIELD;
    } else {
        type = PowerUpType::XP;
    }

    auto powerUp = std::make_shared<PowerUp>(x, -50, type);
    GameUtils::powerUpList.push_back(powerUp);
}

void GameWindow::applyPowerUpEffect(const PowerUp& powerUp) {
    // Apply power-up effect
    auto now = std::chrono::system_clock::now().time_since_epoch().count() / 1000000;

    switch (powerUp.type) {
        case PowerUpType::SPEED:
            GameUtils::hasSpeedBoost = true;
            GameUtils::speedBoostEndTime = now + GameUtils::SPEED_BOOST_DURATION * 1000;
            break;
        case PowerUpType::SHIELD:
            GameUtils::hasShield = true;
            std::cout << "获得护盾!可抵挡一次高等级敌人攻击!" << std::endl;
            break;
        case PowerUpType::XP:
            // Gain experience points for current level
            levelScore += 10;
            // Immediately check if the level is complete after gaining XP
            checkLevelComplete();
            return; // Exit early if level state changed
            break;
    }
}

void GameWindow::reGame() {
    // Restart game
    GameUtils::enemyList.clear();
    GameUtils::powerUpList.clear();
    GameUtils::hasShield = false;
    GameUtils::hasSpeedBoost = false;
    gameTime = 0;
    levelScore = 0;
    myFish->reset();
    boss = nullptr;
    isBoss = false;
}

void GameWindow::render() {
    window.clear(sf::Color::Black);

    switch (state) {
        case STATE_MENU: {
            // Render menu background
            background->paintSelf(window, myFish->level, 0);
            
            // Menu title
            GameUtils::drawText(window, ChineseText::GAME_TITLE, 650, 100, 64, sf::Color::White);
            
            // Menu buttons with improved visual effect
            sf::RectangleShape startButton(sf::Vector2f(200, 50));
            startButton.setFillColor(sf::Color(0, 100, 255, 150));
            startButton.setPosition(650, 300);
            startButton.setOutlineColor(sf::Color::White);
            startButton.setOutlineThickness(2);
            window.draw(startButton);
            GameUtils::drawText(window, ChineseText::START_GAME, 680, 310, 36, sf::Color::White);
            
            sf::RectangleShape helpButton(sf::Vector2f(100, 50));
            helpButton.setFillColor(sf::Color(0, 100, 255, 150));
            helpButton.setPosition(650, 400);
            helpButton.setOutlineColor(sf::Color::White);
            helpButton.setOutlineThickness(2);
            window.draw(helpButton);
            GameUtils::drawText(window, ChineseText::HELP, 670, 410, 36, sf::Color::White);
            
            sf::RectangleShape settingsButton(sf::Vector2f(100, 50));
            settingsButton.setFillColor(sf::Color(0, 100, 255, 150));
            settingsButton.setPosition(650, 500);
            settingsButton.setOutlineColor(sf::Color::White);
            settingsButton.setOutlineThickness(2);
            window.draw(settingsButton);
            GameUtils::drawText(window, ChineseText::SETTINGS, 660, 510, 36, sf::Color::White);
            break;
        }
        case STATE_LEVEL_SELECT: {
            // Render level select background
            background->paintSelf(window, myFish->level, 0);
            
            // Title
            GameUtils::drawText(window, ChineseText::SELECT_LEVEL, 650, 100, 48, sf::Color::White);
            
            // Render level buttons
            for (int i = 0; i < levels.size(); i++) {
                int levelX = 300 + (i % 2) * 400;
                int levelY = 200 + (i / 2) * 150;
                
                sf::RectangleShape levelButton(sf::Vector2f(300, 100));
                if (i + 1 <= maxUnlockedLevel) {
                    // Unlocked level
                    levelButton.setFillColor(sf::Color(0, 200, 100, 150));
                } else {
                    // Locked level
                    levelButton.setFillColor(sf::Color(100, 100, 100, 150));
                }
                levelButton.setPosition(levelX, levelY);
                levelButton.setOutlineColor(sf::Color::White);
                levelButton.setOutlineThickness(2);
                window.draw(levelButton);
                
                if (i + 1 <= maxUnlockedLevel) {
                        // Draw level name and info
                        GameUtils::drawText(window, levels[i].levelName, levelX + 50, levelY + 20, 24, sf::Color::White);
                        GameUtils::drawText(window, std::string(ChineseText::SCORE_PREFIX) + std::to_string(levels[i].requiredScore), levelX + 50, levelY + 50, 18, sf::Color::Yellow);
                    } else {
                    // Draw locked symbol
                    GameUtils::drawText(window, "Locked", levelX + 80, levelY + 35, 24, sf::Color::White);
                    GameUtils::drawText(window, "Complete previous level to unlock", levelX + 70, levelY + 65, 16, sf::Color(128, 128, 128));
                }
            }
            
            // Back button
            sf::RectangleShape backButton(sf::Vector2f(200, 50));
            backButton.setFillColor(sf::Color(0, 100, 255, 150));
            backButton.setPosition(650, 600);
            backButton.setOutlineColor(sf::Color::White);
            backButton.setOutlineThickness(2);
            window.draw(backButton);
            GameUtils::drawText(window, ChineseText::BACK_TO_MENU, 670, 610, 36, sf::Color::White);
            break;
        }
        case STATE_START: {
            background->paintSelf(window, myFish->level, 0);
            break;
        }
        case STATE_RUNNING: {
            background->paintSelf(window, myFish->level, 1);
            
            // Render boss dash warning area if active
            renderBossDashWarning();
            
            myFish->paintSelf(window);
            
            for (auto& enemy : GameUtils::enemyList) {
                enemy->paintSelf(window);
            }
            
            for (auto& powerUp : GameUtils::powerUpList) {
                powerUp->paintSelf(window);
            }
            
            if (isBoss && boss) {
                boss->paintSelf(window);
            }
            
            // Draw game info using simple graphical elements
            
            // Draw a background panel with improved design
            sf::RectangleShape infoPanel(sf::Vector2f(300, 150));
            infoPanel.setFillColor(sf::Color(0, 0, 0, 180));
            infoPanel.setPosition(10, 10);
            infoPanel.setOutlineColor(sf::Color(255, 255, 255, 100));
            infoPanel.setOutlineThickness(1);
            window.draw(infoPanel);
            
            // Draw current level info
            Level& current = levels[currentLevel - 1];
            GameUtils::drawText(window, std::string(ChineseText::LEVEL_PREFIX) + current.levelName, 20, 20, 24, sf::Color::Cyan);

            // Draw current score
            GameUtils::drawText(window, std::string(ChineseText::SCORE_PREFIX) + std::to_string(levelScore), 20, 50, 20, sf::Color::White);

            // Draw required score (target)
            GameUtils::drawText(window, std::string(ChineseText::TARGET_PREFIX) + std::to_string(current.requiredScore), 20, 80, 20, sf::Color::Yellow);
            
            // Draw player level
            GameUtils::drawText(window, "Player Level: " + std::to_string(myFish->level), 20, 110, 20, sf::Color::Green);
            
            // Draw progress bar
            sf::RectangleShape progressBarBg(sf::Vector2f(280, 15));
            progressBarBg.setFillColor(sf::Color(50, 50, 50, 200));
            progressBarBg.setPosition(20, 130);
            window.draw(progressBarBg);
            
            float progress = static_cast<float>(levelScore) / current.requiredScore;
            sf::RectangleShape progressBar(sf::Vector2f(280 * progress, 15));
            progressBar.setFillColor(sf::Color(0, 255, 0, 200));
            progressBar.setPosition(20, 130);
            window.draw(progressBar);
            break;
        }
        case STATE_LOSE: {
            background->paintSelf(window, myFish->level, 2);
            
            // Draw game over text with improved design
            sf::RectangleShape gameOverPanel(sf::Vector2f(400, 200));
            gameOverPanel.setFillColor(sf::Color(0, 0, 0, 200));
            gameOverPanel.setPosition(520, 300);
            gameOverPanel.setOutlineColor(sf::Color::Red);
            gameOverPanel.setOutlineThickness(2);
            window.draw(gameOverPanel);
            
            GameUtils::drawText(window, ChineseText::GAME_OVER, 600, 350, 48, sf::Color::Red);
            GameUtils::drawText(window, ChineseText::CLICK_TO_CONTINUE, 620, 420, 24, sf::Color::White);
            break;
        }
        case STATE_WIN: {
            background->paintSelf(window, myFish->level, 3);
            
            // Draw win text with improved design
            sf::RectangleShape winPanel(sf::Vector2f(400, 200));
            winPanel.setFillColor(sf::Color(0, 0, 0, 200));
            winPanel.setPosition(520, 300);
            winPanel.setOutlineColor(sf::Color::Green);
            winPanel.setOutlineThickness(2);
            window.draw(winPanel);
            
            GameUtils::drawText(window, ChineseText::YOU_WIN, 630, 350, 48, sf::Color::Green);
            GameUtils::drawText(window, ChineseText::CLICK_TO_RESTART, 620, 420, 24, sf::Color::White);
            break;
        }
        case STATE_PAUSE: {
            background->paintSelf(window, myFish->level, 4);
            
            // Draw pause screen
            sf::RectangleShape pausePanel(sf::Vector2f(300, 150));
            pausePanel.setFillColor(sf::Color(0, 0, 0, 200));
            pausePanel.setPosition(570, 350);
            pausePanel.setOutlineColor(sf::Color::Blue);
            pausePanel.setOutlineThickness(2);
            window.draw(pausePanel);
            
            GameUtils::drawText(window, ChineseText::GAME_PAUSED, 620, 400, 36, sf::Color::White);
            GameUtils::drawText(window, ChineseText::PRESS_SPACE_TO_CONTINUE, 610, 450, 24, sf::Color(128, 128, 128));
            break;
        }
        case STATE_HELP: {
            // Render help background
            background->paintSelf(window, myFish->level, 0);
            
            // Help title
            GameUtils::drawText(window, ChineseText::HELP_TITLE, 600, 100, 64, sf::Color::White);
            
            // Help content with improved layout
            sf::RectangleShape helpPanel(sf::Vector2f(800, 400));
            helpPanel.setFillColor(sf::Color(0, 0, 0, 150));
            helpPanel.setPosition(320, 200);
            helpPanel.setOutlineColor(sf::Color::White);
            helpPanel.setOutlineThickness(1);
            window.draw(helpPanel);
            
            GameUtils::drawText(window, ChineseText::HELP_CONTROLS, 350, 250, 32, sf::Color::White);
            GameUtils::drawText(window, ChineseText::HELP_EAT, 350, 300, 32, sf::Color::White);
            GameUtils::drawText(window, ChineseText::HELP_AVOID, 350, 350, 32, sf::Color::White);
            GameUtils::drawText(window, ChineseText::HELP_POWERUPS, 350, 400, 32, sf::Color::White);
            GameUtils::drawText(window, ChineseText::HELP_WIN, 350, 450, 32, sf::Color::White);
            GameUtils::drawText(window, ChineseText::HELP_PAUSE, 350, 500, 32, sf::Color::White);
            
            // Back button
            sf::RectangleShape backButton(sf::Vector2f(200, 50));
            backButton.setFillColor(sf::Color(0, 100, 255, 150));
            backButton.setPosition(650, 600);
            backButton.setOutlineColor(sf::Color::White);
            backButton.setOutlineThickness(2);
            window.draw(backButton);
            GameUtils::drawText(window, ChineseText::HELP_BACK, 690, 610, 36, sf::Color::White);
            break;
        }
        case STATE_SETTINGS: {
            // Render settings background
            background->paintSelf(window, myFish->level, 0);
            
            // Settings title
            GameUtils::drawText(window, ChineseText::SETTINGS_TITLE, 600, 100, 64, sf::Color::White);
            
            // Settings content with improved layout
            sf::RectangleShape settingsPanel(sf::Vector2f(400, 200));
            settingsPanel.setFillColor(sf::Color(0, 0, 0, 150));
            settingsPanel.setPosition(520, 300);
            settingsPanel.setOutlineColor(sf::Color::White);
            settingsPanel.setOutlineThickness(1);
            window.draw(settingsPanel);
            
            GameUtils::drawText(window, ChineseText::SETTINGS_VOLUME, 650, 350, 48, sf::Color::White);
            GameUtils::drawText(window, ChineseText::SETTINGS_SOUND, 650, 420, 48, sf::Color::White);
            
            // Back button
            sf::RectangleShape backButton(sf::Vector2f(200, 50));
            backButton.setFillColor(sf::Color(0, 100, 255, 150));
            backButton.setPosition(650, 600);
            backButton.setOutlineColor(sf::Color::White);
            backButton.setOutlineThickness(2);
            window.draw(backButton);
            GameUtils::drawText(window, ChineseText::SETTINGS_BACK, 670, 610, 36, sf::Color::White);
            break;
        }
        default: {
            break;
        }
    }

    window.display();
}