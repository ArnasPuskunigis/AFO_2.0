cmake_minimum_required(VERSION 3.20)
project(MyGame)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG        2.6.1
)
FetchContent_MakeAvailable(SFML)

cmakeadd_executable(MyGame
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