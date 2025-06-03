#include "../Utils/Utils.hpp"
#include "CommentCell.hpp"

void ProCommentCell::loadFromComment(GJComment* comment) {
    CommentCell::loadFromComment(comment);

    if (Utils::isSettingEnabled(MOD_DISABLED) || m_accountComment || !comment) return;
    if (comment->m_accountID != GJAccountManager::get()->m_accountID) return;

    if (CCNode* menu = m_mainLayer->getChildByID("main-menu"))
        if (CCNode* userMenu = menu->getChildByID("user-menu"))
            if (SimplePlayer* icon = typeinfo_cast<SimplePlayer*>(userMenu->getChildByID("player-icon"))) {
                IconType type = Utils::getIconType(icon);

                Utils::applyGradient(icon, Utils::getSavedConfig(type, false), false, true);
                Utils::applyGradient(icon, Utils::getSavedConfig(type, true), true, true);
            }
}