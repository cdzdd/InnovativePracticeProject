#ifndef POWERUP_H
#define POWERUP_H

#include <SFML/Graphics.hpp>

enum class PowerUpType {
    SPEED,
    SHIELD,
    XP
};

class PowerUp {
public:
    int x;
    int y;
    int width;
    int height;
    int speed;
    PowerUpType type;

    sf::Sprite sprite;

    PowerUp(int x, int y, PowerUpType type);

    void paintSelf(sf::RenderWindow& window);
    void move();
    sf::FloatRect getRect() const;
};

#endif // POWERUP_H
