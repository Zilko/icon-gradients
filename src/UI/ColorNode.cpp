#include "ColorNode.hpp"

ColorNode* ColorNode::create(bool invis, int opacity) {
    ColorNode* ret = new ColorNode();

    ret->m_opacity = opacity;

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
    m_dot->setOpacity(m_opacity);

    m_circle = CCSprite::create("circle.png"_spr);
    m_circle->setPosition(m_dot->getContentSize() / 2.f);
    m_circle->setOpacity(0);

    setContentSize(m_dot->getContentSize());

    m_select = CCSprite::create("select.png"_spr);
    m_select->setScale(0.275f);
    m_select->setPosition(getContentSize() / 2.f);
    m_select->setOpacity(m_opacity);
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

    m_select->stopAllActions();
    m_select->setVisible(selected);
    m_select->setOpacity(m_isHidden ? 20 : m_opacity);
    m_select->runAction(CCRepeatForever::create(CCRotateBy::create(8, 360)));

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
        m_dot->setOpacity(hidden ? 0 : m_opacity);
        m_select->setOpacity((m_isSelected && !hidden) ? m_opacity : 0);
    } else {
        m_dot->runAction(CCFadeTo::create(time, hidden ? 0 : m_opacity));
        m_select->runAction(CCFadeTo::create(time, (m_isSelected && !hidden) ? m_opacity : 0));

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
        m_dot->runAction(CCEaseSineOut::create(CCTintTo::create(time, color.r, color.g, color.b)));

    m_color = color;

    setHovered(m_isHovered);
}

void ColorNode::setOpacity(int opacity) {
    m_opacity = opacity;

    m_dot->setOpacity(m_isHidden ? 0 : opacity);
    m_select->setOpacity(m_isHidden || !m_isSelected ? 0 : opacity);
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

    m_circle->setOpacity(std::clamp(m_opacity - 15, 0, 255));   
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