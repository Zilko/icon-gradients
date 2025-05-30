#pragma once

#include "../Includes.hpp"

#include <Geode/modify/ProfilePage.hpp>

class $modify(ProProfilePage, ProfilePage) {

	struct Fields {
		bool m_isShip = true;
		bool m_isSecondPlayer = false;

		SEL_MenuHandler m_originalCallback = nullptr;
	};

	static void onModify(auto& self) {
        (void)self.setHookPriorityAfterPost("ProfilePage::getUserInfoFinished", "alphalaneous.fine_outline");
	}

	void onSwap(CCObject*);

	void updateGradient();

	void getUserInfoFinished(GJUserScore*);

	void toggleShip(CCObject*);

};