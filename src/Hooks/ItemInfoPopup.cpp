#include "../Utils/Utils.hpp"

#include "ItemInfoPopup.hpp"

bool ProItemInfoPopup::init(int p0, UnlockType p1) {
    if (!ItemInfoPopup::init(p0, p1)) return false;

    if (!Loader::get()->isModLoaded("rynat.better_unlock_info")) return true;

    if (GJItemIcon* item = m_mainLayer->getChildByType<GJItemIcon>(0))
        if (SimplePlayer* icon = item->getChildByType<SimplePlayer>(0)) {
            IconType type = Utils::getIconType(icon);

            Utils::applyGradient(icon, Utils::getSavedConfig(type, false), false, true);
            Utils::applyGradient(icon, Utils::getSavedConfig(type, true), true, true);
        }

    return true;
}