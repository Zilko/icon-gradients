#include "../Utils/Utils.hpp"
#include "ccTypes.h"

#include "IconButton.hpp"

#include <hiimjustin000.more_icons/include/MoreIcons.hpp>

IconButton* IconButton::create(CCObject* target, SEL_MenuHandler callback, IconType type, bool secondPlayer) {
    IconButton* ret = new IconButton();

    ret->m_type = type;
    ret->m_isSecondPlayer = secondPlayer;

    if (ret->init(target, callback)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
};

bool IconButton::init(CCObject* target, SEL_MenuHandler callback) {
    m_icon = Utils::createIcon(m_type, m_isSecondPlayer);

    if (Loader::get()->isModLoaded("hiimjustin000.more_icons"))
        MoreIcons::updateSimplePlayer(m_icon, m_type, m_isSecondPlayer);

    CCSize buttonSize = m_icon->getChildByType<CCSprite>(0)->getContentSize() * 0.63f;
    buttonSize.width = std::min(buttonSize.width, 21.f);
    buttonSize.height = std::min(buttonSize.height, 21.f);

    updatePlayerScale();

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

void IconButton::setColor(ColorType colorType, bool white, bool secondPlayer) {
    Utils::setIconColors(m_icon, colorType, white, secondPlayer);
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

        Utils::applyGradient(m_dot->getSprite(), m_currentConfig, m_type, 1, true, false, false, 121);

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
        Utils::applyGradient(m_icon, gradient, false, false, 121);
    } else
        Utils::applyGradient(m_icon, m_currentConfig, colorType, false, false, secondPlayer);

    ccColor3B color = Utils::getPlayerColor(colorType, secondPlayer);

    m_dot->setColor(m_currentConfig.points.empty()
        ? color
        : ccc3(255, 255, 255));

    m_secondDot->setColor(m_currentConfig.points.empty()
        ? color
        : ccc3(255, 255, 255));

    if (!transition || !isLocked() || previousConfig == m_currentConfig)
       return Utils::applyGradient(m_dot->getSprite(), m_currentConfig, m_type, static_cast<int>(-1), false, false, false, 123);

    Utils::applyGradient(m_dot->getSprite(), m_currentConfig, m_type, static_cast<int>(-1), true, false, false, 123);
    Utils::applyGradient(m_secondDot->getSprite(), previousConfig, m_type, static_cast<int>(-1), true, false, false, 124);

    m_secondDot->setHidden(false, 0.f);
    m_secondDot->setHidden(true, 0.1f);

    m_didForce = force;
    
    runAction(CCSequence::create(
        CCDelayTime::create(0.1f),
        CCCallFunc::create(this, callfunc_selector(IconButton::onAnimationEnded)),
        nullptr
    ));
}

void IconButton::updateSprite(bool secondPlayer) {
    m_isSecondPlayer = secondPlayer;
    
    m_icon->updatePlayerFrame(Utils::getIconID(m_type, secondPlayer), m_type);

    if (Loader::get()->isModLoaded("hiimjustin000.more_icons"))
        MoreIcons::updateSimplePlayer(m_icon, m_type, m_isSecondPlayer);
    
    updatePlayerScale();
}

void IconButton::updatePlayerScale() {
    if (m_type == IconType::Robot || m_type == IconType::Spider)
        m_icon->setScale(0.474f);
    else {
        CCSize size = m_icon->m_firstLayer->getContentSize();
        float max = size.width > size.height ? size.width : size.height;

        m_icon->setScale(max > 19.21f ? 19.21f / max : 0.63f);
    }
}

void IconButton::onAnimationEnded() {
    Utils::applyGradient(m_dot->getSprite(), m_currentConfig, m_type, static_cast<int>(-1), false, false, false, 123);

    m_dot->setHidden(!m_isLocked, 0.f, true);
    m_secondDot->setHidden(true, 0.f, true);
}