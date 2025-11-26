#include "Enemy.h"
#include "GameUtils.h"
#include <cstdlib>
#include <ctime>

Enemy::Enemy() : x(0), y(0), width(0), height(0), speed(0), dir(1), type(0), count(0) {}

void Enemy::paintSelf(sf::RenderWindow& window) {
    sprite.setPosition(x, y);
    window.draw(sprite);
}

sf::FloatRect Enemy::getRect() const {
    return sf::FloatRect(x, y, width, height);
}

// Enemy 1 Left
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
    sprite.setPosition(x, y);
    sprite.setScale(width / sprite.getLocalBounds().width,
                    height / sprite.getLocalBounds().height);
}

// Enemy 1 Right
Enemy1R::Enemy1R() : Enemy1L() {
    x = 1400;
    dir = -1;
    sprite.setTexture(GameUtils::enamyr1Texture);
    sprite.setPosition(x, y);
}

// Enemy 2 Left
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
    sprite.setPosition(x, y);
    sprite.setScale(width / sprite.getLocalBounds().width,
                    height / sprite.getLocalBounds().height);
}

// Enemy 2 Right
Enemy2R::Enemy2R() : Enemy2L() {
    x = 1400;
    dir = -1;
    sprite.setTexture(GameUtils::enamyr2Texture);
    sprite.setPosition(x, y);
}

// Enemy 3 Left
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
    sprite.setPosition(x, y);
    sprite.setScale(width / sprite.getLocalBounds().width,
                    height / sprite.getLocalBounds().height);
}

sf::FloatRect Enemy3L::getRect() const {
    return sf::FloatRect(x + 40, y + 30, width - 80, height - 60);
}

// Enemy 3 Right
Enemy3R::Enemy3R() : Enemy3L() {
    x = 1400;
    dir = -1;
    sprite.setTexture(GameUtils::enamyr3Texture);
    sprite.setPosition(x, y);
}

sf::FloatRect Enemy3R::getRect() const {
    return sf::FloatRect(x + 40, y + 30, width - 80, height - 60);
}

// Enemy Boss
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
    sprite.setPosition(x, y);
    sprite.setScale(width / sprite.getLocalBounds().width,
                    height / sprite.getLocalBounds().height);
}
