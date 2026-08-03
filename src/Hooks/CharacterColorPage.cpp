#include "Utils/Utils.hpp"
#include "Utils/Cache.hpp"

#include "CharacterColorPage.hpp"
#include "GJGarageLayer.hpp"

ProCharacterColorPage::Fields::~Fields() {
    if (m_garage) {
        m_garage->updateGradient();
    }
}

void ProCharacterColorPage::updateGradient() {
    if (Utils::isSettingEnabled(MOD_DISABLED)) return;

    bool p2Selected = false;
    if (Mod* sdiMod = Loader::get()->getLoadedMod("weebify.separate_dual_icons"))
		p2Selected = sdiMod->getSavedValue<bool>("2pselected");

    Loader::get()->queueInMainThread([self = Ref(this), p2Selected] {
        CCArrayExt<SimplePlayer*> array = CCArrayExt<SimplePlayer*>(self->m_playerObjects);

        for (int i = 0; i < array.size(); i++) {
            IconType type =  static_cast<IconType>(i);
            
            if (type == IconType::Ship)
                if (!self->m_fields->m_isShip)
                    type = IconType::Jetpack;

            Gradient gradient = Utils::getGradient(type, p2Selected);
            Utils::applyGradient(array[i], gradient, false, false, 372);
        }
    });
}

bool ProCharacterColorPage::init() {
    if (!CharacterColorPage::init()) return false;

    updateGradient();

    queueInMainThread([self = Ref(this)] {
        self->m_fields->m_garage = static_cast<ProGJGarageLayer*>(self->getParent());
    });
    
    return true;
}

void ProCharacterColorPage::toggleShip(CCObject* p0) {
    CharacterColorPage::toggleShip(p0);

    m_fields->m_isShip = !m_fields->m_isShip;

    updateGradient();
}

void ProCharacterColorPage::onPlayerColor(CCObject* sender) {
    CharacterColorPage::onPlayerColor(sender);

    updateGradient();
}