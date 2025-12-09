# Copilot instructions for EatFish (C++ SFML)

This project is a native C++ reimplementation of an existing Java game using SFML for rendering. Use this file to get immediately productive when making code changes, adding assets, or extending game behavior.

## 1) Big picture — where things fit
- Entry point: `src/main.cpp` → constructs `GameWindow` and calls `run()`.
- Game controller: `include/GameWindow.h` / `src/GameWindow.cpp` — main loop, input handling, spawn & collision logic, render phases.
- Global/shared state and resources: `include/GameUtils.h` / `src/GameUtils.cpp` — static flags (UP/DOWN/LEFT/RIGHT), global counters (`count`, `level`), resource textures and loadTextures(assetsDir).
- Entities & visuals:
  - Player: `include/MyFish.h` / `src/MyFish.cpp`
  - Enemies: `include/Enemy.h` / `src/Enemy_new.cpp`
  - Power-ups: `include/PowerUp.h` / `src/PowerUp.cpp`
  - Background: `include/Background.h` / `src/Background.cpp`

## 2) Short build / run / debug summary (use these first)
- Standard CMake flow (works cross-platform):
  - mkdir build; cd build
  - cmake ..
  - cmake --build .
  - run the binary `EatFish` or `EatFish.exe` from the build output
- Windows specifics: project includes a Visual Studio solution under `build/` and the top-level `CMakeLists.txt` currently hard-codes `SFML_DIR` to `D:/SFML-2.5.1-windows-vc15-64-bit/SFML-2.5.1` — change this before building or use vcpkg to install SFML:
  - vcpkg install sfml:x64-windows
- Important: CMake copies SFML DLLs after build and references `sfml-graphics-3.dll` (SFML v3). README mentions SFML 2.5 — confirm which SFML version is being used and adjust `CMakeLists.txt` if you install a different SFML version.

## 3) Assets (required files and locations)
- The game expects `assets/` next to the executable or project root as shown in `GameUtils::loadTextures`.
- Exact files and paths used by the loader:
  - `assets/sea.png`
  - `assets/enemyFish/fish1_r.png`, `fish1_l.png`, `fish2_r.png`, `fish2_l.png`, `fish3_r.png`, `fish3_l.png`, `boss.png`
  - `assets/myFish/myfish_left.png`, `myfish_right.png`
  - `assets/powerup/icon_powerup_speed.png`, `icon_powerup_shield.png`, `icon_powerup_xp.png`

If you add new sprites or textures, add them to `assets/`, then register and load them in `GameUtils::loadTextures` and expose a static sf::Texture in `GameUtils.h`.

## 4) Important patterns and project-specific conventions
- Global game state is stored as statics in `GameUtils` (flags, lists, textures). Many classes read/write these directly. Avoid creating duplicate global state; prefer extending `GameUtils` when appropriate.
- Entities are stored as `std::shared_ptr<T>` in `GameUtils::enemyList` / `powerUpList`. Spawn by pushing shared_ptr into these vectors and the `GameWindow` main loop will move/paint them.
- `GameWindow::generateEnemies` uses a switch-case with fall-through to spawn across levels — be careful if you rewrite this logic.
- Input is implemented as boolean flags (W/A/S/D set/unset in `GameWindow::handleInput`), not per-frame callback handlers.
- Timekeeping uses epoch-based values divided by 1 000 000 (microseconds → ms) — search for `shieldEndTime`, `speedBoostEndTime` when debugging timeouts.
- Debug helpers: `GameUtils::debugVisual` and `GameUtils::logLoads` are available (set to true in code during troubleshooting).

## 5) Common tasks with concrete pointers
- Add a new enemy type:
  1. Add static texture in `include/GameUtils.h` and load in `GameUtils::loadTextures`.
  2. Add `class NewEnemy : public Enemy` in `include/Enemy.h` and implementation in `src/Enemy_new.cpp`.
  3. Add spawn logic into `GameWindow::generateEnemies`.
- Add a UI or HUD text update: use `GameUtils::drawText(window, ... )` from `Background::paintSelf` or elsewhere.
- Troubleshoot asset load failures: run the game from the directory containing `assets/` and enable `GameUtils::logLoads = true` / `debugVisual = true` to get verbose texture load messages and scaling debug.

## 6) Known gotchas / things to check first
- Hard-coded SFML path in `CMakeLists.txt` — prefer using vcpkg or editing `SFML_DIR` for your machine.
- Mismatch between README SFML version (2.5) and CMake copying `sfml-*-3.dll` — verify SFML version before linking or copying DLLs.
- `delete_enemy.ps1` is a small Windows helper script used to delete a locked `Enemy.cpp` file (leftover tool in the repo) — not needed for normal development.
- No automated tests or CI configuration. Changes that affect game logic should be manually tested by running the compiled binary.

## 7) How an AI helper should behave (practical guidance)
- Make minimal, focused edits. For UI/UX or gameplay changes, update the handful of files above rather than sweeping global changes.
- When adding assets, ensure the filename/path matches `GameUtils::loadTextures` exactly, and add any new sf::Texture members into `GameUtils.h`.
- Prefer modifying existing spawn/collision code in `GameWindow.cpp` and `Enemy_new.cpp`. Adding new classes should follow the project's header/implementation patterns (parallel files under include/ and src/).

---
If anything above is unclear or you'd like me to expand examples (for example, a PR-ready TODO + patch that adds a new enemy or a small debug flag), tell me which area to document or implement next.