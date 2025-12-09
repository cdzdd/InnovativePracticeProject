#include "Enemy.h"
#include "GameUtils.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

Enemy::Enemy() : x(0), y(0), width(0), height(0), speed(0), dir(1), type(0), count(0) {}

void Enemy::paintSelf(sf::RenderWindow& window) {
    sprite.setPosition(sf::Vector2f(x, y));
    window.draw(sprite);

    // Debug: when enabled, draw bounding box and print sprite info
    if (GameUtils::debugVisual) {
        sf::FloatRect b = sprite.getGlobalBounds();
        sf::RectangleShape outline(sf::Vector2f(b.width, b.height));
        outline.setPosition(b.left, b.top);
        outline.setFillColor(sf::Color::Transparent);
        outline.setOutlineColor(sf::Color::Red);
        outline.setOutlineThickness(2.f);
        window.draw(outline);
        std::cout << "Enemy paint type=" << type << " pos=" << x << "," << y
                  << " spriteBounds=" << b.left << "," << b.top << "," << b.width << "x" << b.height << std::endl;
    }
}

sf::FloatRect Enemy::getRect() const {
    return sf::FloatRect(x, y, width, height);
}

// Enemy1 heading left
Enemy1L::Enemy1L() {
    x = -45;
    y = rand() % 700 + 100;
    width = 45;
    height = 69;
    speed = 10;
    count = 1;
    type = 1;
    dir = 1;
    sprite.setTexture(GameUtils::enamy1Texture);
    sprite.setPosition(sf::Vector2f(x, y));
    sprite.setScale(width / sprite.getLocalBounds().width,
                    height / sprite.getLocalBounds().height);
}

// Enemy1 heading right
Enemy1R::Enemy1R()  {
    x = 1400;
    y = rand() % 700 + 100;
    width = 45;
    height = 69;
    speed = 10;
    count = 1;
    type = 1;
    dir = -1;
    sprite.setTexture(GameUtils::enamyr1Texture);
    sprite.setPosition(sf::Vector2f(x, y));
    sprite.setScale(width / sprite.getLocalBounds().width,
                    height / sprite.getLocalBounds().height);
}

// Enemy2 heading left
Enemy2L::Enemy2L() {
    x = -100;
    y = rand() % 700 + 100;
    width = 100;
    height = 100;
    speed = 5;
    count = 2;
    type = 2;
    dir = 1;
    sprite.setTexture(GameUtils::enamy2Texture);
    sprite.setPosition(sf::Vector2f(x, y));
    sprite.setScale(width / sprite.getLocalBounds().width,
                    height / sprite.getLocalBounds().height);
}

// Enemy2 heading right
Enemy2R::Enemy2R()  {
    x = 1400;
    y = rand() % 700 + 100;
    width = 100;
    height = 100;
    speed = 5;
    count = 2;
    type = 2;
    dir = -1;
    sprite.setTexture(GameUtils::enamyr2Texture);
    sprite.setPosition(sf::Vector2f(x, y));
    sprite.setScale(width / sprite.getLocalBounds().width,
                    height / sprite.getLocalBounds().height);
}

// Enemy3 heading left
Enemy3L::Enemy3L() {
    x = -300;
    y = rand() % 700 + 100;
    width = 300;
    height = 150;
    speed = 20;
    count = 3;
    type = 3;
    dir = 1;
    sprite.setTexture(GameUtils::enamy3Texture);
    sprite.setPosition(sf::Vector2f(x, y));
    sprite.setScale(width / sprite.getLocalBounds().width,
                    height / sprite.getLocalBounds().height);
}

sf::FloatRect Enemy3L::getRect() const {
    return sf::FloatRect(x + 40, y + 30, width - 80, height - 60);
}

// Enemy3 heading right
Enemy3R::Enemy3R()  {
    x = 1400;
    y = rand() % 700 + 100;
    width = 300;
    height = 150;
    speed = 20;
    count = 3;
    type = 3;
    dir = -1;
    sprite.setTexture(GameUtils::enamyr3Texture);
    sprite.setPosition(sf::Vector2f(x, y));
    sprite.setScale(width / sprite.getLocalBounds().width,
                    height / sprite.getLocalBounds().height);
}

sf::FloatRect Enemy3R::getRect() const {
    return sf::FloatRect(x + 40, y + 30, width - 80, height - 60);
}

// Boss enemy
EnemyBoss::EnemyBoss() {
    x = -1000;
    y = rand() % 700 + 100;
    width = 200;
    height = 200;
    speed = 80;
    count = 0;
    type = 10;
    dir = 1;
    sprite.setTexture(GameUtils::bossTexture);
    sprite.setPosition(sf::Vector2f(x, y));
    sprite.setScale(width / sprite.getLocalBounds().width,
                    height / sprite.getLocalBounds().height);
}
