#include "GameUtils.h"
#include <iostream>

// static member initialization
bool GameUtils::UP = false;
bool GameUtils::DOWN = false;
bool GameUtils::LEFT = false;
bool GameUtils::RIGHT = false;

int GameUtils::count = 60;
int GameUtils::level = 0;

std::vector<std::shared_ptr<Enemy>> GameUtils::enemyList;
std::vector<std::shared_ptr<PowerUp>> GameUtils::powerUpList;

sf::Texture GameUtils::bgTexture;
sf::Texture GameUtils::enamy1Texture;
sf::Texture GameUtils::enamyr1Texture;
sf::Texture GameUtils::enamy2Texture;
sf::Texture GameUtils::enamyr2Texture;
sf::Texture GameUtils::enamy3Texture;
sf::Texture GameUtils::enamyr3Texture;
sf::Texture GameUtils::bossTexture;
sf::Texture GameUtils::myFishLeftTexture;
sf::Texture GameUtils::myFishRightTexture;
sf::Texture GameUtils::powerupSpeedTexture;
sf::Texture GameUtils::powerupShieldTexture;
sf::Texture GameUtils::powerupXpTexture;

bool GameUtils::hasShield = false;
bool GameUtils::hasSpeedBoost = false;
long long GameUtils::shieldEndTime = 0;
long long GameUtils::speedBoostEndTime = 0;

// debug visual flag default: off for normal gameplay
bool GameUtils::debugVisual = false;
// logLoads default: enable logging of successful loads
bool GameUtils::logLoads = true;

