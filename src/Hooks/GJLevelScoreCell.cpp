#include "../Utils/Utils.hpp"

#include "GJLevelScoreCell.hpp"

void ProGJLevelScoreCell::loadFromScore(GJUserScore* score) {
    GJLevelScoreCell::loadFromScore(score);

    if (Utils::isSettingEnabled(MOD_DISABLED) || score->m_accountID != GJAccountManager::get()->m_accountID) return;

    if (SimplePlayer* icon = m_mainLayer->getChildByType<SimplePlayer>(0)) {
        IconType type = Utils::getIconType(icon);

        Gradient gradient = Utils::getGradient(type, false);

        Utils::applyGradient(icon, gradient.main, ColorType::Main, true);
        Utils::applyGradient(icon, gradient.secondary, ColorType::Secondary, true);
        Utils::applyGradient(icon, gradient.glow, ColorType::Glow, true);
    }
}