#ifndef GAMEUTILS_H
#define GAMEUTILS_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

class Enemy;
class PowerUp;

class GameUtils {
public:
    // Directions
    static bool UP;
    static bool DOWN;
    static bool LEFT;
    static bool RIGHT;

    // Score
    static int count;

    // Level
    static int level;

    // Enemy list
    static std::vector<std::shared_ptr<Enemy>> enemyList;

    // PowerUp list
    static std::vector<std::shared_ptr<PowerUp>> powerUpList;

    // Textures
    static sf::Texture bgTexture;
    static sf::Texture enamy1Texture;
    static sf::Texture enamyr1Texture;
    static sf::Texture enamy2Texture;
    static sf::Texture enamyr2Texture;
    static sf::Texture enamy3Texture;
    static sf::Texture enamyr3Texture;
    static sf::Texture bossTexture;
    static sf::Texture myFishLeftTexture;
    static sf::Texture myFishRightTexture;
    static sf::Texture powerupSpeedTexture;
    static sf::Texture powerupShieldTexture;
    static sf::Texture powerupXpTexture;

    // Power-up states
    static bool hasShield;
    static bool hasSpeedBoost;
    static long long shieldEndTime;
    static long long speedBoostEndTime;

    // Duration constants (milliseconds)
    static const long long SHIELD_DURATION = 5000;
    static const long long SPEED_BOOST_DURATION = 3000;

    // Load all textures
    static bool loadTextures(const std::string& assetsDir);

    // Draw text helper
    static void drawText(sf::RenderWindow& window, const std::string& text,
                        int x, int y, int fontSize, const sf::Color& color);
};

#endif // GAMEUTILS_H
