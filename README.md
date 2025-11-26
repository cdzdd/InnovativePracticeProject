# EatFish - C++ 版本

这是原Java项目的C++重写版本，使用SFML库进行图形渲染。

## 环境要求

- C++17 或更高版本
- CMake 3.15 或更高版本
- SFML 2.5 或更高版本

## 安装依赖

### Windows (使用vcpkg)
```bash
vcpkg install sfml:x64-windows
```

### Ubuntu/Debian
```bash
sudo apt-get install libsfml-dev
```

### macOS
```bash
brew install sfml
```

## 编译说明

1. 克隆或打开项目文件夹
2. 创建构建目录：
```bash
mkdir build
cd build
```

3. 使用CMake配置：
```bash
cmake ..
```

4. 编译项目：
```bash
cmake --build .
```

5. 运行游戏：
```bash
./EatFish  # Linux/macOS
EatFish.exe  # Windows
```

## 资源文件设置

游戏需要以下文件夹结构（与C++项目同级或在构建输出目录）：

```
assets/
├── sea.jpg
├── enemyFish/
│   ├── fish1_r.gif
│   ├── fish1_l.gif
│   ├── fish2_r.png
│   ├── fish2_l.png
│   ├── fish3_r.png
│   ├── fish3_l.png
│   └── boss.gif
├── myFish/
│   ├── myfish_left.gif
│   └── myfish_right.gif
└── powerup/
    ├── icon_powerup_speed.jpg
    ├── icon_powerup_shield.png
    └── icon_powerup_xp.jpg
```

从原Java项目的`images/`文件夹复制所有图片到`assets/`文件夹中。

## 游戏操作

- **W/A/S/D** - 控制鱼的移动
- **鼠标左键** - 开始游戏/重新开始
- **空格键** - 暂停/继续

## 游戏特性

- 多个敌鱼等级（1级、2级、3级）
- BOSS敌人
- 三种道具效果（加速、护盾、XP）
- 进度等级系统
- 碰撞检测和游戏状态管理

## 代码结构

- `include/` - 头文件
  - `GameWindow.h` - 主游戏窗口类
  - `MyFish.h` - 玩家控制的鱼
  - `Enemy.h` - 敌鱼类
  - `PowerUp.h` - 道具类
  - `Background.h` - 背景管理
  - `GameUtils.h` - 游戏工具和全局状态

- `src/` - 实现文件

## 主要类说明

### GameWindow
处理游戏的主循环、输入事件、游戏逻辑和渲染。

### MyFish
管理玩家控制的鱼，包括移动、碰撞检测和绘制。

### Enemy
基础敌鱼类，有多个派生类：
- Enemy1L/Enemy1R - 1级敌鱼
- Enemy2L/Enemy2R - 2级敌鱼
- Enemy3L/Enemy3R - 3级敌鱼
- EnemyBoss - BOSS敌人

### PowerUp
管理游戏中的道具（速度提升、护盾、XP）。

### GameUtils
存储游戏的全局状态和工具函数。

## 与Java版本的区别

- 使用SFML代替Swing进行图形渲染
- 使用C++内存管理（智能指针）
- 性能更高（原生编译）
- 跨平台支持（Windows/Linux/macOS）
