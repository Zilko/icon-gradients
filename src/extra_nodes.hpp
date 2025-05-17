#pragma once

#include "includes.hpp"

class ColorNode : public CCNode {

private:

    CCSprite* m_dot = nullptr;
    CCSprite* m_circle = nullptr;

    cocos2d::CCPoint m_relativePos = {0, 0};

    cocos2d::ccColor3B m_color = ccc3(255, 255, 255);

    bool m_isHovered = false;

public:

    static ColorNode* create();

    bool init() override;

    void setColor(const cocos2d::ccColor3B&);

    const cocos2d::ccColor3B& getColor();

    void setSelected(bool);

    void setHovered(bool);

    void setGradient(GradientConfig);

};

class ColorToggle : public CCMenuItemSpriteExtra {

private:

    ColorChannelSprite* m_sprite = nullptr;

    CCSprite* m_select = nullptr;

    bool init(CCObject*, cocos2d::SEL_MenuHandler, bool);

public:

    static ColorToggle* create(CCObject*, cocos2d::SEL_MenuHandler, bool);

    void setSelected(bool);

    void setColor(const cocos2d::ccColor3B&);

};

class IconButton : public CCMenuItemSpriteExtra {

private:

    CCSprite* m_lock = nullptr;
    CCSprite* m_select = nullptr;

    SimplePlayer* m_icon = nullptr;

    IconType m_iconType = IconType::Cube;

    bool m_isLocked = false;

    bool init(CCObject*, cocos2d::SEL_MenuHandler);

public:

    static IconButton* create(CCObject*, cocos2d::SEL_MenuHandler, IconType);

    IconType getType();

    bool isLocked();

    void setLocked(bool);

    void setSelected(bool);

    void applyGradient(GradientConfig);

};

class ColorPicker : public CCNode {

private:

    CCControlColourPicker* m_picker = nullptr;

    bool init() override;

public:

    static ColorPicker* create();

    void setDelegate(ColorPickerDelegate*);

    const cocos2d::ccColor3B getColor();

    void setColor(const cocos2d::ccColor3B&);

    void setEnabled(bool);

};

class PointsLayer : public CCLayer {

private:

    SimplePlayer* m_centerIcon = nullptr;

    CCSprite* m_shadow = nullptr;

    PointsLayerDelegate* m_delegate = nullptr;

    ColorNode* m_selectedPoint = nullptr;
    ColorNode* m_hoveredPoint = nullptr;

    std::vector<ColorNode*> m_points;
    std::unordered_map<IconType, std::vector<cocos2d::CCPoint>> m_lockedIcons;

    cocos2d::CCPoint m_moveOffset = ccp(0, 0);

    bool m_isLinear = true;
    bool m_ignoreColorChange = false;
    bool m_isMoving = false;

    bool init(cocos2d::CCSize);

    cocos2d::CCPoint getRelativePos(ColorNode*);

    bool ccTouchBegan(CCTouch*, CCEvent*) override;
    void ccTouchMoved(CCTouch*, CCEvent*) override;
    void ccTouchEnded(CCTouch*, CCEvent*) override;
    
    void addPoint(const cocos2d::CCPoint&);
    void selectPoint(ColorNode*);

    void updateCenter();

public:

    static PointsLayer* create(const cocos2d::CCSize&, PointsLayerDelegate*);

    ColorNode* getNodeForPos(cocos2d::CCPoint);

    void updateHover(const cocos2d::CCPoint&);
    void updateGradient(GradientConfig);

    void setPlayerFrame(IconType);

    void selectFirst();
    void selectLast();
    void removeSelected();  
    
    ColorNode* getSelectedPoint();

    void addPoint();
    void loadPoints(const std::vector<SimplePoint>&);

    int getPointCount();
    std::vector<SimplePoint> getPoints();

};