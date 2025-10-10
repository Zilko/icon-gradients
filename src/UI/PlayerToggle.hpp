#pragma once

#include "../Includes.hpp"

class GradientLayer;

class PlayerToggle : public CCMenu {

private:

    CCMenuItemSpriteExtra* m_p1Btn = nullptr;
    CCMenuItemSpriteExtra* m_p2Btn = nullptr;
    
    CCLabelBMFont* m_p1Lbl = nullptr;
    CCLabelBMFont* m_p2Lbl = nullptr;

    CCScale9Sprite* m_select = nullptr;

    GradientLayer* m_layer;
    
    bool m_isToggled = false;
    
    bool init() override;
    
    void onP1(CCObject*);
    void onP2(CCObject*);
    
    PlayerToggle(GradientLayer* layer)
        : m_layer(layer) {}

public:

    static PlayerToggle* create(GradientLayer* layer);
    
    void toggle(bool);
    
    bool isToggled();
    
};