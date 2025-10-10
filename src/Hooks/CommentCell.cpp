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

                Gradient gradient = Utils::getGradient(type, false);

                Utils::applyGradient(icon, gradient, false, false, 2);
            }
}