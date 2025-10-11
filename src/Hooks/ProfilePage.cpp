#include "../Utils/Utils.hpp"
#include "../Utils/Cache.hpp"

#include "ProfilePage.hpp"

void ProProfilePage::onSwap(CCObject* sender) {
    (this->*m_fields->m_originalCallback)(sender);

    m_fields->m_isSecondPlayer = !m_fields->m_isSecondPlayer;

    Loader::get()->queueInMainThread([self = Ref(this)] {
        self->updateGradient();
    });
}

void ProProfilePage::updateGradient() {
    if (!m_ownProfile || Utils::isSettingEnabled(MOD_DISABLED)) return;
    
    CCNode* menu = m_mainLayer->getChildByID("player-menu");

    if (!menu) return;

    CCArrayExt<CCNode*> array = CCArrayExt<CCNode*>(menu->getChildren());

    for (int i = 0; i < array.size(); i++) {
        SimplePlayer* child = array[i]->getChildByType<SimplePlayer>(0);

        if (!child) continue;

        IconType type =  static_cast<IconType>(i);
        
        if (type == IconType::Ship)
            if (!m_fields->m_isShip)
                type = IconType::Jetpack;

        Gradient gradient = Utils::getGradient(type, m_fields->m_isSecondPlayer);
        
        Utils::applyGradient(child, gradient, false, false, 99);
    }
}

void ProProfilePage::getUserInfoFinished(GJUserScore* p0) {
    ProfilePage::getUserInfoFinished(p0);

    updateGradient();

    Loader::get()->queueInMainThread([self = Ref(this)] {
        if (Loader::get()->isModLoaded("weebify.separate_dual_icons"))
            if (CCNode* menu = self->m_mainLayer->getChildByID("left-menu")) {
                if (CCNode* toggleNode = menu->getChildByID("2p-toggler")) {
                    CCMenuItemToggler* toggle = static_cast<CCMenuItemToggler*>(toggleNode);

                    self->m_fields->m_originalCallback = toggle->m_pfnSelector;
                    toggle->m_pfnSelector = menu_selector(ProProfilePage::onSwap);
                }
            }
    });
}

void ProProfilePage::toggleShip(CCObject* p0) {
    ProfilePage::toggleShip(p0);

    m_fields->m_isShip = !m_fields->m_isShip;

    Loader::get()->queueInMainThread([this] {
        updateGradient();
    });
}