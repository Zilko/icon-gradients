#pragma once

#include "../Includes.hpp"

#include <Geode/modify/MenuGameLayer.hpp>

class $modify(ProMenuGameLayer, MenuGameLayer) {

	struct Fields {
		PlayerObject* m_realPlayerObject = nullptr;
	};

	bool init();

	void resetPlayer();
	
	void updateGradient();

};