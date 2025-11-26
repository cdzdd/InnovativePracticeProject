#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <SFML/Graphics.hpp>
#include <memory>
#include "MyFish.h"
#include "Enemy.h"
#include "Background.h"

class GameWindow {
public:
    // Game states
    enum GameState {
        STATE_START = 0,
        STATE_RUNNING = 1,
        STATE_LOSE = 2,
        STATE_WIN = 3,
        STATE_PAUSE = 4,
        STATE_RESTART = 5
    };

    GameWindow();
    void run();

private:
    sf::RenderWindow window;
    int width;
    int height;
    GameState state;
    int gameTime;
    bool isBoss;

    std::unique_ptr<MyFish> myFish;
    std::unique_ptr<Background> background;
    std::shared_ptr<Enemy> boss;

    void handleInput();
    void update();
    void logic();
    void render();
    void reGame();
    void checkShieldExpiration();
    void generatePowerUp();
    void applyPowerUpEffect(const std::shared_ptr<PowerUp>& powerUp);
    void generateEnemies();
    void checkCollisions();
};

#endif // GAMEWINDOW_H
