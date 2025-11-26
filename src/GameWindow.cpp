#include "GameWindow.h"
#include "GameUtils.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>

GameWindow::GameWindow()
    : window(sf::VideoMode(1440, 900), "Eat Fish"),
      width(1440), height(900), state(STATE_START), gameTime(0), isBoss(false) {
    
    srand(static_cast<unsigned>(time(0)));
    
    window.setFramerateLimit(25);
    
    myFish = std::make_unique<MyFish>();
    background = std::make_unique<Background>();
    
    std::string assetsDir = "assets/";
    if (!GameUtils::loadTextures(assetsDir)) {
        std::cerr << "Warning: Some textures failed to load!" << std::endl;
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
                if (state == STATE_START) {
                    state = STATE_RUNNING;
                } else if (state == STATE_LOSE || state == STATE_WIN) {
                    reGame();
                    state = STATE_RUNNING;
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
                if (state == STATE_RUNNING) {
                    state = STATE_PAUSE;
                } else if (state == STATE_PAUSE) {
                    state = STATE_RUNNING;
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
    checkShieldExpiration();

    // Level difficulty
    if (GameUtils::count < 5) {
        GameUtils::level = 0;
        myFish->level = 1;
    } else if (GameUtils::count <= 15) {
        GameUtils::level = 1;
        myFish->level = 1;
    } else if (GameUtils::count <= 50) {
        GameUtils::level = 2;
        myFish->level = 2;
    } else if (GameUtils::count <= 150) {
        GameUtils::level = 3;
        myFish->level = 3;
    } else if (GameUtils::count <= 200) {
        GameUtils::level = 4;
        myFish->level = 3;
    } else if (GameUtils::count > 200) {
        state = STATE_WIN;
    }

    // Power-up generation logic
    if (gameTime % 50 == 0 && rand() % 100 < 50) {
        generatePowerUp();
    }

    // Power-up movement and collision detection
    for (int i = GameUtils::powerUpList.size() - 1; i >= 0; i--) {
        auto& powerUp = GameUtils::powerUpList[i];
        powerUp->move();

        // Check collision with player
        if (myFish->getRect().intersects(powerUp->getRect())) {
            applyPowerUpEffect(powerUp);
            GameUtils::powerUpList.erase(GameUtils::powerUpList.begin() + i);
            continue;
        }

        // Remove power-ups off-screen
        if (powerUp->y > height) {
            GameUtils::powerUpList.erase(GameUtils::powerUpList.begin() + i);
        }
    }

    // Generate enemies
    generateEnemies();

    // Check collisions
    checkCollisions();
}

void GameWindow::generateEnemies() {
    double random = static_cast<double>(rand()) / RAND_MAX;

    switch (GameUtils::level) {
        case 4:
            if (gameTime % 60 == 0 && random > 0.5 && !isBoss) {
                boss = std::make_shared<EnemyBoss>();
                isBoss = true;
            }
            // Fall through
        case 3:
            if (gameTime % 30 == 0 && random > 0.5) {
                std::shared_ptr<Enemy> enemy;
                if (random > 0.5) {
                    enemy = std::make_shared<Enemy3L>();
                } else {
                    enemy = std::make_shared<Enemy3R>();
                }
                GameUtils::enemyList.push_back(enemy);
            }
            // Fall through
        case 2:
            if (gameTime % 20 == 0 && random > 0.5) {
                std::shared_ptr<Enemy> enemy;
                if (random > 0.5) {
                    enemy = std::make_shared<Enemy2L>();
                } else {
                    enemy = std::make_shared<Enemy2R>();
                }
                GameUtils::enemyList.push_back(enemy);
            }
            // Fall through
        case 1:
            if (gameTime % 20 == 0 && random > 0.5) {
                std::shared_ptr<Enemy> enemy;
                if (random > 0.5) {
                    enemy = std::make_shared<Enemy1L>();
                } else {
                    enemy = std::make_shared<Enemy1R>();
                }
                GameUtils::enemyList.push_back(enemy);
            }
            // Fall through
        case 0:
            if (gameTime % 10 == 0 && random > 0.5) {
                std::shared_ptr<Enemy> enemy;
                if (random > 0.5) {
                    enemy = std::make_shared<Enemy1L>();
                } else {
                    enemy = std::make_shared<Enemy1R>();
                }
                GameUtils::enemyList.push_back(enemy);
            }
            break;
    }
}

void GameWindow::checkCollisions() {
    auto now = std::chrono::system_clock::now().time_since_epoch().count() / 1000000;

    for (auto& enemy : GameUtils::enemyList) {
        enemy->x += enemy->dir * enemy->speed;

        // Check collision with player
        if (myFish->getRect().intersects(enemy->getRect())) {
            if (GameUtils::hasShield && now <= GameUtils::shieldEndTime) {
                if (enemy->type > myFish->level) {
                    GameUtils::hasShield = false;
                    std::cout << "Shield consumed by fish!" << std::endl;
                    enemy->x = -200;
                    enemy->y = -200;
                } else {
                    enemy->x = -200;
                    enemy->y = -200;
                    GameUtils::count += enemy->count;
                }
            } else if (myFish->level >= enemy->type) {
                enemy->x = -200;
                enemy->y = -200;
                GameUtils::count += enemy->count;
            } else {
                state = STATE_LOSE;
            }
        }
    }

    // Boss collision detection
    if (isBoss && boss) {
        boss->x += boss->dir * boss->speed;
        
        if (boss->getRec().intersects(myFish->getRect())) {
            if (GameUtils::hasShield && now <= GameUtils::shieldEndTime) {
                GameUtils::hasShield = false;
                std::cout << "Shield consumed by boss!" << std::endl;
            } else {
                state = STATE_LOSE;
            }
        }
    }
}

void GameWindow::checkShieldExpiration() {
    auto now = std::chrono::system_clock::now().time_since_epoch().count() / 1000000;
    if (GameUtils::hasShield && now > GameUtils::shieldEndTime) {
        GameUtils::hasShield = false;
        std::cout << "Shield expired!" << std::endl;
    }
}

void GameWindow::generatePowerUp() {
    int x = rand() % (width - 50);
    PowerUpType type;
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

void GameWindow::applyPowerUpEffect(const std::shared_ptr<PowerUp>& powerUp) {
    auto now = std::chrono::system_clock::now().time_since_epoch().count() / 1000000;

    switch (powerUp->type) {
        case PowerUpType::SPEED:
            GameUtils::hasSpeedBoost = true;
            GameUtils::speedBoostEndTime = now + GameUtils::SPEED_BOOST_DURATION * 1000;
            break;
        case PowerUpType::SHIELD:
            GameUtils::hasShield = true;
            GameUtils::shieldEndTime = now + GameUtils::SHIELD_DURATION * 1000;
            std::cout << "Shield acquired! Duration: " << GameUtils::SHIELD_DURATION << "ms" << std::endl;
            break;
        case PowerUpType::XP:
            GameUtils::count += 10;
            break;
    }
}

void GameWindow::reGame() {
    GameUtils::enemyList.clear();
    GameUtils::powerUpList.clear();
    GameUtils::hasShield = false;
    GameUtils::hasSpeedBoost = false;
    gameTime = 0;
    myFish->reset();
    GameUtils::count = 0;
    boss = nullptr;
    isBoss = false;
}

void GameWindow::render() {
    window.clear(sf::Color::Black);

    switch (state) {
        case STATE_START:
            background->paintSelf(window, myFish->level, 0);
            break;
        case STATE_RUNNING:
            background->paintSelf(window, myFish->level, 1);
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
            break;
        case STATE_LOSE:
            background->paintSelf(window, myFish->level, 2);
            break;
        case STATE_WIN:
            background->paintSelf(window, myFish->level, 3);
            break;
        case STATE_PAUSE:
            background->paintSelf(window, myFish->level, 4);
            break;
        default:
            break;
    }

    window.display();
}
