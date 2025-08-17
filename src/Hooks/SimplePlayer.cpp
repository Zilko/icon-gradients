#include "../Utils/Utils.hpp"

#include "SimplePlayer.hpp"

void ProSimplePlayer::updatePlayerFrame(int p0, IconType type) {
	SimplePlayer::updatePlayerFrame(p0, type);

	m_fields->m_type = type;

	if (!Loader::get()->isModLoaded("ninkaz.colorful-icons") || GJBaseGameLayer::get()) return;

	retain();

	Loader::get()->queueInMainThread([this, type] {
		if (!getParent()) return;
		if (!typeinfo_cast<GJItemIcon*>(getParent())) return;

		if (CCSprite* spr = getChildByType<CCSprite>(0))
			if (spr->getOpacity() <= 120) return;

		Utils::applyGradient(this, Utils::getSavedConfig(type, false), false, true);
		Utils::applyGradient(this, Utils::getSavedConfig(type, true), true, true);

		autorelease();
	});
}