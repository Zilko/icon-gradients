#pragma once

#include "includes.hpp"

class ColorNode : public CCNode {

private:

    CCSprite* m_dot = nullptr;
    CCSprite* m_circle = nullptr;
    CCSprite* m_select = nullptr;

    cocos2d::CCPoint m_relativePos = {0, 0};

    cocos2d::ccColor3B m_color = ccc3(255, 255, 255);

    bool m_isHovered = false;
    bool m_isSelected = false;
    bool m_isHidden = false;

    bool init(bool);

public:

    static ColorNode* create(bool);

    void setColor(const cocos2d::ccColor3B&, float = 0.f);
    void setSelected(bool);
    void setHovered(bool);
    void setHidden(bool, float, bool = false);

    cocos2d::ccColor3B getColor();
    CCSprite* getSprite();

    bool isSelected();
    bool isHidden();

    void flash(float = 0.3f);

};

class ColorToggle : public CCMenuItemSpriteExtra {

private:

    CCSprite* m_sprite = nullptr;
    CCSprite* m_secondSprite = nullptr;
    CCSprite* m_select = nullptr;

    GradientConfig m_currentConfig;

    bool m_isSecondary = false;
    bool m_didForce = false;

    bool init(CCObject*, cocos2d::SEL_MenuHandler);

    void onAnimationEnded();

public:

    static ColorToggle* create(CCObject*, cocos2d::SEL_MenuHandler, bool);

    void setSelected(bool);
    void setColor(const cocos2d::ccColor3B&);

    void applyGradient(GradientConfig, bool, bool);

};

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

    void setLocked(bool);
    void setSelected(bool);
    void setColor(bool, bool);

    void applyGradient(bool, bool, bool = false);

    IconType getType();

    bool isLocked();

};

class ColorPicker : public CCNode {

private:

    CCControlColourPicker* m_picker = nullptr;

    bool init() override;

public:

    static ColorPicker* create();

    void setDelegate(ColorPickerDelegate*);
    void setColor(const cocos2d::ccColor3B&);
    void setEnabled(bool);

    const cocos2d::ccColor3B getColor();

};

