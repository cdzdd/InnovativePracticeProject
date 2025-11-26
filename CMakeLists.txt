cmake_minimum_required(VERSION 3.15)
project(EatFish)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 找到SFML库
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# 添加可执行文件
add_executable(EatFish
    src/main.cpp
    src/GameWindow.cpp
    src/MyFish.cpp
    src/Enemy.cpp
    src/PowerUp.cpp
    src/GameUtils.cpp
    src/Background.cpp
)

# 链接SFML库
target_link_libraries(EatFish PRIVATE sfml-graphics sfml-window sfml-system)

# 设置资源文件路径
target_compile_definitions(EatFish PRIVATE ASSETS_DIR="${CMAKE_SOURCE_DIR}/assets/")

# 拷贝资源文件夹到构建目录
add_custom_command(TARGET EatFish POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/assets"
    "$<TARGET_FILE_DIR:EatFish>/assets"
    COMMENT "Copying assets..."
)
