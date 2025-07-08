#pragma once

#include "../Includes.hpp"

class ColorToggle : public CCMenuItemSpriteExtra {

private:

    CCSprite* m_sprite = nullptr;
    CCSprite* m_secondSprite = nullptr;
    CCSprite* m_select = nullptr;

    GradientConfig m_currentConfig;

    bool m_isSecondary = false;
    bool m_didForce = false;
    bool m_isSelected = false;

    bool init(CCObject*, cocos2d::SEL_MenuHandler, bool, float);

    void onAnimationEnded();

public:

    static ColorToggle* create(CCObject*, cocos2d::SEL_MenuHandler, bool, bool = true,float = 1.f);

    void setEnabled(bool) override;
    void setSelected(bool);
    void setColor(const cocos2d::ccColor3B&, float = 0.f);
    
    bool isSelected() override;

    void applyGradient(GradientConfig, bool, bool);

};