#include "../Utils/Utils.hpp"

#include "SimplePlayer.hpp"

void ProSimplePlayer::updatePlayerFrame(int p0, IconType type) {
	SimplePlayer::updatePlayerFrame(p0, type);

	m_fields->m_type = type;

	if (!Loader::get()->isModLoaded("ninkaz.colorful-icons") || GJBaseGameLayer::get()) return;

	retain();

	bool useP2Gradient = false;
	if (Mod* sdiMod = Loader::get()->getLoadedMod("weebify.separate_dual_icons"))
		if (CCDirector::sharedDirector()->getRunningScene()->getChildByType<GJGarageLayer>(0))
			useP2Gradient = sdiMod->getSavedValue<bool>("2pselected");

	Loader::get()->queueInMainThread([this, type, useP2Gradient] {
		if (!getParent()) return;
		if (!typeinfo_cast<GJItemIcon*>(getParent())) return;

		if (CCSprite* spr = getChildByType<CCSprite>(0))
			if (spr->getOpacity() <= 120) return;

		Gradient gradient = Utils::getGradient(type, useP2Gradient);

		Utils::applyGradient(this, gradient.main, ColorType::Main, true);
		Utils::applyGradient(this, gradient.secondary, ColorType::Secondary, true);
		Utils::applyGradient(this, gradient.glow, ColorType::Glow, true);

		autorelease();
	});
}
