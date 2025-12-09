#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <SFML/Graphics.hpp>

class Background {
public:
    sf::Sprite sprite;

    Background();

    void paintSelf(sf::RenderWindow& window, int fishLevel, int gameState);
    
private:
    bool scaled;
};

#endif // BACKGROUND_H
