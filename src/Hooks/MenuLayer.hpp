#pragma once

#include "../Includes.hpp"

#include <Geode/modify/MenuLayer.hpp>

class $modify(ProMenuLayer, MenuLayer) {

	static void onModify(auto& self) {
        (void)self.setHookPriorityAfterPost("MenuLayer::init", "capeling.icon_profile");
	}

	bool init();

};