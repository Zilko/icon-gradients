#include "extra_nodes.hpp"
#include "utils.hpp"
#include <functional>

ColorNode* ColorNode::create(bool invis) {
    ColorNode* ret = new ColorNode();

    if (ret->init(invis)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

bool ColorNode::init(bool invis) {
    setAnchorPoint({0.5f, 0.5f});
    setScale(0.8f);

    m_dot = CCSprite::create("dot.png"_spr);
    m_dot->setPosition(m_dot->getContentSize() / 2.f);

    m_circle = CCSprite::create("circle.png"_spr);
    m_circle->setPosition(m_dot->getContentSize() / 2.f);
    m_circle->setOpacity(0);

    setContentSize(m_dot->getContentSize());

    m_select = CCSprite::create("select.png"_spr);
    m_select->setScale(0.275f);
    m_select->setPosition(getContentSize() / 2.f);
    m_select->runAction(CCRepeatForever::create(CCRotateBy::create(8, 360)));

    m_select->setVisible(false);

    if (invis) {
        m_dot->setOpacity(0);   
        // m_select->setOpacity(0);
    }

    addChild(m_dot);
    addChild(m_select);
    addChild(m_circle);
    
    return true;
}

void ColorNode::setSelected(bool selected) {
    m_isSelected = selected;

    m_select->setVisible(selected);

    setZOrder(selected ? 1 : 0);
}

void ColorNode::setHovered(bool hovered) {
    m_dot->setColor(hovered ? ccc3(
        std::clamp(m_color.r - 55, 0, 255),
        std::clamp(m_color.g - 55, 0, 255),
        std::clamp(m_color.b - 55, 0, 255)
    ) : m_color);

    m_isHovered = hovered;
}

void ColorNode::setHidden(bool hidden, float time, bool useAction) {
    // m_dot->stopAllActions();
    // m_select->stopAllActions();

    m_isHidden = hidden;

    if (time <= 0.f && !useAction) {
        m_dot->setOpacity(hidden ? 0 : 255);
        m_select->setOpacity((m_isSelected && !hidden) ? 255 : 0);
    } else {
        m_dot->runAction(CCFadeTo::create(time, hidden ? 0 : 255));
        m_select->runAction(CCFadeTo::create(time, (m_isSelected && !hidden) ? 255 : 0));

        m_isAnimating = true;

        runAction(CCSequence::create(
            CCDelayTime::create(time),
            CCCallFunc::create(this, callfunc_selector(ColorNode::onAnimationEnded)),
            nullptr 
        ));
    }
}

void ColorNode::setColor(const cocos2d::ccColor3B& color, float time) {
    if (time <= 0.f)
        m_dot->setColor(color);
    else
        m_dot->runAction(CCTintTo::create(time, color.r, color.g, color.b));

    m_color = color;

    setHovered(m_isHovered);
}

cocos2d::ccColor3B ColorNode::getColor() {
    return m_color;
}

CCSprite* ColorNode::getSprite() {
    return m_dot;
}

bool ColorNode::isHidden() {
    return m_isHidden;
}

bool ColorNode::isAnimating() {
    return m_isAnimating;
}

bool ColorNode::isSelected() {
    return m_isSelected;
}

void ColorNode::flash(float time) {
    m_isAnimating = true;

    m_circle->setOpacity(240);   
    m_circle->runAction(CCFadeTo::create(time, 0));

    runAction(CCSequence::create(
        CCDelayTime::create(time),
        CCCallFunc::create(this, callfunc_selector(ColorNode::onAnimationEnded)),
        nullptr 
    ));
}

void ColorNode::onAnimationEnded() {
    m_isAnimating = false;
}

ColorToggle* ColorToggle::create(CCObject* target, cocos2d::SEL_MenuHandler callback, bool secondary) {
    ColorToggle* ret = new ColorToggle();
    
    ret->m_isSecondary = secondary;

    if (ret->init(target, callback)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

bool ColorToggle::init(CCObject* target, cocos2d::SEL_MenuHandler callback) {
    m_sprite = CCSprite::createWithSpriteFrameName("GJ_colorBtn_001.png");
    m_sprite->setScale(0.6f);

    CCLabelBMFont* lbl = CCLabelBMFont::create(m_isSecondary ? "2" : "1", "bigFont.fnt");
    lbl->setScale(0.525f);
    lbl->setPosition({24, 14});

    m_sprite->addChild(lbl);

    m_secondSprite = CCSprite::createWithSpriteFrameName("GJ_colorBtn_001.png");
    m_secondSprite->setScale(0.6f);
    m_secondSprite->setOpacity(0);

    lbl = CCLabelBMFont::create(m_isSecondary ? "2" : "1", "bigFont.fnt");
    lbl->setScale(0.525f);
    lbl->setPosition({24, 14});

    m_secondSprite->addChild(lbl);

    m_select = CCSprite::createWithSpriteFrameName("GJ_select_001.png");
    m_select->setScale(0.7f);
    m_select->setPosition(m_sprite->getContentSize() * 0.6f / 2.f);
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

void ColorToggle::setSelected(bool selected) {
    m_select->setVisible(selected);
}

void ColorToggle::setColor(const cocos2d::ccColor3B& color) {
    m_sprite->setColor(color);
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

    Utils::applyGradient(m_sprite, m_currentConfig, m_didForce, true);

    if (!transition)
        return Utils::applyGradient(m_secondSprite, m_currentConfig, force, true);

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

void IconButton::setColor(bool secondary, bool white) {
    Utils::setIconColors(m_icon, secondary, white);
}

void IconButton::setLocked(bool locked) {
    m_isLocked = locked;
    
    m_dot->flash(0.3f);

    if (locked)
        m_dot->setHidden(false, 0.f, true);
    else {
        m_didForce = true;

        Utils::applyGradient(m_dot->getSprite(), m_currentConfig, true, true);

        m_dot->setHidden(true, 0.1f);

        runAction(CCSequence::create(
            CCDelayTime::create(0.1f),
            CCCallFunc::create(this, callfunc_selector(IconButton::onAnimationEnded)),
            nullptr 
        ));
    }
}

void IconButton::applyGradient(bool force, bool secondary, bool transition) {
    GameManager* gm = GameManager::get();

    GradientConfig oldConfig = m_currentConfig;

    m_currentConfig = Utils::getSavedConfig(m_type, secondary);
    m_didForce = force;

    Utils::applyGradient(m_icon, m_currentConfig, secondary, force);

    m_dot->setColor(m_currentConfig.points.empty()
        ? gm->colorForIdx(secondary ? gm->getPlayerColor2() : gm->getPlayerColor())
        : ccc3(255, 255, 255));

    m_secondDot->setColor(m_currentConfig.points.empty()
        ? gm->colorForIdx(secondary ? gm->getPlayerColor2() : gm->getPlayerColor())
        : ccc3(255, 255, 255));

    if (!transition || !isLocked())
       return Utils::applyGradient(m_dot->getSprite(), m_currentConfig, m_didForce);

    Utils::applyGradient(m_dot->getSprite(), m_currentConfig, force, true);
    Utils::applyGradient(m_secondDot->getSprite(), oldConfig, force, true);

    m_secondDot->setHidden(false, 0.f);
    m_secondDot->setHidden(true, 0.1f);
    
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

ColorPicker* ColorPicker::create() {
    ColorPicker* ret = new ColorPicker();

    if (ret->init()) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
};

bool ColorPicker::init() {
    m_picker = CCControlColourPicker::colourPicker();

    addChild(m_picker);
    setContentSize(m_picker->getContentSize());

    return true;
}

void ColorPicker::setDelegate(ColorPickerDelegate* delegate) {
    m_picker->setDelegate(delegate);
}

void ColorPicker::setColor(const cocos2d::ccColor3B& color) {
    m_picker->setColorValue(color);
}

const cocos2d::ccColor3B ColorPicker::getColor() {
    return m_picker->m_rgb;
}

void ColorPicker::setEnabled(bool enabled) {
    m_picker->getChildByType<CCControlHuePicker>(0)->setEnabled(enabled);
    m_picker->getChildByType<CCControlSaturationBrightnessPicker>(0)->setEnabled(enabled);
    
    for (CCSprite* spr : CCArrayExt<CCSprite*>(m_picker->getChildByType<CCSpriteBatchNode>(0)->getChildren()))
        spr->setOpacity(enabled ? 255 : 100);
}