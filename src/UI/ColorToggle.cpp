#include "../Utils/Utils.hpp"

#include "ColorToggle.hpp"

ColorToggle* ColorToggle::create(CCObject* target, cocos2d::SEL_MenuHandler callback, bool secondary, bool number, float scale) {
    ColorToggle* ret = new ColorToggle();
    
    ret->m_isSecondary = secondary;

    if (ret->init(target, callback, number, scale)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

bool ColorToggle::init(CCObject* target, cocos2d::SEL_MenuHandler callback, bool number, float scale) {
    m_sprite = CCSprite::createWithSpriteFrameName("GJ_colorBtn_001.png");
    m_sprite->setScale(0.6f * scale);

    if (number) {
        CCLabelBMFont* lbl = CCLabelBMFont::create(m_isSecondary ? "2" : "1", "bigFont.fnt");
        lbl->setScale(0.525f);
        lbl->setPosition({24, 14});

        m_sprite->addChild(lbl);
    }

    m_secondSprite = CCSprite::createWithSpriteFrameName("GJ_colorBtn_001.png");
    m_secondSprite->setScale(0.6f * scale);
    m_secondSprite->setOpacity(0);

    if (number) {
        CCLabelBMFont* lbl = CCLabelBMFont::create(m_isSecondary ? "2" : "1", "bigFont.fnt");
        lbl->setScale(0.525f);
        lbl->setPosition({24, 14});

        m_secondSprite->addChild(lbl);
    }

    m_select = CCSprite::createWithSpriteFrameName("GJ_select_001.png");
    m_select->setScale(0.7f * scale);
    m_select->setPosition(m_sprite->getContentSize() * m_sprite->getScale() / 2.f);
    m_select->setVisible(false);

    addChild(m_secondSprite);
    addChild(m_sprite);
    addChild(m_select);

    setContentSize(m_sprite->getContentSize());
    setAnchorPoint({0, 0});

    if (!CCMenuItemSpriteExtra::init(m_sprite, nullptr, target, callback)) return false;

    m_secondSprite->setPosition(m_sprite->getPosition());
    m_secondSprite->setZOrder(m_sprite->getZOrder() + 1);

    return true;
}

void ColorToggle::setEnabled(bool enabled) {
    CCMenuItemSpriteExtra::setEnabled(enabled);
    m_sprite->setOpacity(enabled ? 255 : 85);
}

void ColorToggle::setSelected(bool selected) {
    m_isSelected = selected;
    m_select->setVisible(selected);
    setEnabled(!selected);
}

void ColorToggle::setColor(const cocos2d::ccColor3B& color, float time) {
    m_sprite->runAction(CCTintTo::create(time, color.r, color.g, color.b));
}

bool ColorToggle::isSelected() {
    return m_isSelected;
}

void ColorToggle::applyGradient(GradientConfig config, bool force, bool transition) {
    GameManager* gm = GameManager::get();

    m_currentConfig = config;
    m_didForce = force;

    m_sprite->setColor(m_currentConfig.points.empty()
        ? gm->colorForIdx(m_isSecondary ? gm->getPlayerColor2() : gm->getPlayerColor())
        : ccc3(255, 255, 255));
    
    m_sprite->setOpacity(255);
    m_secondSprite->setOpacity(0);

    m_sprite->stopAllActions();
    m_secondSprite->stopAllActions();

    Utils::applyGradient(m_sprite, m_currentConfig, m_didForce, transition);

    if (!transition)
        return Utils::applyGradient(m_secondSprite, m_currentConfig, force, transition);

    m_secondSprite->setOpacity(255);
    m_secondSprite->runAction(CCFadeTo::create(0.2f, 0));
   
    runAction(CCSequence::create(
        CCDelayTime::create(0.2f),
        CCCallFunc::create(this, callfunc_selector(ColorToggle::onAnimationEnded)),
        nullptr 
    ));
}

void ColorToggle::onAnimationEnded() {
    Utils::applyGradient(m_secondSprite, m_currentConfig, m_didForce, true);

    m_sprite->setOpacity(255);
    m_secondSprite->setOpacity(0);
}