#pragma once

#include "Includes.hpp"

#include <Geode/modify/GJBaseGameLayer.hpp>

class $modify(ProGJBaseGameLayer, GJBaseGameLayer) {
  
    struct Fields {
        bool isExitingDual = false;
        SimplePlayer* dualSimplePlayer = nullptr;
    };

    void playExitDualEffect(PlayerObject*);
    
};