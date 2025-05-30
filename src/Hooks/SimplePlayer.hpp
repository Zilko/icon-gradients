#pragma once

#include "../Includes.hpp"

#include <Geode/modify/SimplePlayer.hpp>

class $modify(ProSimplePlayer, SimplePlayer) {

	struct Fields {
		IconType m_type = IconType::Cube;
	};

	void updatePlayerFrame(int, IconType);

};