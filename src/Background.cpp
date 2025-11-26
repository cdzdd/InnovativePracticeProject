#include "Background.h"
#include "GameUtils.h"

Background::Background() {
    sprite.setTexture(GameUtils::bgTexture);
    sprite.setPosition(0, 0);
}

void Background::paintSelf(sf::RenderWindow& window, int fishLevel, int gameState) {
    window.draw(sprite);

    switch (gameState) {
        case 0:
            GameUtils::drawText(window, "Click to Start Game", 500, 450, 80, sf::Color::Red);
            break;
        case 1:
            GameUtils::drawText(window, "Score: " + std::to_string(GameUtils::count), 200, 120, 50, sf::Color::Yellow);
            GameUtils::drawText(window, "Level: " + std::to_string(GameUtils::level), 600, 120, 50, sf::Color::Yellow);
            GameUtils::drawText(window, "Fish Level: " + std::to_string(fishLevel), 1000, 120, 50, sf::Color::Yellow);
            
            int effectY = 160;
            if (GameUtils::hasShield) {
                auto now = std::chrono::system_clock::now().time_since_epoch().count() / 1000000;
                long long remaining = (GameUtils::shieldEndTime - now) / 1000;
                if (remaining > 0) {
                    GameUtils::drawText(window, "Shield: " + std::to_string(remaining) + "s", 200, effectY, 30, sf::Color::Cyan);
                    effectY += 40;
                }
            }
            if (GameUtils::hasSpeedBoost) {
                auto now = std::chrono::system_clock::now().time_since_epoch().count() / 1000000;
                long long remaining = (GameUtils::speedBoostEndTime - now) / 1000;
                if (remaining > 0) {
                    GameUtils::drawText(window, "Speed: " + std::to_string(remaining) + "s", 200, effectY, 30, sf::Color::Yellow);
                }
            }
            break;
        case 2:
            GameUtils::drawText(window, "Game Over!", 600, 400, 80, sf::Color::Red);
            GameUtils::drawText(window, "Click to Restart", 580, 500, 40, sf::Color::Magenta);
            break;
        case 3:
            GameUtils::drawText(window, "You Win!", 600, 400, 80, sf::Color::Green);
            GameUtils::drawText(window, "Click to Restart", 580, 500, 40, sf::Color::Magenta);
            break;
        case 4:
            GameUtils::drawText(window, "Paused", 600, 400, 80, sf::Color::Blue);
            GameUtils::drawText(window, "Press Space to Continue", 600, 500, 40, sf::Color::Magenta);
            break;
        default:
            break;
    }
}
