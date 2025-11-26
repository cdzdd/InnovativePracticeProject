#include "GameUtils.h"
#include <iostream>

// Static member initialization
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

bool GameUtils::loadTextures(const std::string& assetsDir) {
    bool allLoaded = true;

    // Load background
    if (!bgTexture.loadFromFile(assetsDir + "sea.jpg")) {
        std::cerr << "Failed to load sea.jpg" << std::endl;
        allLoaded = false;
    }

    // Load enemy textures
    if (!enamy1Texture.loadFromFile(assetsDir + "enemyFish/fish1_r.gif")) {
        std::cerr << "Failed to load fish1_r.gif" << std::endl;
        allLoaded = false;
    }
    if (!enamyr1Texture.loadFromFile(assetsDir + "enemyFish/fish1_l.gif")) {
        std::cerr << "Failed to load fish1_l.gif" << std::endl;
        allLoaded = false;
    }
    if (!enamy2Texture.loadFromFile(assetsDir + "enemyFish/fish2_r.png")) {
        std::cerr << "Failed to load fish2_r.png" << std::endl;
        allLoaded = false;
    }
    if (!enamyr2Texture.loadFromFile(assetsDir + "enemyFish/fish2_l.png")) {
        std::cerr << "Failed to load fish2_l.png" << std::endl;
        allLoaded = false;
    }
    if (!enamy3Texture.loadFromFile(assetsDir + "enemyFish/fish3_r.png")) {
        std::cerr << "Failed to load fish3_r.png" << std::endl;
        allLoaded = false;
    }
    if (!enamyr3Texture.loadFromFile(assetsDir + "enemyFish/fish3_l.png")) {
        std::cerr << "Failed to load fish3_l.png" << std::endl;
        allLoaded = false;
    }
    if (!bossTexture.loadFromFile(assetsDir + "enemyFish/boss.gif")) {
        std::cerr << "Failed to load boss.gif" << std::endl;
        allLoaded = false;
    }

    // Load player fish textures
    if (!myFishLeftTexture.loadFromFile(assetsDir + "myFish/myfish_left.gif")) {
        std::cerr << "Failed to load myfish_left.gif" << std::endl;
        allLoaded = false;
    }
    if (!myFishRightTexture.loadFromFile(assetsDir + "myFish/myfish_right.gif")) {
        std::cerr << "Failed to load myfish_right.gif" << std::endl;
        allLoaded = false;
    }

    // Load power-up textures
    if (!powerupSpeedTexture.loadFromFile(assetsDir + "powerup/icon_powerup_speed.jpg")) {
        std::cerr << "Failed to load icon_powerup_speed.jpg" << std::endl;
        allLoaded = false;
    }
    if (!powerupShieldTexture.loadFromFile(assetsDir + "powerup/icon_powerup_shield.png")) {
        std::cerr << "Failed to load icon_powerup_shield.png" << std::endl;
        allLoaded = false;
    }
    if (!powerupXpTexture.loadFromFile(assetsDir + "powerup/icon_powerup_xp.jpg")) {
        std::cerr << "Failed to load icon_powerup_xp.jpg" << std::endl;
        allLoaded = false;
    }

    return allLoaded;
}

void GameUtils::drawText(sf::RenderWindow& window, const std::string& text,
                         int x, int y, int fontSize, const sf::Color& color) {
    sf::Font font;
    // Note: You should load a font file and use it here
    // For now, we'll use a simple approach without custom fonts
    sf::Text sfText(text);
    sfText.setPosition(x, y);
    sfText.setFillColor(color);
    window.draw(sfText);
}
