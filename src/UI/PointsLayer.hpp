#pragma once

#include "../Includes.hpp"

#include "ColorNode.hpp"

class GradientLayer;

class PointsLayer : public CCLayer {

private:

    SimplePlayer* m_icon = nullptr;

    CCSprite* m_shadow = nullptr;

    GradientLayer* m_layer = nullptr;

    ColorNode* m_selectedPoint = nullptr;
    ColorNode* m_hoveredPoint = nullptr;

    std::vector<ColorNode*> m_removingPoints;
    std::vector<ColorNode*> m_points;
    std::unordered_map<IconType, std::vector<CCPoint>> m_dotedIcons;

    CCPoint m_moveOffset = ccp(0, 0);
    CCPoint m_pointOffset = ccp(0, 0);

    IconType m_type = IconType::Cube;
    GradientConfig m_currentConfig;

    bool m_isLinear = true;
    bool m_isMoving = false;
    bool m_isAnimating = false;
    ColorType m_currentColor = ColorType::Main;
    bool m_ignoreColorChange = false;
    bool m_pointsHidden = false;

    bool init(CCSize);

    CCPoint getRelativePos(ColorNode*);

    bool ccTouchBegan(CCTouch*, CCEvent*) override;
    void ccTouchMoved(CCTouch*, CCEvent*) override;
    void ccTouchEnded(CCTouch*, CCEvent*) override;
    
    void addRealPoints();
    void addPoint(const CCPoint&, bool = false);
    void selectPoint(ColorNode*);

    void updateCenter();

    void onAnimationEnded();

public:

    static PointsLayer* create(const CCSize&, GradientLayer*);

    ColorNode* getNodeForPos(CCPoint);
    ColorNode* getSelectedPoint();
    SimplePlayer* getIcon();

    std::vector<SimplePoint> getPoints();

    IconType getType();

    void updateHover(const CCPoint&);
    void updatePointOpacity(int);
    void updatePointScale(float);
    void updateGradient(GradientConfig, ColorType, bool = false);
    void updateGradient(float);

    void setPlayerFrame(IconType);
    void setPointsHidden(bool, float);

    void selectFirst();
    void selectLast();
    void removeSelected();
    void moveSelected(const CCPoint&);
    
    void addPoint();
    void loadPoints(GradientConfig, bool = true);

    int getPointCount();

};
