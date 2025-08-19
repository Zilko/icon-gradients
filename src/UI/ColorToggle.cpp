#include "../Utils/Utils.hpp"
#include "ccTypes.h"

#include "ColorToggle.hpp"
#include "GradientLayer.hpp"

ColorToggle* ColorToggle::create(CCObject* target, SEL_MenuHandler callback, ColorType colorType, GradientLayer* layer, bool number, float scale, bool shouldCache) {
    ColorToggle* ret = new ColorToggle();
    
    ret->m_colorType = colorType;
    ret->m_layer = layer;
    ret->m_shouldCache = shouldCache;

    if (ret->init(target, callback, number, scale)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

bool ColorToggle::init(CCObject* target, SEL_MenuHandler callback, bool number, float scale) {
    m_sprite = CCSprite::createWithSpriteFrameName("GJ_colorBtn_001.png");
    m_sprite->setScale(0.6f * scale);
    
    addChild(m_sprite);
    
    if (!number) {
        m_select = CCSprite::createWithSpriteFrameName("GJ_select_001.png");
        m_select->setScale(0.7f * scale);
        m_select->setPosition(m_sprite->getContentSize() * m_sprite->getScale() / 2.f);
        m_select->setVisible(false);
    
        addChild(m_select); // queue in mian thread, garage cache, idk
    
        setContentSize(m_sprite->getContentSize());
        setAnchorPoint({0, 0});
    
        return CCMenuItemSpriteExtra::init(m_sprite, nullptr, target, callback);
    }

    std::string numberStr;
    switch (m_colorType) {
        case ColorType::Main:
            numberStr = "1";
            break;
        case ColorType::Secondary:
            numberStr = "2";
            break;
        case ColorType::Glow:
            numberStr = "3";
            break;
        case ColorType::White:
            numberStr = "4";
            break;
    }

    m_numberLabel = CCLabelBMFont::create(numberStr.c_str(), "bigFont.fnt");
    m_numberLabel->setScale(0.315f * scale);
    m_numberLabel->setPosition({16, 8});

    addChild(m_numberLabel, 10);

    m_secondSprite = CCSprite::createWithSpriteFrameName("GJ_colorBtn_001.png");
    m_secondSprite->setScale(0.6f * scale);
    m_secondSprite->setOpacity(0);

    addChild(m_secondSprite);
    
    m_select = CCSprite::createWithSpriteFrameName("GJ_select_001.png");
    m_select->setScale(0.7f * scale);
    m_select->setPosition(m_sprite->getContentSize() * m_sprite->getScale() / 2.f);
    m_select->setVisible(false);

    addChild(m_select);

    setContentSize(m_sprite->getContentSize());
    setAnchorPoint({0, 0});

    if (!CCMenuItemSpriteExtra::init(m_sprite, nullptr, target, callback))
        return false;

    m_secondSprite->setPosition(m_sprite->getPosition());
    m_secondSprite->setZOrder(m_sprite->getZOrder() + 1);

    return true;
}

ColorType ColorToggle::getColorType() {
    return m_colorType;
}

void ColorToggle::setEnabled(bool enabled) {
    if (m_forceDisabled) return;
    
    CCMenuItemSpriteExtra::setEnabled(enabled);
    
    if (!m_secondSprite)
        m_sprite->setOpacity(enabled ? 255 : 85);
}

void ColorToggle::setSelected(bool selected) {
    m_isSelected = selected;
    m_select->setVisible(selected);
    setEnabled(!selected);
}

void ColorToggle::setColor(const ccColor3B& color, float time) {
    m_sprite->runAction(CCTintTo::create(time, color.r, color.g, color.b));
}

bool ColorToggle::isSelected() {
    return m_isSelected;
}

void ColorToggle::applyGradient(GradientConfig config, bool force, bool transition) {    
    if (config == m_currentConfig) return;
    
    if (config.points.empty())
        config.points.push_back(SimplePoint{ {0, 0}, Utils::getPlayerColor(m_colorType, m_layer->isSecondPlayer()) });
    
    ccColor3B targetColor = config.points.empty() ? Utils::getPlayerColor(m_colorType, m_layer->isSecondPlayer()) : ccc3(255, 255, 255);
    
    if (transition && !m_currentConfig.points.empty() && m_secondSprite)
        m_secondSprite->setColor({255, 255, 255});
    
    if (transition && !config.points.empty()) {
        if (m_secondSprite)
            Utils::applyGradient(m_secondSprite, m_currentConfig, static_cast<IconType>(-1), static_cast<int>(m_colorType), true, false, false, m_shouldCache ? 120 : -4732);
        
        m_sprite->setColor(targetColor);
    } else {
        m_sprite->setColor(targetColor);
        
        if (m_secondSprite)
            m_secondSprite->setColor(targetColor);
    }
        
    m_currentConfig = config;
    m_didForce = force;

    if (!transition || !m_secondSprite) {
        m_sprite->setOpacity(255);
        return Utils::applyGradient(m_sprite, m_currentConfig, static_cast<IconType>(-1), static_cast<int>(m_colorType), false, false, false, m_shouldCache ? 120 : -4732);
    }
    
    m_sprite->setOpacity(255);
    m_secondSprite->setOpacity(0);

    m_sprite->stopAllActions();
    m_secondSprite->stopAllActions();

    Utils::applyGradient(m_sprite, m_currentConfig, static_cast<IconType>(-1), static_cast<int>(m_colorType), true, false, false, m_shouldCache ? 120 : -4732);

    m_secondSprite->setOpacity(255);
    m_secondSprite->runAction(CCSequence::create(
        CCFadeTo::create(0.2f, 0),
        CCCallFunc::create(this, callfunc_selector(ColorToggle::onAnimationEnded)),
        nullptr 
    ));
}

void ColorToggle::onAnimationEnded() {
    ccColor3B targetColor = m_currentConfig.points.empty() ? Utils::getPlayerColor(m_colorType, m_layer->isSecondPlayer()) : ccColor3B{255, 255, 255};
    
    m_secondSprite->setColor(targetColor);
    
    Utils::applyGradient(m_secondSprite, m_currentConfig, static_cast<IconType>(-1), static_cast<int>(m_colorType), true, false, false, m_shouldCache ? 120 : -4732);

    m_sprite->setOpacity(255);
    m_secondSprite->setOpacity(0);
}

void ColorToggle::setForceDisabled(bool disabled) {
    setEnabled(!disabled);

    m_forceDisabled = disabled;
    
    setEnabled(!disabled);
    
    if (m_numberLabel)
        m_numberLabel->runAction(CCFadeTo::create(0.2f, disabled ? 60 : 255));
    
    if (m_darkererSprite) {
        m_darkererSprite->runAction(CCFadeTo::create(0.2f, disabled ? 170 : 0));
        return;
    }
    
    m_darkererSprite = CCSprite::createWithSpriteFrameName("GJ_colorBtn_001.png");
    m_darkererSprite->setPosition(getContentSize() / 2.f);
    m_darkererSprite->setScale(m_sprite->getScale());
    m_darkererSprite->setColor({0, 0, 0});
    m_darkererSprite->setOpacity(0);
    m_darkererSprite->runAction(CCFadeTo::create(0.2f, disabled ? 170 : 0));
    
    addChild(m_darkererSprite, 9);    
}
