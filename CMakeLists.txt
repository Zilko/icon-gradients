cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if ("${CMAKE_SYSTEM_NAME}" STREQUAL "iOS" OR IOS)
    set(CMAKE_OSX_ARCHITECTURES "arm64")
else()
    set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
endif()
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

project(IconGradients VERSION 1.0.0)

add_library(${PROJECT_NAME} SHARED 
    src/Utils/Utils.cpp
    src/Utils/Cache.cpp
    src/UI/ColorNode.cpp    
    src/UI/ColorPicker.cpp    
    src/UI/ColorToggle.cpp    
    src/UI/GradientLayer.cpp    
    src/UI/IconButton.cpp    
    src/UI/LoadLayer.cpp    
    src/UI/PointsLayer.cpp
    src/Hooks/CharacterColorPage.cpp
    src/Hooks/CommentCell.cpp
    src/Hooks/GJGarageLayer.cpp
    src/Hooks/GJLevelScoreCell.cpp
    src/Hooks/ItemInfoPopup.cpp
    src/Hooks/MenuGameLayer.cpp
    src/Hooks/MenuLayer.cpp
    src/Hooks/PlayerObject.cpp
    src/Hooks/ProfilePage.cpp
    src/Hooks/SimplePlayer.cpp
)

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

setup_geode_mod(${PROJECT_NAME})
