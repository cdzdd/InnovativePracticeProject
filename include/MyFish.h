#ifndef MYFISH_H
#define MYFISH_H

#include <SFML/Graphics.hpp>

class MyFish {
public:
    int x;
    int y;
    int width;
    int height;
    int speed;
    int level;

    sf::Sprite sprite;

    MyFish();

    void logic();
    void paintSelf(sf::RenderWindow& window);
    sf::FloatRect getRect() const;
    void checkPowerUpStatus();
    void reset();

private:
};

#endif // MYFISH_H
