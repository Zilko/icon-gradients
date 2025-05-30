#pragma once

#include "../Includes.hpp"

#include <Geode/modify/CharacterColorPage.hpp>

class $modify(ProCharacterColorPage, CharacterColorPage) {

	static void onModify(auto& self) {
        (void)self.setHookPriorityAfterPost("CharacterColorPage::onPlayerColor", "alphalaneous.fine_outline");
	}

	struct Fields {
		bool m_isShip = true;
	};

	void updateGradient();

	bool init();

	void toggleShip(CCObject*);

	void onPlayerColor(CCObject*);

	void onClose(CCObject*);

	void keyBackClicked();

};