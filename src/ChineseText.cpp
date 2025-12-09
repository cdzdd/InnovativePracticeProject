#include "ChineseText.h"

// Chinese text definitions

namespace ChineseText {
    // Game info text
    const char* SCORE_PREFIX = u8"分数: ";
    const char* LEVEL_PREFIX = u8"等级: ";
    const char* WIN_REQ = u8"胜利所需: 50";
    
    // Game state text
    const char* GAME_OVER = u8"游戏结束!";
    const char* YOU_WIN = u8"你胜利了!";
    const char* CLICK_TO_RESTART = u8"点击重新开始";
    
    // Menu text
    const char* MENU_TITLE = u8"吃小鱼";
    const char* MENU_START = u8"开始游戏";
    const char* MENU_HELP = u8"帮助";
    const char* MENU_SETTINGS = u8"设置";
    
    // Help text
    const char* HELP_TITLE = u8"游戏帮助";
    const char* HELP_CONTROLS = u8"使用 W, A, S, D 控制鱼的移动";
    const char* HELP_EAT = u8"吃掉比你小的鱼来成长";
    const char* HELP_AVOID = u8"躲避比你大的鱼";
    const char* HELP_POWERUPS = u8"收集道具获得特殊能力";
    const char* HELP_WIN = u8"达到50分即可胜利";
    const char* HELP_PAUSE = u8"按空格键暂停游戏";
    const char* HELP_BACK = u8"返回菜单";
    
    // Settings text
    const char* SETTINGS_TITLE = u8"游戏设置";
    const char* SETTINGS_VOLUME = u8"音量";
    const char* SETTINGS_SOUND = u8"音效";
    const char* SETTINGS_BACK = u8"返回菜单";
}