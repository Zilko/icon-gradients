#include "../Utils/Utils.hpp"

#include "IconButton.hpp"

#include <hiimjustin000.more_icons/include/MoreIcons.hpp>

IconButton* IconButton::create(CCObject* target, cocos2d::SEL_MenuHandler callback, IconType type) {
    IconButton* ret = new IconButton();

    ret->m_type = type;

    if (ret->init(target, callback)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
};

bool IconButton::init(CCObject* target, cocos2d::SEL_MenuHandler callback) {
    m_icon = Utils::createIcon(m_type);

    if (Loader::get()->isModLoaded("hiimjustin000.more_icons"))
        MoreIcons::updateSimplePlayer(m_icon, m_type);

    cocos2d::CCSize buttonSize = m_icon->getChildByType<CCSprite>(0)->getContentSize() * 0.63f;

    if (m_type == IconType::Robot || m_type == IconType::Spider)
        m_icon->setScale(0.474f);
    else {
        cocos2d::CCSize size = m_icon->m_firstLayer->getContentSize();
        float max = size.width > size.height ? size.width : size.height;

        m_icon->setScale(max > 19.21f ? 19.21f / max : 0.63f);
    }

    if (m_type == IconType::Wave)
        m_icon->setScale(m_icon->getScale() - 0.1f);

    m_icon->setPosition(buttonSize / 2.f);

    CCNode* container = CCNode::create();
    container->setContentSize(buttonSize);
    container->addChild(m_icon);

    m_dot = ColorNode::create(false);
    m_dot->setScale(0.3f);
    m_dot->setPosition(buttonSize / 2.f + ccp(8.818f, -8));
    
    container->addChild(m_dot);

    m_secondDot = ColorNode::create(false);
    m_secondDot->setScale(0.3f);
    m_secondDot->setPosition(buttonSize / 2.f + ccp(8.818f, -8));
    m_secondDot->setHidden(true, 0.f);

    container->addChild(m_secondDot);

    m_select = CCSprite::createWithSpriteFrameName("GJ_select_001.png");
    m_select->setScale(0.7f);
    m_select->setPosition(buttonSize / 2.f);
    m_select->setVisible(false);
    
    container->addChild(m_select);

    return CCMenuItemSpriteExtra::init(container, nullptr, target, callback);
}

IconType IconButton::getType() {
    return m_type;
}

bool IconButton::isLocked() {
    return m_isLocked;
}

void IconButton::setSelected(bool selected) {
    m_select->setVisible(selected);
}

void IconButton::setColor(ColorType colorType, bool white) {
    Utils::setIconColors(m_icon, colorType, white);
}

void IconButton::setLocked(bool locked, bool instant) {
    m_isLocked = locked;
    
    if (!instant)
        m_dot->flash(0.3f);

    if (locked)
        m_dot->setHidden(false, 0.f, true);
    else {
        m_didForce = true;

        float time = instant ? 0.f : 0.1f;

        Utils::applyGradient(m_dot->getSprite(), m_currentConfig, true, true);

        m_dot->setHidden(true, time);

        runAction(CCSequence::create(
            CCDelayTime::create(time),
            CCCallFunc::create(this, callfunc_selector(IconButton::onAnimationEnded)),
            nullptr 
        ));
    }
}

void IconButton::applyGradient(bool force, ColorType colorType, bool transition, bool all, bool secondPlayer) {
    GameManager* gm = GameManager::get();

    GradientConfig previousConfig = m_currentConfig;

    // transition = false; // ignore

    m_currentConfig = Utils::getSavedConfig(m_type, colorType, secondPlayer);

    if (all) {
        Gradient gradient = Utils::getGradient(m_type, secondPlayer);

        Utils::applyGradient(m_icon, gradient.main, ColorType::Main, force);
        Utils::applyGradient(m_icon, gradient.secondary, ColorType::Secondary, force);
        Utils::applyGradient(m_icon, gradient.glow, ColorType::Glow, force);
    } else
        Utils::applyGradient(m_icon, m_currentConfig, colorType, force);

    int playerColor;
    switch (colorType) {
        case ColorType::Main:
            playerColor = gm->getPlayerColor();
            break;
        case ColorType::Secondary:
            playerColor = gm->getPlayerColor2();
            break;
        case ColorType::Glow:
            playerColor = gm->getPlayerGlowColor();
            break;
        break;
    }

    m_dot->setColor(m_currentConfig.points.empty()
        ? gm->colorForIdx(playerColor)
        : ccc3(255, 255, 255));

    m_secondDot->setColor(m_currentConfig.points.empty()
        ? gm->colorForIdx(playerColor)
        : ccc3(255, 255, 255));

    if (!transition || !isLocked() || previousConfig == m_currentConfig)
       return Utils::applyGradient(m_dot->getSprite(), m_currentConfig, force);

    Utils::applyGradient(m_dot->getSprite(), m_currentConfig, force, true);
    Utils::applyGradient(m_secondDot->getSprite(), previousConfig, force, true);

    m_secondDot->setHidden(false, 0.f);
    m_secondDot->setHidden(true, 0.1f);

    m_didForce = force;
    
    runAction(CCSequence::create(
        CCDelayTime::create(0.1f),
        CCCallFunc::create(this, callfunc_selector(IconButton::onAnimationEnded)),
        nullptr
    ));
}

void IconButton::onAnimationEnded() {
    Utils::applyGradient(m_dot->getSprite(), m_currentConfig, m_didForce);

    m_dot->setHidden(!m_isLocked, 0.f, true);
    m_secondDot->setHidden(true, 0.f, true);
}