#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>

class Enemy {
public:
    int x;
    int y;
    int width;
    int height;
    int speed;
    int dir;
    int type;
    int count;

    sf::Sprite sprite;

    Enemy();
    virtual ~Enemy() = default;

    virtual void paintSelf(sf::RenderWindow& window);
    virtual sf::FloatRect getRect() const;
};

class Enemy1L : public Enemy {
public:
    Enemy1L();
};

class Enemy1R : public Enemy {
public:
    Enemy1R();
};

class Enemy2L : public Enemy {
public:
    Enemy2L();
};

class Enemy2R : public Enemy {
public:
    Enemy2R();
};

class Enemy3L : public Enemy {
public:
    Enemy3L();
    sf::FloatRect getRect() const override;
};

class Enemy3R : public Enemy {
public:
    Enemy3R();
    sf::FloatRect getRect() const override;
};

class EnemyBoss : public Enemy {
public:
    EnemyBoss();
};

#endif // ENEMY_H
