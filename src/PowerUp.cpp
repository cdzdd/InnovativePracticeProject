#include "PowerUp.h"
#include "GameUtils.h"

PowerUp::PowerUp(int x, int y, PowerUpType type)
    : x(x), y(y), width(40), height(40), speed(2), type(type) {
    
    switch (type) {
        case PowerUpType::SPEED:
            sprite.setTexture(GameUtils::powerupSpeedTexture);
            break;
        case PowerUpType::SHIELD:
            sprite.setTexture(GameUtils::powerupShieldTexture);
            break;
        case PowerUpType::XP:
            sprite.setTexture(GameUtils::powerupXpTexture);
            break;
    }

    sprite.setPosition(sf::Vector2f(x, y));
    sprite.setScale(sf::Vector2f(width / sprite.getLocalBounds().width,
                                  height / sprite.getLocalBounds().height));
}

void PowerUp::paintSelf(sf::RenderWindow& window) {
    sprite.setPosition(sf::Vector2f(x, y));
    window.draw(sprite);
}

void PowerUp::move() {
    y += speed;
}

sf::FloatRect PowerUp::getRect() const {
    return sf::FloatRect(x, y, width, height);
}
