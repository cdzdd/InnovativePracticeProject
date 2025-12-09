#include "MyFish.h"
#include "GameUtils.h"
#include <chrono>

MyFish::MyFish() : x(700), y(500), width(50), height(50), speed(20), level(1) {
    sprite.setTexture(GameUtils::myFishRightTexture);
    sprite.setPosition(x, y);
}

void MyFish::logic() {
    checkPowerUpStatus();

    int currentSpeed = speed;
    if (GameUtils::hasSpeedBoost) {
        currentSpeed = speed * 2;
    }

    if (GameUtils::UP) {
        y -= currentSpeed;
    }
    if (GameUtils::DOWN) {
        y += currentSpeed;
    }
    if (GameUtils::LEFT) {
        x -= currentSpeed;
        sprite.setTexture(GameUtils::myFishLeftTexture);
    }
    if (GameUtils::RIGHT) {
        x += currentSpeed;
        sprite.setTexture(GameUtils::myFishRightTexture);
    }

    // Calculate size based on level (jump increase, not gradual)
    float scaleFactor = 1.0f + (level - 1) * 0.5f; // 1.0x for level 1, 1.5x for level 2, 2.0x for level 3
    float baseWidth = 50.0f;
    float baseHeight = 50.0f;
    
    // Set scale based on level only, not score
    sprite.setScale(sf::Vector2f((baseWidth * scaleFactor) / sprite.getLocalBounds().width,
                                  (baseHeight * scaleFactor) / sprite.getLocalBounds().height));
    sprite.setPosition(sf::Vector2f(x, y));
    
    // Get actual rendered size for accurate boundary checking
    sf::FloatRect bounds = sprite.getGlobalBounds();
    float actualWidth = bounds.width;
    float actualHeight = bounds.height;
    
    // Clamp position to screen bounds
    if (x < 0) x = 0;
    if (x > 1440 - actualWidth) x = 1440 - actualWidth;
    if (y < 0) y = 0;
    if (y > 900 - actualHeight) y = 900 - actualHeight;
    
    // Update position after clamping
    sprite.setPosition(sf::Vector2f(x, y));
}

void MyFish::paintSelf(sf::RenderWindow& window) {
    // paintSelf no longer calls logic(); movement/updates are driven from GameWindow::logic()
    window.draw(sprite);

    // 绘制护盾效果
    if (GameUtils::hasShield) {
        // Get actual fish bounds for accurate shield positioning
        sf::FloatRect bounds = sprite.getGlobalBounds();
        float fishCenterX = bounds.left + bounds.width / 2;
        float fishCenterY = bounds.top + bounds.height / 2;
        
        // Calculate shield radius based on fish size with padding
        float maxDimension = std::max(bounds.width, bounds.height);
        float shieldRadius = maxDimension / 2 + 20;
        
        sf::CircleShape shield(shieldRadius);
        shield.setOrigin(shieldRadius, shieldRadius); // Center the shield
        shield.setPosition(fishCenterX, fishCenterY); // Position at fish center
        shield.setFillColor(sf::Color(0, 255, 255, 100));
        window.draw(shield);
    }
}

sf::FloatRect MyFish::getRect() const {
    // Use actual rendered bounds for accurate collision detection
    // Create a temporary sprite to calculate bounds without modifying the actual one
    sf::Sprite tempSprite = sprite;
    
    // Calculate size based on level, same as in logic()
    float scaleFactor = 1.0f + (level - 1) * 0.5f;
    float baseWidth = 50.0f;
    float baseHeight = 50.0f;
    
    tempSprite.setScale(sf::Vector2f((baseWidth * scaleFactor) / sprite.getLocalBounds().width,
                                      (baseHeight * scaleFactor) / sprite.getLocalBounds().height));
    tempSprite.setPosition(sf::Vector2f(x, y));
    return tempSprite.getGlobalBounds();
}

void MyFish::checkPowerUpStatus() {
    auto now = std::chrono::system_clock::now().time_since_epoch().count() / 1000000;

    // Shield is now one-time use, no duration check needed
    if (GameUtils::hasSpeedBoost && now > GameUtils::speedBoostEndTime) {
        GameUtils::hasSpeedBoost = false;
    }
}

void MyFish::reset() {
    x = 700;
    y = 500;
    width = 50;
    height = 50;
    level = 1;
    sprite.setPosition(x, y);
}
