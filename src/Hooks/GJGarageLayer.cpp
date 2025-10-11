#include "../Utils/Utils.hpp"
#include "../Utils/Cache.hpp"
#include "../UI/GradientLayer.hpp"

#include "GJGarageLayer.hpp"

void ProGJGarageLayer::onGradient(CCObject*) {
	GradientLayer::create()->show();
}

void ProGJGarageLayer::onSwap(CCObject* sender) {
	if (!m_fields->m_originalCallback) return;

	(this->*m_fields->m_originalCallback)(sender);

	updateGradient();
}

std::vector<SimplePlayer*> ProGJGarageLayer::getPageIcons() {
    std::vector<SimplePlayer*> ret;
    
    if (!Loader::get()->isModLoaded("ninkaz.colorful-icons")) return ret;
    
    if (CCMenu* menu = static_cast<CCNode*>(m_iconSelection->m_pages->firstObject())->getChildByType<CCMenu>(0))
			for (CCNode* node : CCArrayExt<CCNode*>(menu->getChildren()))
				if (GJItemIcon* item = node->getChildByType<GJItemIcon>(0))
					if (SimplePlayer* icon = item->getChildByType<SimplePlayer>(0))
    					ret.push_back(icon);
    
    return ret;
}

IconType ProGJGarageLayer::getType() {
	auto f = m_fields.self();

	if (!f->m_pageIcon) return IconType::Cube;
	
	return Utils::getIconType(f->m_pageIcon);
}

void ProGJGarageLayer::updatePageIcons() {
    bool p2 = false;
    
    if (Mod* sdiMod = Loader::get()->getLoadedMod("weebify.separate_dual_icons"))
        p2 = sdiMod->getSavedValue<bool>("2pselected");
    
    if (!p2 || !Utils::isSettingEnabled(P2_DISABLED)) {
        IconType type = getPageIcons().empty() ? IconType::Cube : Utils::getIconType(getPageIcons().front());
       	Gradient gradient = Utils::getGradient(type, p2);
				
  		for (SimplePlayer* icon : getPageIcons())
      		Utils::applyGradient(icon, gradient, false, false, 66);
    }
}

void ProGJGarageLayer::updateGradient() {
    auto f = m_fields.self();
    
	if (Utils::isSettingEnabled(MOD_DISABLED)) {
		if (!m_playerObject || f->m_isDisabled) return;
		
		f->m_isDisabled = true;
		
		Gradient emptyGradient = { {{}, true}, {{}, true}, {{}, true} };

		Utils::applyGradient(m_playerObject, emptyGradient, false, false, 0);
		
		if (Loader::get()->isModLoaded("weebify.separate_dual_icons"))
			if (SimplePlayer* icon = typeinfo_cast<SimplePlayer*>(getChildByID("player2-icon")))
    			Utils::applyGradient(icon, emptyGradient, false, false, 0);
		
		for (SimplePlayer* icon : getPageIcons())
    		Utils::applyGradient(icon, emptyGradient, false, false, 0);
		
		return;
	}
	
	if (f->m_isP2Separate != Utils::isSettingEnabled(P2_SEPARATE)) {
        f->m_isP2Separate = Utils::isSettingEnabled(P2_SEPARATE);
        
        updatePageIcons();
	}
	
   	if (Utils::isSettingEnabled(P2_DISABLED)) {
        f->m_isP2Disabled = true;
                
    	if (Mod* sdiMod = Loader::get()->getLoadedMod("weebify.separate_dual_icons")) {
            Gradient emptyGradient = { {{}, true}, {{}, true}, {{}, true} };
         
            if (Loader::get()->isModLoaded("weebify.separate_dual_icons"))
    			if (SimplePlayer* icon = typeinfo_cast<SimplePlayer*>(getChildByID("player2-icon")))
             			Utils::applyGradient(icon, emptyGradient, false, false, 0);
         
    	    if (sdiMod->getSavedValue<bool>("2pselected"))
                for (SimplePlayer* icon : getPageIcons())
              		Utils::applyGradient(icon, emptyGradient, false, false, 0);
        }
   	}
    
    if (f->m_isP2Disabled && !Utils::isSettingEnabled(P2_DISABLED)) {
	    f->m_isP2Disabled = false;
					
		if (Mod* sdiMod = Loader::get()->getLoadedMod("weebify.separate_dual_icons"))
    	    if (sdiMod->getSavedValue<bool>("2pselected")) {
               	IconType type = getPageIcons().empty() ? IconType::Cube : Utils::getIconType(getPageIcons().front());
                Gradient gradient = Utils::getGradient(type, true);
       					
          		for (SimplePlayer* icon : getPageIcons())
              		Utils::applyGradient(icon, gradient, false, false, 66);
            }
    }
	
	if (f->m_isDisabled) {
	    f->m_isDisabled = false;

		updatePageIcons();
	}
	
	Loader::get()->queueInMainThread([self = Ref(this)] {
		if (!self->m_playerObject) return;
		
		Mod* sdiMod = Loader::get()->getLoadedMod("weebify.separate_dual_icons");
		bool p2 = false;
		
		if (sdiMod)
    		p2 = sdiMod->getSavedValue<bool>("2pselected");
		
		Utils::applyGradient(self->m_playerObject, Utils::getGradient(Utils::getIconType(self->m_playerObject), false), false, false, 201);
		
		if (sdiMod && !Utils::isSettingEnabled(P2_DISABLED))
			if (SimplePlayer* icon = typeinfo_cast<SimplePlayer*>(self->getChildByID("player2-icon"))) {
				Gradient gradient = Utils::getGradient(Utils::getIconType(icon), true);
				Utils::applyGradient(icon, gradient, false, true, 202);
			}
		
        if ((!p2 || !Utils::isSettingEnabled(P2_DISABLED)) && Loader::get()->isModLoaded("ninkaz.colorful-icons")) {
            auto f = self->m_fields.self();
            
            if (f->m_pageIcon) {
                if (CCSprite* spr = f->m_pageIcon->getChildByType<CCSprite>(0))
         			if (spr->getOpacity() > 120)
                        Utils::applyGradient(f->m_pageIcon, Utils::getGradient(Utils::getIconType(f->m_pageIcon), p2), false, false, 66);
            }
        }
	});
}

