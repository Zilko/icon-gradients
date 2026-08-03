#pragma once

#include "Includes.hpp"

#include <Geode/modify/CharacterColorPage.hpp>

class ProGJGarageLayer;

class $modify(ProCharacterColorPage, CharacterColorPage) {

	static void onModify(auto& self) {
        (void)self.setHookPriorityAfterPost("CharacterColorPage::onPlayerColor", "alphalaneous.fine_outline");
	}

	struct Fields {
		bool m_isShip = true;
		ProGJGarageLayer* m_garage = nullptr;
		
		~Fields();

	};

	void updateGradient();

	bool init();

	void toggleShip(CCObject*);

	void onPlayerColor(CCObject*);


};