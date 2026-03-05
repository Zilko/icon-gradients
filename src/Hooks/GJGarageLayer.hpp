#pragma once

#include "../Includes.hpp"

#include <Geode/modify/GJGarageLayer.hpp>

class $modify(ProGJGarageLayer, GJGarageLayer) {

	static void onModify(auto& self) {
		(void)self.setHookPriorityBeforePre("GJGarageLayer::onSelect", "weebify.separate_dual_icons");
		(void)self.setHookPriorityBeforePre("GJGarageLayer::onSelect", "hiimjustin000.more_icons");
	}

	struct Fields {
		SEL_MenuHandler m_originalCallback = nullptr;

		Ref<SimplePlayer> m_pageIcon = nullptr;
		
		bool m_isDisabled = false;
		bool m_isP2Disabled = false;
		bool m_isP2Separate = false;
		bool m_wasEmptied = false;
	};

	bool init();

	IconType getType();
	
	void updatePageIcons();

	void onGradient(CCObject*);

	void onSwap(CCObject*);
	
	std::vector<SimplePlayer*> getPageIcons();

	void updateGradient();

	void onSelect(CCObject*);

	void setupPage(int, IconType);

};