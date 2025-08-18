#pragma once

#include "../Includes.hpp"

#include "ColorNode.hpp"

class IconButton : public CCMenuItemSpriteExtra {

private:

    ColorNode* m_dot = nullptr;
    ColorNode* m_secondDot = nullptr;
    CCSprite* m_select = nullptr;

    SimplePlayer* m_icon = nullptr;

    IconType m_type = IconType::Cube;

    GradientConfig m_currentConfig;

    bool m_isLocked = false;
    bool m_didForce = false;

    bool init(CCObject*, cocos2d::SEL_MenuHandler);

    void onAnimationEnded();

public:

    static IconButton* create(CCObject*, cocos2d::SEL_MenuHandler, IconType);

    void setLocked(bool, bool = false);
    void setSelected(bool);
    void setColor(ColorType, bool);

    void applyGradient(bool, ColorType, bool = false, bool = false, bool = false);

    IconType getType();

    bool isLocked();

};
