#ifndef CHINESETEXT_H
#define CHINESETEXT_H

// Chinese text keys for EatFish game (ASCII keys; translations handled in GameUtils::drawText)

namespace ChineseText {
    // Game info keys (ASCII)
    const char* const SCORE_PREFIX = "Score: ";
    const char* const LEVEL_PREFIX = "Level: ";
    const char* const TARGET_PREFIX = "Target: ";
    const char* const WIN_REQ = "Win Req: 50";

    // Game state keys
    const char* const GAME_OVER = "GAME OVER!";
    const char* const YOU_WIN = "YOU WIN!";
    const char* const CLICK_TO_RESTART = "Click to Restart";

    // Menu keys
    const char* const MENU_TITLE = "Eat Fish";
    const char* const MENU_START = "Start Game";
    const char* const MENU_HELP = "Help";
    const char* const MENU_SETTINGS = "Settings";

    // Help keys
    const char* const HELP_TITLE = "Game Help";
    const char* const HELP_CONTROLS = "Use W, A, S, D to control the fish";
    const char* const HELP_EAT = "Eat smaller fish to grow";
    const char* const HELP_AVOID = "Avoid larger fish";
    const char* const HELP_POWERUPS = "Collect power-ups for special abilities";
    const char* const HELP_WIN = "Reach 50 points to win";
    const char* const HELP_PAUSE = "Press Space to pause the game";
    const char* const HELP_BACK = "Back to Menu";

    // Settings keys
    const char* const SETTINGS_TITLE = "Game Settings";
    const char* const SETTINGS_VOLUME = "Volume";
    const char* const SETTINGS_SOUND = "Sound Effects";
    const char* const SETTINGS_BACK = "Back to Menu";

    // Aliases / backwards-compatible names expected by GameWindow.cpp
    const char* const GAME_TITLE = MENU_TITLE;
    const char* const START_GAME = MENU_START;
    const char* const HELP = MENU_HELP;
    const char* const SETTINGS = MENU_SETTINGS;
    const char* const SELECT_LEVEL = "Select Level";
    const char* const BACK_TO_MENU = SETTINGS_BACK;
    const char* const CLICK_TO_CONTINUE = "Click to Continue";
    const char* const GAME_PAUSED = "Game Paused";
    const char* const PRESS_SPACE_TO_CONTINUE = "Press Space to Continue";
}

#endif // CHINESETEXT_H
