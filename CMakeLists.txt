cmake_minimum_required(VERSION 3.20)
project(AFO_2.0)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 2.6 COMPONENTS graphics audio window system REQUIRED)

add_executable(AFO_2.0
    main.cpp
    AudioManager.cpp
    Bullet.cpp
    Enemy.cpp
    Pickup.cpp
    Player.cpp
)

target_link_libraries(AFO_2.0 PRIVATE
    sfml-graphics
    sfml-audio
    sfml-window
    sfml-system
)

cmake_minimum_required(VERSION 3.20)
project(MyGame)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML COMPONENTS Network Graphics Window Audio System CONFIG REQUIRED)

add_executable(MyGame
    main.cpp
    AudioManager.cpp
    Bullet.cpp
    Enemy.cpp
    Pickup.cpp
    Player.cpp
)
target_link_libraries(MyGame PRIVATE
    SFML::Graphics
    SFML::Audio
    SFML::Window
    SFML::System
    SFML::Network
)