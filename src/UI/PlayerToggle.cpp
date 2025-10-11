#include "PlayerToggle.hpp"
#include "GradientLayer.hpp"

PlayerToggle* PlayerToggle::create(GradientLayer* layer) {
    PlayerToggle* ret = new PlayerToggle(layer);
    
    ret->init();
    ret->autorelease();
    
    return ret;
}

void PlayerToggle::toggle(bool toggled) {
    m_isToggled = toggled;
    
    m_select->runAction(CCEaseSineOut::create(CCMoveTo::create(0.1f, {toggled ? 12.f : -12.f, 0})));
    
    m_p1Btn->runAction(CCEaseSineOut::create(CCMoveTo::create(0.1f, {toggled ? -14.5f : -12.f, 0})));
    m_p2Btn->runAction(CCEaseSineOut::create(CCMoveTo::create(0.1f, {toggled ? 12.f : 14.5f, 0})));
    
    m_p1Lbl->runAction(CCSpawn::create(
        CCEaseSineOut::create(CCFadeTo::create(0.1f, toggled ? 90 : 161)),
        CCEaseSineOut::create(CCScaleTo::create(0.1f, toggled ? 0.256f : 0.3f)),
        nullptr
    ));
    m_p2Lbl->runAction(CCSpawn::create(
        CCEaseSineOut::create(CCFadeTo::create(0.1f, toggled ? 161 : 90)),
        CCEaseSineOut::create(CCScaleTo::create(0.1f, toggled ? 0.3f : 0.256f)),
        nullptr
    ));
    
    m_p1Btn->setEnabled(toggled);
    m_p2Btn->setEnabled(!toggled);
}

void PlayerToggle::onP1(CCObject*) {
    toggle(false);
    
    m_layer->onPlayerToggle(this);
}

void PlayerToggle::onP2(CCObject*) {
    toggle(true);
    
    m_layer->onPlayerToggle(this);
}

bool PlayerToggle::isToggled() {
    return m_isToggled;
}

bool PlayerToggle::init() {
    CCNode::init();
    
    CCMenu* menu = CCMenu::create();
    menu->setPosition({0, 0});
    
    addChild(menu, 2);
    
    CCScale9Sprite* bg = CCScale9Sprite::create("square02b_001.png");
    bg->setColor({0, 0, 0});
    bg->setOpacity(49);
    bg->setScale(0.275f);
	bg->setContentSize({204, 74});
    
    addChild(bg);
    
    m_select = CCScale9Sprite::create("square02b_001.png");
    m_select->setColor({0, 0, 0});
    m_select->setOpacity(49);
    m_select->setScale(0.2475f);
    m_select->setPositionX(-12);
	m_select->setContentSize({106.6666f, 55.555555f});
    
    addChild(m_select);
    
    CCNode* container = CCNode::create();
    container->setContentSize(ccp(26.39f, 13.74f) * 1.5f);
    
    m_p1Lbl = CCLabelBMFont::create("P1", "bigFont.fnt");
    m_p1Lbl->setScale(0.3f);
    m_p1Lbl->setOpacity(161);
    m_p1Lbl->setPosition(container->getContentSize() / 2.f);
    
    container->addChild(m_p1Lbl);
    
    m_p1Btn = CCMenuItemSpriteExtra::create(container, this, menu_selector(PlayerToggle::onP1));
    m_p1Btn->setPosition({-12, 0.5f});
    
    menu->addChild(m_p1Btn);
    
    container = CCNode::create();
    container->setContentSize(ccp(26.39f, 13.74f) * 1.5f);
    
    m_p2Lbl = CCLabelBMFont::create("P2", "bigFont.fnt");m_p1Lbl->setScale(0.3f);
    m_p2Lbl->setOpacity(100);
    m_p2Lbl->setScale(0.3f);
    m_p2Lbl->setPosition(container->getContentSize() / 2.f);
    
    container->addChild(m_p2Lbl);
    
    m_p2Btn = CCMenuItemSpriteExtra::create(container, this, menu_selector(PlayerToggle::onP2));
    m_p2Btn->setPosition({14.5f, 0.5f});
    
    menu->addChild(m_p2Btn);
    
    toggle(false);
    
    return true;
}