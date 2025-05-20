#include "extra_nodes.hpp"

class PointsLayer : public CCLayer {

private:

    SimplePlayer* m_icon = nullptr;

    CCSprite* m_shadow = nullptr;

    PointsLayerDelegate* m_delegate = nullptr;

    ColorNode* m_selectedPoint = nullptr;
    ColorNode* m_hoveredPoint = nullptr;

    std::vector<ColorNode*> m_points;
    std::unordered_map<IconType, std::vector<cocos2d::CCPoint>> m_dotedIcons;

    cocos2d::CCPoint m_moveOffset = ccp(0, 0);
    cocos2d::CCPoint m_pointOffset = ccp(0, 0);

    IconType m_type = IconType::Cube;
    GradientConfig m_currentConfig;

    bool m_isLinear = true;
    bool m_isMoving = false;
    bool m_isAnimating = false;
    bool m_ignoreColorChange = false;
    bool m_pointsHidden = false;
    bool m_isSecondaryColor = false;

    bool init(cocos2d::CCSize);

    cocos2d::CCPoint getRelativePos(ColorNode*);

    bool ccTouchBegan(CCTouch*, CCEvent*) override;
    void ccTouchMoved(CCTouch*, CCEvent*) override;
    void ccTouchEnded(CCTouch*, CCEvent*) override;
    
    void addRealPoints();
    void addPoint(const cocos2d::CCPoint&, bool = false);
    void selectPoint(ColorNode*);

    void updateCenter();

    void onAnimationEnded();

public:

    static PointsLayer* create(const cocos2d::CCSize&, PointsLayerDelegate*);

    ColorNode* getNodeForPos(cocos2d::CCPoint);
    ColorNode* getSelectedPoint();

    std::vector<SimplePoint> getPoints();

    cocos2d::CCPoint getPointOffset();

    void updateHover(const cocos2d::CCPoint&);
    void updateGradient(GradientConfig, bool, bool = false);
    void updateGradient(float);

    void setPlayerFrame(IconType);
    void setPointsHidden(bool, float);

    void selectFirst();
    void selectLast();
    void removeSelected();  
    
    void addPoint();
    void loadPoints(GradientConfig, bool = true);

    int getPointCount();

};