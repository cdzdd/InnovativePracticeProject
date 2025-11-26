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

    // Clamp position to screen bounds
    if (x < 0) x = 0;
    if (x > 1440 - width - GameUtils::count) x = 1440 - width - GameUtils::count;
    if (y < 0) y = 0;
    if (y > 900 - height - GameUtils::count) y = 900 - height - GameUtils::count;

    sprite.setPosition(x, y);
    sprite.setScale((width + GameUtils::count) / sprite.getLocalBounds().width,
                    (height + GameUtils::count) / sprite.getLocalBounds().height);
}

void MyFish::paintSelf(sf::RenderWindow& window) {
    logic();
    window.draw(sprite);

    // Draw shield effect
    if (GameUtils::hasShield) {
        sf::CircleShape shield(width + GameUtils::count + 20);
        shield.setPosition(x - 10, y - 10);
        shield.setFillColor(sf::Color(0, 255, 255, 100));
        window.draw(shield);
    }
}

sf::FloatRect MyFish::getRect() const {
    return sf::FloatRect(x, y, width + GameUtils::count, height + GameUtils::count);
}

void MyFish::checkPowerUpStatus() {
    auto now = std::chrono::system_clock::now().time_since_epoch().count() / 1000000;

    if (GameUtils::hasShield && now > GameUtils::shieldEndTime) {
        GameUtils::hasShield = false;
    }
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
