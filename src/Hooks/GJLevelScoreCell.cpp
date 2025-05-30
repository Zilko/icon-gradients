#include "../Utils/Utils.hpp"

#include "GJLevelScoreCell.hpp"

void ProGJLevelScoreCell::loadFromScore(GJUserScore* score) {
    GJLevelScoreCell::loadFromScore(score);

    if (score->m_accountID != GJAccountManager::get()->m_accountID) return;

    if (SimplePlayer* icon = m_mainLayer->getChildByType<SimplePlayer>(0)) {
        IconType type = Utils::getIconType(icon);

        Utils::applyGradient(icon, Utils::getSavedConfig(type, false), false, true);
        Utils::applyGradient(icon, Utils::getSavedConfig(type, true), true, true);
    }
}