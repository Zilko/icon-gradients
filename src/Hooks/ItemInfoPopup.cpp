#include "../Utils/Utils.hpp"

#include "ItemInfoPopup.hpp"

bool ProItemInfoPopup::init(int p0, UnlockType p1) {
    if (!ItemInfoPopup::init(p0, p1)) return false;

    if (!Loader::get()->isModLoaded("rynat.better_unlock_info")) return true;

    if (GJItemIcon* item = m_mainLayer->getChildByType<GJItemIcon>(0))
        if (SimplePlayer* icon = item->getChildByType<SimplePlayer>(0)) {
            IconType type = Utils::getIconType(icon);

            Gradient gradient = Utils::getGradient(type, false);

            Utils::applyGradient(icon, gradient.main, ColorType::Main, true);
            Utils::applyGradient(icon, gradient.secondary, ColorType::Secondary, true);
            Utils::applyGradient(icon, gradient.glow, ColorType::Glow, true);
        }

    return true;
}