bool GameUtils::loadTextures(const std::string& assetsDir) {
    bool allLoaded = true;

    // 加载背景
    if (!bgTexture.loadFromFile(assetsDir + "sea.png")) {
        std::cerr << "Failed to load sea.png" << std::endl;
        allLoaded = false;
    } else if (logLoads) {
        auto s = bgTexture.getSize();
        std::cout << "Loaded sea.png " << s.x << "x" << s.y << std::endl;
    }

    // 加载敌人纹理
    if (debugVisual) std::cout << "Loading enemy textures..." << std::endl;
    {
        sf::Image img;
        if (!img.loadFromFile(assetsDir + "enemyFish/fish1_r.png")) {
            std::cerr << "Failed to load fish1_r.png" << std::endl;
            allLoaded = false;
        } else {
            // Do NOT mask enemy images here; they may already contain proper alpha.
            enamy1Texture.loadFromImage(img);
            enamy1Texture.setSmooth(true);
            if (logLoads) {
                auto s = enamy1Texture.getSize();
                std::cout << "Loaded enemy fish1_r.png " << s.x << "x" << s.y << std::endl;
            }
        }
    }
    {
        sf::Image img;
        if (!img.loadFromFile(assetsDir + "enemyFish/fish1_l.png")) {
            std::cerr << "Failed to load fish1_l.png" << std::endl;
            allLoaded = false;
        } else {
            // Apply white mask for transparent background on left-facing fish
            img.createMaskFromColor(sf::Color::White, 0);
            enamyr1Texture.loadFromImage(img);
            enamyr1Texture.setSmooth(true);
            if (logLoads) {
                auto s = enamyr1Texture.getSize();
                std::cout << "Loaded enemy fish1_l.png " << s.x << "x" << s.y << std::endl;
            }
        }
    }
    {
        sf::Image img;
        if (!img.loadFromFile(assetsDir + "enemyFish/fish2_r.png")) {
            std::cerr << "Failed to load fish2_r.png" << std::endl;
            allLoaded = false;
        } else {
            enamy2Texture.loadFromImage(img);
            enamy2Texture.setSmooth(true);
            if (logLoads) {
                auto s = enamy2Texture.getSize();
                std::cout << "Loaded enemy fish2_r.png " << s.x << "x" << s.y << std::endl;
            }
        }
    }
    {
        sf::Image img;
        if (!img.loadFromFile(assetsDir + "enemyFish/fish2_l.png")) {
            std::cerr << "Failed to load fish2_l.png" << std::endl;
            allLoaded = false;
        } else {
            // Apply white mask for transparent background on left-facing fish
            img.createMaskFromColor(sf::Color::White, 0);
            enamyr2Texture.loadFromImage(img);
            enamyr2Texture.setSmooth(true);
            if (logLoads) {
                auto s = enamyr2Texture.getSize();
                std::cout << "Loaded enemy fish2_l.png " << s.x << "x" << s.y << std::endl;
            }
        }
    }
    {
        sf::Image img;
        if (!img.loadFromFile(assetsDir + "enemyFish/fish3_r.png")) {
            std::cerr << "Failed to load fish3_r.png" << std::endl;
            allLoaded = false;
        } else {
            enamy3Texture.loadFromImage(img);
            enamy3Texture.setSmooth(true);
            if (logLoads) {
                auto s = enamy3Texture.getSize();
                std::cout << "Loaded enemy fish3_r.png " << s.x << "x" << s.y << std::endl;
            }
        }
    }
    {
        sf::Image img;
        if (!img.loadFromFile(assetsDir + "enemyFish/fish3_l.png")) {
            std::cerr << "Failed to load fish3_l.png" << std::endl;
            allLoaded = false;
        } else {
            // Apply white mask for transparent background on left-facing fish
            img.createMaskFromColor(sf::Color::White, 0);
            enamyr3Texture.loadFromImage(img);
            enamyr3Texture.setSmooth(true);
            if (logLoads) {
                auto s = enamyr3Texture.getSize();
                std::cout << "Loaded enemy fish3_l.png " << s.x << "x" << s.y << std::endl;
            }
        }
    }
    {
        sf::Image img;
        if (!img.loadFromFile(assetsDir + "enemyFish/boss.png")) {
            std::cerr << "Failed to load boss.png" << std::endl;
            allLoaded = false;
        } else {
            // boss image: do not mask here; preserve existing alpha
            bossTexture.loadFromImage(img);
            bossTexture.setSmooth(true);
            if (logLoads) {
                auto s = bossTexture.getSize();
                std::cout << "Loaded boss.png " << s.x << "x" << s.y << std::endl;
            }
        }
    }

    // (image-based loading above already applied mask + smoothing when needed)

    // 加载玩家鱼的纹理
    {
        sf::Image img;
        if (!img.loadFromFile(assetsDir + "myFish/myfish_left.png")) {
            std::cerr << "Failed to load myfish_left.png" << std::endl;
            allLoaded = false;
        } else {
            img.createMaskFromColor(sf::Color::White, 0);
            myFishLeftTexture.loadFromImage(img);
            myFishLeftTexture.setSmooth(true);
            if (debugVisual) {
                auto s = myFishLeftTexture.getSize();
                std::cout << "Loaded myfish_left.png " << s.x << "x" << s.y << std::endl;
            }
        }
    }
    {
        sf::Image img;
        if (!img.loadFromFile(assetsDir + "myFish/myfish_right.png")) {
            std::cerr << "Failed to load myfish_right.png" << std::endl;
            allLoaded = false;
        } else {
            img.createMaskFromColor(sf::Color::White, 0);
            myFishRightTexture.loadFromImage(img);
            myFishRightTexture.setSmooth(true);
            if (debugVisual) {
                auto s = myFishRightTexture.getSize();
                std::cout << "Loaded myfish_right.png " << s.x << "x" << s.y << std::endl;
            }
        }
    }

    // (player textures already created via sf::Image and smoothing applied)

    // 加载道具纹理（对 jpg 使用白色掩码以去掉背景）
    {
        sf::Image img;
        if (!img.loadFromFile(assetsDir + "powerup/icon_powerup_speed.png")) {
            std::cerr << "Failed to load icon_powerup_speed.png" << std::endl;
            allLoaded = false;
        } else {
            img.createMaskFromColor(sf::Color::White, 0);
            powerupSpeedTexture.loadFromImage(img);
            powerupSpeedTexture.setSmooth(true);
        }
    }
    if (!powerupShieldTexture.loadFromFile(assetsDir + "powerup/icon_powerup_shield.png")) {
        std::cerr << "Failed to load icon_powerup_shield.png" << std::endl;
        allLoaded = false;
    } else {
        powerupShieldTexture.setSmooth(true);
    }
    {
        sf::Image img;
        if (!img.loadFromFile(assetsDir + "powerup/icon_powerup_xp.png")) {
            std::cerr << "Failed to load icon_powerup_xp.png" << std::endl;
            allLoaded = false;
        } else {
            img.createMaskFromColor(sf::Color::White, 0);
            powerupXpTexture.loadFromImage(img);
            powerupXpTexture.setSmooth(true);
        }
    }

    // Shield icon may include transparent areas already; ensure smoothing for nicer scaling
    powerupShieldTexture.setSmooth(true);

    // Background texture smoothing (helps when it's scaled to window)
    bgTexture.setSmooth(true);

    return allLoaded;
}