bool ProGJGarageLayer::init() {
	if (!GJGarageLayer::init()) return false;
	
	m_fields->m_isP2Separate = Utils::isSettingEnabled(P2_SEPARATE);

	updateGradient();

	Loader::get()->queueInMainThread([self = Ref(this)] {
		if (CCNode* menu = self->getChildByID("shards-menu")) {
			CircleButtonSprite* spr = CircleButtonSprite::createWithSprite("buton.png"_spr, 0.87f, CircleBaseColor::Gray, CircleBaseSize::Small);
			spr->getTopNode()->setRotation(8);
			
			CCMenuItemSpriteExtra* btn = CCMenuItemSpriteExtra::create(spr, self, menu_selector(ProGJGarageLayer::onGradient));
			btn->setID("gradient-button"_spr);

			menu->addChild(btn);
			menu->updateLayout();

			if (Loader::get()->isModLoaded("weebify.separate_dual_icons"))
				if (CCNode* buttonNode = menu->getChildByID("swap-2p-button")) {
					CCMenuItemSpriteExtra* button = static_cast<CCMenuItemSpriteExtra*>(buttonNode);

					self->m_fields->m_originalCallback = button->m_pfnSelector;
					button->m_pfnSelector = menu_selector(ProGJGarageLayer::onSwap);
				}
		}
	}); 

	return true;
}

void ProGJGarageLayer::onSelect(CCObject* sender) {
	GJGarageLayer::onSelect(sender);
	updateGradient();
}

void ProGJGarageLayer::setupPage(int p0, IconType p1) {
	GJGarageLayer::setupPage(p0, p1);
	
	Loader::get()->queueInMainThread([self = Ref(this)] {
    	if (CCMenu* menu = static_cast<CCNode*>(self->m_iconSelection->m_pages->firstObject())->getChildByType<CCMenu>(0))
            for (CCNode* node : CCArrayExt<CCNode*>(menu->getChildren()))
               	if (GJItemIcon* item = node->getChildByType<GJItemIcon>(0))
              		if (SimplePlayer* icon = item->getChildByType<SimplePlayer>(0)) {
                       	self->m_fields->m_pageIcon = icon;
                        break;
                    }
                   					
		self->updateGradient();
	});
}