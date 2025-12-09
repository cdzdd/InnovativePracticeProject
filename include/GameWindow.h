#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <fstream>
#include "MyFish.h"
#include "Enemy.h"
#include "Background.h"
class PowerUp;

// Level data structure
struct Level {
    int levelNumber;          // Level number
    int difficulty;           // Difficulty level
    int requiredScore;        // Score required to pass
    int enemyTypes[3];        // Enemy type distribution
    int enemySpawnRate;       // Enemy spawn rate
    bool hasBoss;             // Whether this level has a boss
    std::string bossType;     // Boss type
    std::string levelName;    // Level name
    std::string levelDesc;    // Level description
};

class GameWindow {
public:
    // Game state
    enum GameState {
        STATE_MENU = 0,
        STATE_LEVEL_SELECT = 1,  // Level selection screen
        STATE_START = 2,
        STATE_RUNNING = 3,
        STATE_LOSE = 4,
        STATE_WIN = 5,
        STATE_PAUSE = 6,
        STATE_HELP = 7,
        STATE_SETTINGS = 8
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
    
    // Level system variables
    int currentLevel;         // Current level
    int maxUnlockedLevel;     // Maximum unlocked level
    std::vector<Level> levels; // Level list
    int levelScore;           // Current level score
    
    std::unique_ptr<MyFish> myFish;
    std::unique_ptr<Background> background;
    std::shared_ptr<Enemy> boss;

    // Level related methods
    void initLevels();        // Initialize level data
    void loadLevel(int level); // Load specified level
    void checkLevelComplete(); // Check if level is complete
    void saveProgress();       // Save game progress
    void loadProgress();       // Load game progress
    void generateLevelEnemies(); // Generate enemies based on current level
    
    // Boss dash attack related methods
    void checkBossDashTrigger(); // Check if boss dash should be triggered
    void startBossDash(); // Start boss dash attack
    void updateBossDash(); // Update boss dash logic
    void renderBossDashWarning(); // Render boss dash warning area
    
    // Original methods
    void handleInput();
    void update();
    void logic();
    void render();
    void reGame();
    void checkShieldExpiration();
    void generatePowerUp();
    void applyPowerUpEffect(const PowerUp& powerUp);
    void generateEnemies();
    void checkCollisions();
    
    // Boss dash state variables
    bool isBossDashActive;
    bool isBossDashWarning;
    int bossDashTimer;
    int bossDashY;
    int bossDashCooldown;
    int lastBossDashScore;
    sf::RectangleShape bossDashWarningArea;
};

#endif // GAMEWINDOW_H
