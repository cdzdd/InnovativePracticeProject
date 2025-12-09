cmake_minimum_required(VERSION 3.15)
project(EatFish)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加 /FS 标志以允许多个编译器实例写入同一个 PDB 文件
if(MSVC)
    string(APPEND CMAKE_CXX_FLAGS " /FS")
endif()

# 添加项目的include目录
include_directories("${CMAKE_SOURCE_DIR}/include")

# 设置SFML库的路径 - 使用 SFML 2.6.1
set(SFML_DIR "E:/SFML-2.6.1")
# 添加SFML的include目录
include_directories("${SFML_DIR}/include")
# 设置SFML库文件路径
set(SFML_LIB_DIR "${SFML_DIR}/lib")
set(SFML_BIN_DIR "${SFML_DIR}/bin")

# 添加可执行文件
add_executable(EatFish
    src/main.cpp
    src/GameWindow.cpp
    src/MyFish.cpp
    src/Enemy_new.cpp
    src/PowerUp.cpp
    src/GameUtils.cpp
    src/Background.cpp)

# 链接SFML库
foreach(target IN ITEMS EatFish)
    target_link_libraries(${target} PRIVATE "${SFML_LIB_DIR}/sfml-graphics.lib" "${SFML_LIB_DIR}/sfml-window.lib" "${SFML_LIB_DIR}/sfml-system.lib")
    # 添加库文件搜索路径
    target_link_directories(${target} PRIVATE ${SFML_LIB_DIR})
    
    # 拷贝SFML DLL文件到构建目录
    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        "${SFML_BIN_DIR}/sfml-graphics-2.dll"
        "${SFML_BIN_DIR}/sfml-window-2.dll"
        "${SFML_BIN_DIR}/sfml-system-2.dll"
        "$<TARGET_FILE_DIR:${target}>"
        COMMENT "Copying SFML DLLs for ${target}..."
    )
endforeach()
