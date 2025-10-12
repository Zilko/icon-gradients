#pragma once

#include "../Includes.hpp"

class GradientLayer;

class ColorToggle : public CCMenuItemSpriteExtra {

private:

    CCSprite* m_sprite = nullptr;
    CCSprite* m_secondSprite = nullptr;
    CCSprite* m_select = nullptr;
    CCSprite* m_darkererSprite = nullptr;
    
    CCLabelBMFont* m_numberLabel = nullptr;
    
    GradientLayer* m_layer = nullptr;

    GradientConfig m_currentConfig;

    ColorType m_colorType = ColorType::Main;
    
    bool m_didForce = false;
    bool m_isSelected = false;
    bool m_forceDisabled = false;
    bool m_shouldCache = true;

    bool init(CCObject*, SEL_MenuHandler, bool, float);

    void onAnimationEnded();

public:

    static ColorToggle* create(CCObject*, SEL_MenuHandler, ColorType, GradientLayer*, bool = true, float = 1.f, bool = true);

    CCSprite* getMainSprite();
    
    ColorType getColorType();

    void setEnabled(bool) override;
    void setSelected(bool);
    void setColor(const ccColor3B&, float = 0.f);
    
    bool isSelected() override;

    void applyGradient(GradientConfig, bool, bool);
    void setForceDisabled(bool);

};
