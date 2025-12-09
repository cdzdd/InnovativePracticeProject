#include "Background.h"
#include "GameUtils.h"
#include <iostream>
#include <chrono>

Background::Background() {
    sprite.setTexture(GameUtils::bgTexture);
    sprite.setPosition(0, 0);
    scaled = false;
}

void Background::paintSelf(sf::RenderWindow& window, int fishLevel, int gameState) {
    // Ensure the background covers the whole window. Previously scaling only occurred
    // when debugVisual was enabled which left uncovered regions for normal runs.
    // We'll always scale the background to cover the window (cover, not fit)
    // so there are no thin black borders.
    if (!scaled) {
        sf::Vector2u texSize = GameUtils::bgTexture.getSize();
        if (texSize.x > 0 && texSize.y > 0) {
            sf::Vector2u winSize = window.getSize();
            float sx = static_cast<float>(winSize.x) / texSize.x;
            float sy = static_cast<float>(winSize.y) / texSize.y;
            // Use the larger scale so the texture covers the entire window (may crop but avoids borders)
            float s = std::max(sx, sy);
            // Add a tiny epsilon to avoid off-by-one rounding creating a thin black border
            const float eps = 0.002f;
            s += eps;
            sprite.setScale(s, s);
            // Center the background so any cropping is balanced on both sides
            // Compute offset to keep the background visually centered
            float offsetX = (static_cast<float>(winSize.x) - texSize.x * s) / 2.0f;
            float offsetY = (static_cast<float>(winSize.y) - texSize.y * s) / 2.0f;
            sprite.setPosition(offsetX, offsetY);
            scaled = true;
            std::cout << "Background scaled to: " << winSize.x << "x" << winSize.y << std::endl;
        }
    }

    window.draw(sprite);

    // Text rendering is now handled in GameWindow::render() using system fonts
    // This avoids duplicate text and the rectangle fallback rendering
    // Remove all text rendering from Background::paintSelf to prevent duplicate text
    // The game state text is now rendered in GameWindow::render() with proper system fonts
}
