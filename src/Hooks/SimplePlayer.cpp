#include "../Utils/Utils.hpp"

#include "SimplePlayer.hpp"

void ProSimplePlayer::updatePlayerFrame(int p0, IconType type) {
	SimplePlayer::updatePlayerFrame(p0, type);

	m_fields->m_type = type;

	if (
		GJBaseGameLayer::get()
		|| Utils::isSettingEnabled(MOD_DISABLED)
	    || !Loader::get()->isModLoaded("ninkaz.colorful-icons")
	) {
    	return;
	}

	bool p2 = false;
	if (Mod* sdiMod = Loader::get()->getLoadedMod("weebify.separate_dual_icons"))
		if (CCDirector::sharedDirector()->getRunningScene()->getChildByType<GJGarageLayer>(0))
			p2 = sdiMod->getSavedValue<bool>("2pselected");

	Loader::get()->queueInMainThread([self = Ref(this), type, p2] {
		if (!self->getParent()) return;
		if (!typeinfo_cast<GJItemIcon*>(self->getParent())) return;

		if (CCSprite* spr = self->getChildByType<CCSprite>(0))
			if (spr->getOpacity() <= 120) return;

		Gradient gradient = Utils::getGradient(type, p2);
		Utils::applyGradient(self, gradient, false, false, 66);
	});
}
