cmake_minimum_required(VERSION 3.20)
project(MyGame)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 2.6 COMPONENTS graphics audio window system network REQUIRED)

add_executable(MyGame
    main.cpp
    AudioManager.cpp
    Bullet.cpp
    Enemy.cpp
    Pickup.cpp
    Player.cpp
)
target_link_libraries(MyGame PRIVATE
    sfml-graphics
    sfml-audio
    sfml-window
    sfml-system
    sfml-network
)