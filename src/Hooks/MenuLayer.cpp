#include "../Utils/Utils.hpp"

#include "MenuLayer.hpp"

bool ProMenuLayer::init() {
    if (!MenuLayer::init()) return false;

    if (!Loader::get()->isModLoaded("capeling.icon_profile")) return true;

    if (CCNode* spr = getChildByIDRecursive("profile-icon"))
        if (SimplePlayer* icon = spr->getChildByType<SimplePlayer>(0)) {
            IconType type = Utils::getIconType(icon);

            Utils::applyGradient(icon, Utils::getSavedConfig(type, false), false, true);
            Utils::applyGradient(icon, Utils::getSavedConfig(type, true), true, true);
        }

    return true;
}