void GameUtils::drawText(sf::RenderWindow& window, const std::string& text,
                        int x, int y, int fontSize, const sf::Color& color) {
    // Use SFML's sf::Font and sf::Text to render text, supporting Chinese display
    static sf::Font font;
    static bool fontLoaded = false;
    
    // Load font only once
    if (!fontLoaded) {
        // Try to load Chinese system fonts
        if (font.loadFromFile("C:/Windows/Fonts/simhei.ttf")) {
            std::cout << "Successfully loaded simhei.ttf" << std::endl;
            fontLoaded = true;
        } else if (font.loadFromFile("C:/Windows/Fonts/msyh.ttc")) {
            std::cout << "Successfully loaded msyh.ttc" << std::endl;
            fontLoaded = true;
        } else if (font.loadFromFile("C:/Windows/Fonts/simsun.ttc")) {
            std::cout << "Successfully loaded simsun.ttc" << std::endl;
            fontLoaded = true;
        } else {
            std::cerr << "Failed to load Chinese font, using default font" << std::endl;
            fontLoaded = true;
        }
    }
    
    // Create text object with Chinese support
    sf::String sfString;
    
    // Check if the text is English and needs Chinese translation
    if (text == "Score: ") {
        // "分数: " in UTF-8 - correct sequence
        const unsigned char chineseText[] = {0xE5, 0x88, 0x86, 0xE6, 0x95, 0xB0, 0x3A, 0x20, 0x00};
        sfString = sf::String::fromUtf8(reinterpret_cast<const char*>(chineseText), reinterpret_cast<const char*>(chineseText + sizeof(chineseText) - 1));
    } else if (text.find("Score: ") == 0) {
        // Handle dynamic text like "Score: 100"
        std::string numberPart = text.substr(7); // Get everything after "Score: "
        const unsigned char prefix[] = {0xE5, 0x88, 0x86, 0xE6, 0x95, 0xB0, 0x3A, 0x20};
        std::string chineseText(reinterpret_cast<const char*>(prefix), sizeof(prefix));
        chineseText += numberPart;
        sfString = sf::String::fromUtf8(chineseText.begin(), chineseText.end());
    } else if (text == "Level: ") {
        // "等级: " in UTF-8
        const unsigned char chineseText[] = {0xE7, 0xAD, 0x89, 0xE7, 0xBA, 0xA7, 0x3A, 0x20, 0x00};
        sfString = sf::String::fromUtf8(reinterpret_cast<const char*>(chineseText), reinterpret_cast<const char*>(chineseText + sizeof(chineseText) - 1));
    } else if (text.find("Level: ") == 0) {
        // Handle dynamic text like "Level: 5"
        std::string numberPart = text.substr(7); // Get everything after "Level: "
        const unsigned char prefix[] = {0xE7, 0xAD, 0x89, 0xE7, 0xBA, 0xA7, 0x3A, 0x20};
        std::string chineseText(reinterpret_cast<const char*>(prefix), sizeof(prefix));
        chineseText += numberPart;
        sfString = sf::String::fromUtf8(chineseText.begin(), chineseText.end());
    } else if (text == "Win Req: 50") {
        // "胜利所需: 50" in UTF-8
        const unsigned char chineseText[] = {0xE8, 0x83, 0x9C, 0xE5, 0x88, 0xA9, 0xE6, 0x89, 0x80, 0xE9, 0x9C, 0x80, 0x3A, 0x20, 0x35, 0x30, 0x00};
        sfString = sf::String::fromUtf8(reinterpret_cast<const char*>(chineseText), reinterpret_cast<const char*>(chineseText + sizeof(chineseText) - 1));
    } else if (text == "GAME OVER!") {
        // "游戏结束!" in UTF-8
        const unsigned char chineseText[] = {0xE6, 0xB8, 0xB8, 0xE6, 0x88, 0x8F, 0xE7, 0xBB, 0x93, 0xE6, 0x9D, 0x9F, 0x21, 0x00};
        sfString = sf::String::fromUtf8(reinterpret_cast<const char*>(chineseText), reinterpret_cast<const char*>(chineseText + sizeof(chineseText) - 1));
    } else if (text == "YOU WIN!") {
        // "你胜利了!" in UTF-8
        const unsigned char chineseText[] = {0xE4, 0xBD, 0xA0, 0xE8, 0x83, 0x9C, 0xE5, 0x88, 0xA9, 0xE4, 0xBA, 0x86, 0x21, 0x00};
        sfString = sf::String::fromUtf8(reinterpret_cast<const char*>(chineseText), reinterpret_cast<const char*>(chineseText + sizeof(chineseText) - 1));
    } else if (text == "Click to Restart") {
        // "点击重新开始" in UTF-8
        const unsigned char chineseText[] = {0xE7, 0x82, 0xB9, 0xE5, 0x87, 0xBB, 0xE9, 0x87, 0x8D, 0xE6, 0x96, 0xB0, 0xE5, 0xBC, 0x80, 0xE5, 0xA7, 0x8B, 0x00};
        sfString = sf::String::fromUtf8(reinterpret_cast<const char*>(chineseText), reinterpret_cast<const char*>(chineseText + sizeof(chineseText) - 1));
    } else if (text == "Eat Fish") {
        // "吃小鱼" in UTF-8
        const unsigned char chineseText[] = {0xE5, 0x90, 0x83, 0xE5, 0xB0, 0x8F, 0xE9, 0xB1, 0xBC, 0x00};
        sfString = sf::String::fromUtf8(reinterpret_cast<const char*>(chineseText), reinterpret_cast<const char*>(chineseText + sizeof(chineseText) - 1));
    } else if (text == "Start Game") {
        // "开始游戏" in UTF-8
        const unsigned char chineseText[] = {0xE5, 0xBC, 0x80, 0xE5, 0xA7, 0x8B, 0xE6, 0xB8, 0xB8, 0xE6, 0x88, 0x8F, 0x00};
        sfString = sf::String::fromUtf8(reinterpret_cast<const char*>(chineseText), reinterpret_cast<const char*>(chineseText + sizeof(chineseText) - 1));
    } else if (text == "Help") {
        // "帮助" in UTF-8
        const unsigned char chineseText[] = {0xE5, 0xB8, 0xAE, 0xE5, 0x8A, 0xA9, 0x00};
        sfString = sf::String::fromUtf8(reinterpret_cast<const char*>(chineseText), reinterpret_cast<const char*>(chineseText + sizeof(chineseText) - 1));
    } else if (text == "Settings") {
        // "设置" in UTF-8
        const unsigned char chineseText[] = {0xE8, 0xAE, 0xBE, 0xE7, 0xBD, 0xAE, 0x00};
        sfString = sf::String::fromUtf8(reinterpret_cast<const char*>(chineseText), reinterpret_cast<const char*>(chineseText + sizeof(chineseText) - 1));
    } else if (text == "Game Help") {
        // "游戏帮助" in UTF-8
        const unsigned char chineseText[] = {0xE6, 0xB8, 0xB8, 0xE6, 0x88, 0x8F, 0xE5, 0xB8, 0xAE, 0xE5, 0x8A, 0xA9, 0x00};
        sfString = sf::String::fromUtf8(reinterpret_cast<const char*>(chineseText), reinterpret_cast<const char*>(chineseText + sizeof(chineseText) - 1));
    } else if (text == "Game Settings") {
        // "游戏设置" in UTF-8
        const unsigned char chineseText[] = {0xE6, 0xB8, 0xB8, 0xE6, 0x88, 0x8F, 0xE8, 0xAE, 0xBE, 0xE7, 0xBD, 0xAE, 0x00};
        sfString = sf::String::fromUtf8(reinterpret_cast<const char*>(chineseText), reinterpret_cast<const char*>(chineseText + sizeof(chineseText) - 1));
    } else if (text == "Use W, A, S, D to control the fish") {
        // "使用 W、A、S、D 控制小鱼"
        const unsigned char chineseText[] = {0xE4,0xBD,0xBF,0xE7,0x94,0xA8,0x20,0x57,0xEF,0xBC,0x8C,0x41,0xEF,0xBC,0x8C,0x53,0xEF,0xBC,0x8C,0x44,0x20,0xE6,0x8E,0xA7,0xE5,0x88,0xB6,0xE5,0xB0,0x8F,0xE9,0xB1,0xBC,0x00};
        sfString = sf::String::fromUtf8(reinterpret_cast<const char*>(chineseText), reinterpret_cast<const char*>(chineseText + sizeof(chineseText) - 1));
    } else if (text == "Eat smaller fish to grow") {
        // "吞食更小的鱼以成长"
        const unsigned char chineseText[] = {0xE5,0x90,0x9E,0xE9,0xA3,0x9F,0xE6,0x9B,0xB4,0xE5,0xB0,0x8F,0xE7,0x9A,0x84,0xE9,0xB1,0xBC,0xE4,0xBB,0xA5,0xE6,0x88,0x90,0xE9,0x95,0xBF,0x00};
        sfString = sf::String::fromUtf8(reinterpret_cast<const char*>(chineseText), reinterpret_cast<const char*>(chineseText + sizeof(chineseText) - 1));
    } else if (text == "Avoid larger fish") {
        // "躲避更大的鱼"
        const unsigned char chineseText[] = {0xE8,0xBA,0xB2,0xE9,0x81,0xBF,0xE6,0x9B,0xB4,0xE5,0xA4,0xA7,0xE7,0x9A,0x84,0xE9,0xB1,0xBC,0x00};
        sfString = sf::String::fromUtf8(reinterpret_cast<const char*>(chineseText), reinterpret_cast<const char*>(chineseText + sizeof(chineseText) - 1));
    } else if (text == "Collect power-ups for special abilities") {
        // "收集道具以获得特殊能力"
        const unsigned char chineseText[] = {0xE6,0x94,0xB6,0xE9,0x9B,0x86,0xE9,0x81,0x93,0xE5,0x85,0xB7,0xE4,0xBB,0xA5,0xE8,0x8E,0xB7,0xE5,0xBE,0x97,0xE7,0x89,0xB9,0xE6,0xAE,0x8A,0xE8,0x83,0xBD,0xE5,0x8A,0x9B,0x00};
        sfString = sf::String::fromUtf8(reinterpret_cast<const char*>(chineseText), reinterpret_cast<const char*>(chineseText + sizeof(chineseText) - 1));
    } else if (text == "Reach 50 points to win") {
        // "达到目标分数以获胜"
        const unsigned char chineseText[] = {0xE8,0xBE,0xBE,0xE5,0x88,0xB0,0xE7,0x9B,0xAE,0xE6,0xA0,0x87,0xE5,0x88,0x86,0xE6,0x95,0xB0,0xE4,0xBB,0xA5,0xE8,0x8E,0xB7,0xE8,0x83,0x9C,0x00};
        sfString = sf::String::fromUtf8(reinterpret_cast<const char*>(chineseText), reinterpret_cast<const char*>(chineseText + sizeof(chineseText) - 1));
    } else if (text == "Press Space to pause the game") {
        // "按空格暂停游戏"
        const unsigned char chineseText[] = {0xE6,0x8C,0x89,0xE7,0xA9,0xBA,0xE6,0xA0,0x87,0xE6,0x9A,0x82,0xE6,0xAD,0xA2,0xE6,0xB8,0xB8,0xE6,0x88,0x8F,0x00};
        sfString = sf::String::fromUtf8(reinterpret_cast<const char*>(chineseText), reinterpret_cast<const char*>(chineseText + sizeof(chineseText) - 1));
    } else if (text == "Volume") {
        // "音量"
        const unsigned char chineseText[] = {0xE9,0x9F,0xB3,0xE9,0x87,0x8F,0x00};
        sfString = sf::String::fromUtf8(reinterpret_cast<const char*>(chineseText), reinterpret_cast<const char*>(chineseText + sizeof(chineseText) - 1));
    } else if (text == "Sound Effects") {
        // "音效"
        const unsigned char chineseText[] = {0xE9,0x9F,0xB3,0xE6,0x95,0x88,0x00};
        sfString = sf::String::fromUtf8(reinterpret_cast<const char*>(chineseText), reinterpret_cast<const char*>(chineseText + sizeof(chineseText) - 1));
    } else if (text == "Back to Menu") {
        // "返回菜单" in UTF-8
        const unsigned char chineseText[] = {0xE8, 0xBF, 0x94, 0xE5, 0x9B, 0x9E, 0xE8, 0x8F, 0x9C, 0xE5, 0x8D, 0x95, 0x00};
        sfString = sf::String::fromUtf8(reinterpret_cast<const char*>(chineseText), reinterpret_cast<const char*>(chineseText + sizeof(chineseText) - 1));
    } else if (text == "Target: ") {
        // "目标: " in UTF-8
        const unsigned char chineseText[] = {0xE7,0x9B,0xAE,0xE6,0xA0,0x87,0x3A,0x20,0x00};
        sfString = sf::String::fromUtf8(reinterpret_cast<const char*>(chineseText), reinterpret_cast<const char*>(chineseText + sizeof(chineseText) - 1));
    } else if (text.find("Target: ") == 0) {
        // dynamic "Target: 50" -> "目标: 50"
        std::string numberPart = text.substr(8);
        const unsigned char prefix[] = {0xE7,0x9B,0xAE,0xE6,0xA0,0x87,0x3A,0x20};
        std::string chineseText(reinterpret_cast<const char*>(prefix), sizeof(prefix));
        chineseText += numberPart;
        sfString = sf::String::fromUtf8(chineseText.begin(), chineseText.end());
    } else if (text == "Select Level") {
        // "选择关卡" in UTF-8
        const unsigned char chineseText[] = {0xE9,0x80,0x89,0xE6,0x8B,0xA9,0xE5,0x85,0xB3,0xE5,0x8D,0xA1,0x00};
        sfString = sf::String::fromUtf8(reinterpret_cast<const char*>(chineseText), reinterpret_cast<const char*>(chineseText + sizeof(chineseText) - 1));
    } else if (text == "Click to Continue") {
        // "点击继续" in UTF-8
        const unsigned char chineseText[] = {0xE7,0x82,0xB9,0xE5,0x87,0xBB,0xE7,0xBB,0xAD,0xE7,0xBB,0xAD,0x00};
        sfString = sf::String::fromUtf8(reinterpret_cast<const char*>(chineseText), reinterpret_cast<const char*>(chineseText + sizeof(chineseText) - 1));
    } else if (text == "Game Paused") {
        // "游戏已暂停" in UTF-8
        const unsigned char chineseText[] = {0xE6,0xB8,0xB8,0xE6,0x88,0x8F,0xE5,0xB7,0xB2,0xE5,0x81,0x9C,0xE6,0xAD,0xA2,0x00};
        sfString = sf::String::fromUtf8(reinterpret_cast<const char*>(chineseText), reinterpret_cast<const char*>(chineseText + sizeof(chineseText) - 1));
    } else if (text == "Press Space to Continue") {
        // "按空格继续" in UTF-8
        const unsigned char chineseText[] = {0xE6,0x8C,0x89,0xE7,0xA9,0xBA,0xE6,0xA0,0xBC,0xE7,0xBB,0xAD,0xE7,0xBB,0xAD,0x00};
        sfString = sf::String::fromUtf8(reinterpret_cast<const char*>(chineseText), reinterpret_cast<const char*>(chineseText + sizeof(chineseText) - 1));
    } else {
        // For other text, use explicit UTF-8 conversion
        sfString = sf::String::fromUtf8(text.begin(), text.end());
    }
    
    sf::Text sfText(sfString, font, fontSize);
    sfText.setFillColor(color);
    sfText.setPosition(x, y);
    
    // Draw background box for better readability
    sf::FloatRect textBounds = sfText.getLocalBounds();
    sf::RectangleShape bg(sf::Vector2f(textBounds.width + 10, textBounds.height + 10));
    bg.setFillColor(sf::Color(0, 0, 0, 150));
    bg.setPosition(x - 5, y - 5);
    window.draw(bg);
    
    // Draw text
    window.draw(sfText);
}