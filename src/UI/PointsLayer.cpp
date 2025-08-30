#include "../Utils/Utils.hpp"

#include "PointsLayer.hpp"
#include "GradientLayer.hpp"

#include <hiimjustin000.more_icons/include/MoreIcons.hpp>

PointsLayer* PointsLayer::create(const cocos2d::CCSize& size, GradientLayer* layer) {
    PointsLayer* ret = new PointsLayer();

    ret->m_layer = layer;

    if (ret->init(size)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
};

bool PointsLayer::init(cocos2d::CCSize size) {
    if (!CCLayer::init()) return false;

    m_shadow = CCSprite::create("shadow.png"_spr);
    m_shadow->setOpacity(49);
    m_shadow->setPosition({178.5f, 170});
    m_shadow->setScaleY(0.8f);

    m_icon = Utils::createIcon(IconType::Cube);
    m_icon->setPosition({178.5f, 200});
    m_icon->setScale(1.7f);

    addChild(m_shadow);
    addChild(m_icon);

    setPlayerFrame(IconType::Cube);

    setContentSize(size);
    setAnchorPoint({0, 0});
    setTouchEnabled(true);
    registerWithTouchDispatcher();
    setTouchMode(kCCTouchesOneByOne);
    
    return true;
}

int PointsLayer::getPointCount() {
    return m_points.size(); 
}

void PointsLayer::removeSelected() {
    if (!m_selectedPoint) selectLast();
    if (!m_selectedPoint) return;

    if (!m_pointsHidden) {
        addPoint(m_selectedPoint->getPosition());

        m_removingPoints.push_back(m_points.back());
        m_removingPoints.back()->setColor(m_selectedPoint->getColor());
        m_removingPoints.back()->setHidden(true, 0.3f);

        m_points.pop_back();
    }

    std::vector<ColorNode*> newPoints;

    for (ColorNode* point : m_points)
        if (point != m_selectedPoint)
            newPoints.push_back(point);

    m_points = newPoints;

    m_selectedPoint->removeFromParentAndCleanup(true);

    m_selectedPoint = nullptr;
}

void PointsLayer::moveSelected(const cocos2d::CCPoint& move) {
    cocos2d::CCPoint pos = m_selectedPoint->getPosition() + move;
    
    pos.x = std::max(0.f, std::min(pos.x, getContentSize().width));
    pos.y = std::max(0.f, std::min(pos.y, getContentSize().height));
    
    m_selectedPoint->setPosition(pos);
    m_layer->pointMoved();
}

void PointsLayer::addPoint() {
    cocos2d::CCSize size = m_icon->getContentSize() * m_icon->getScale();
    cocos2d::CCPoint position = m_icon->getPosition();

    std::vector<cocos2d::CCPoint> corners = {
        {position.x - size.width / 2, position.y + size.height / 2},
        {position.x + size.width / 2, position.y + size.height / 2},
        {position.x - size.width / 2, position.y - size.height / 2},
        {position.x + size.width / 2, position.y - size.height / 2}
    };

    cocos2d::CCPoint pos = {0, 0};
    
    for (const cocos2d::CCPoint& corner : corners) {
        bool taken = false;

        for (ColorNode* point : m_points) {
            if (
                static_cast<int>(point->getPosition().x) == static_cast<int>(corner.x)
                && static_cast<int>(point->getPosition().y) == static_cast<int>(corner.y)
            ) {
                taken = true;
                break;
            }
        }

        if (!taken) {
            pos = corner;
            break;
        }
    }

    if (pos == ccp(0, 0))
        pos = corners[std::rand() % corners.size()];

    addPoint(pos);
}

void PointsLayer::addPoint(const cocos2d::CCPoint& pos, bool invis) {
    ColorNode* node = ColorNode::create(invis, Mod::get()->getSettingValue<int64_t>("point-opacity"));

    node->setPosition(pos);
    node->setScale(Mod::get()->getSettingValue<double>("point-scale"));

    addChild(node);

    m_points.push_back(node);
}

ColorNode* PointsLayer::getNodeForPos(cocos2d::CCPoint pos) {
    pos = convertToNodeSpace(pos);

    ColorNode* ret = nullptr;
    float closest = getContentSize().width * 2;

    for (ColorNode* point : m_points) {
        float distance = ccpDistance(pos, point->getPosition());
        if (distance < closest && distance < point->getContentSize().width * 0.5f * point->getScale()) {
            closest = distance;
            ret = point;
        }
    }

    return ret;
}

void PointsLayer::selectFirst() {
    if (m_points.empty()) return;

    selectPoint(m_points.front());
}

void PointsLayer::selectLast() {
    if (m_points.empty()) return;
    
    selectPoint(m_points.back());
}

void PointsLayer::selectPoint(ColorNode* point) {
    if (!point) return;

    point->setSelected(true);

    if (m_selectedPoint && m_selectedPoint != point)
        m_selectedPoint->setSelected(false);

    m_selectedPoint = point;

    m_layer->pointSelected(point);
}

bool PointsLayer::ccTouchBegan(CCTouch* touch, CCEvent* event) { 
    if (m_isAnimating) return false;

    cocos2d::CCPoint pos = touch->getLocation();

    for (int i = 0; i < m_removingPoints.size(); i++)
        if (!m_removingPoints[i]->isAnimating()) {
            m_removingPoints[i]->removeFromParentAndCleanup(true);
            m_removingPoints.erase(m_removingPoints.begin() + i);
        }

    if (ColorNode* point = getNodeForPos(pos)) {
        selectPoint(point);

        m_isMoving = true;
        m_moveOffset = point->getContentSize() * 0.5f * point->getScale() - point->convertToNodeSpace(pos)* point->getScale();

        if (Mod::get()->getSettingValue<bool>("hide-on-move")) {
            bool pointsHidden = m_pointsHidden;

            setPointsHidden(true, 0.3f);

            m_pointsHidden = pointsHidden;
        }

        return CCLayer::ccTouchBegan(touch, event);
    }

    return false;
}

void PointsLayer::ccTouchMoved(CCTouch* touch, CCEvent* event) {
    CCLayer::ccTouchMoved(touch, event);

    if (m_isMoving && m_selectedPoint) {
        cocos2d::CCPoint pos = convertToNodeSpace(touch->getLocation()) + m_moveOffset;

        pos.x = std::max(0.f, std::min(pos.x, getContentSize().width));
        pos.y = std::max(0.f, std::min(pos.y, getContentSize().height));

        m_selectedPoint->setPosition(pos);

        m_layer->pointMoved();
    }
}

void PointsLayer::ccTouchEnded(CCTouch* touch, CCEvent* event) {
    m_isMoving = false;
    setPointsHidden(m_pointsHidden, 0.3f);
    m_layer->pointReleased();
}

std::vector<SimplePoint> PointsLayer::getPoints() {
    std::vector<SimplePoint> ret;

    for (ColorNode* point : m_points)
        ret.push_back({
            getRelativePos(point),
            point->getColor()
        });

    return ret;
}

IconType PointsLayer::getType() {
    return m_type;
}

void PointsLayer::updateHover(const cocos2d::CCPoint& pos) {
    if (ColorNode* point = getNodeForPos(pos)) {
        point->setHovered(true);

        if (m_hoveredPoint && m_hoveredPoint != point)
            m_hoveredPoint->setHovered(false);

        m_hoveredPoint = point;
    } else if (m_hoveredPoint) {
        m_hoveredPoint->setHovered(false);
        m_hoveredPoint = nullptr;
    }
}

void PointsLayer::updatePointOpacity(int value) {
    for (ColorNode* point : m_points)
        point->setOpacity(value);
}

void PointsLayer::updatePointScale(float value) {
    for (ColorNode* point : m_points)
        point->setScale(value);
}

void PointsLayer::updateGradient(GradientConfig config, ColorType colorType, bool force) {
    Utils::applyGradient(m_icon, config, colorType, force);

    m_currentColor = colorType;

    updateCenter();
}

void PointsLayer::updateCenter() {
    m_icon->setContentSize(m_icon->m_firstLayer->getContentSize());
    m_icon->m_firstLayer->setPosition(
        m_icon->getContentSize() / 2.f
        - ccp(0, m_type == IconType::Ufo ? 8.f : 0.f)
    );

    // m_pointOffset = ccp(0.f, m_type == IconType::Ufo ? 10.f : 0.f);

    if (m_icon->m_robotSprite)
        m_icon->m_robotSprite->setPosition(m_icon->getContentSize() / 2.f);

    if (m_icon->m_spiderSprite)
        m_icon->m_spiderSprite->setPosition(m_icon->getContentSize() / 2.f);

    Utils::setIconColors(m_icon, m_currentColor, false);
}

ColorNode* PointsLayer::getSelectedPoint() {
    return m_selectedPoint;
}

void PointsLayer::setPlayerFrame(IconType type) {
    m_type = type;
    
    m_icon->updatePlayerFrame(
        Utils::getIconID(type, m_layer->isSecondPlayer()),
        type
    );

    if (Loader::get()->isModLoaded("hiimjustin000.more_icons"))
        MoreIcons::updateSimplePlayer(m_icon, m_type);

    updateCenter();
}

void PointsLayer::setPointsHidden(bool hidden, float time) {
    if (m_points.empty()) return;

    float delay = time / m_points.size();
    
    for (int i = 0; i < m_points.size(); i++)
        m_points[i]->setHidden(hidden, delay * (i + 1) + (time > 0.f ? 0.1f : 0.f));

    m_pointsHidden = hidden;
}

cocos2d::CCPoint PointsLayer::getRelativePos(ColorNode* point) {
    if (!point) return {0, 0};

    cocos2d::CCSize iconSize = ccp(30.5f, 30) * m_icon->getScale();
    cocos2d::CCPoint realPos = point->getPosition() + m_pointOffset - (m_icon->getPosition() - iconSize * m_icon->getAnchorPoint());

    return {
        realPos.x / iconSize.width,
        realPos.y / iconSize.height
    };
}

void PointsLayer::loadPoints(GradientConfig config, bool animate) {
    updateCenter();

    m_currentConfig = config;
    m_isAnimating = true;

    if (!animate || m_pointsHidden) return onAnimationEnded();

    // m_icon->runAction(CCFadeTo::create(0.05f, 0));

    cocos2d::CCSize iconSize = ccp(30.5f, 30) * m_icon->getScale();
    cocos2d::CCPoint bottomLeft = m_icon->getPosition() - iconSize / 2.f;
    
    std::unordered_set<SimplePoint> replacedPoints;
    std::unordered_set<ColorNode*> movedPoints;

    std::vector<ColorNode*> points = m_selectedPoint
        ? std::vector<ColorNode*>{ m_selectedPoint }
        : std::vector<ColorNode*>{};

    for (ColorNode* point : m_points)
        if (point != m_selectedPoint)
            points.push_back(point);

    for (int i = 0; i < m_currentConfig.points.size(); i++) {
        if (i == points.size()) break;

        replacedPoints.insert(m_currentConfig.points[i]);

        cocos2d::CCPoint pos = bottomLeft + m_currentConfig.points[i].pos * iconSize;
        cocos2d::ccColor3B color = m_currentConfig.points[i].color;

        points[i]->setColor(color, 0.2f);
        points[i]->runAction(CCEaseSineOut::create(CCMoveTo::create(0.1f, pos)));

        movedPoints.insert(points[i]);
    }

    bool addedSelected = false;

    for (const SimplePoint& point : m_currentConfig.points) {
        if (replacedPoints.contains(point)) continue;
        
        addPoint(bottomLeft + point.pos * iconSize, true);

        if (!m_selectedPoint && !config.points.empty() && !addedSelected) {
            m_points.back()->setSelected(true);

            m_selectedPoint = m_points.back();
            addedSelected = true;
        }

        m_points.back()->setColor(point.color);
        m_points.back()->setHidden(false, 0.1f);

        movedPoints.insert(m_points.back());
    }

    for (ColorNode* point : m_points)
        if (!movedPoints.contains(point))
            point->setHidden(true, 0.1f);

    runAction(CCSequence::create(
        CCDelayTime::create(0.1f),
        CCCallFunc::create(this, callfunc_selector(PointsLayer::onAnimationEnded)),
        nullptr 
    ));

}

void PointsLayer::onAnimationEnded() {
    // m_icon->runAction(CCFadeTo::create(0.05f, 255));

    cocos2d::CCSize iconSize = ccp(30.5f, 30) * m_icon->getScale();
    cocos2d::CCPoint bottomLeft = m_icon->getPosition() - iconSize / 2.f;
    cocos2d::CCPoint selectPos = {0, 0};

    for (ColorNode* point : m_points)
        if (point->isSelected()) {
            selectPos = point->getPosition();
            break;
        }
        
    for (ColorNode* point : m_points)
        point->removeFromParentAndCleanup(true);

    m_points.clear();

    m_isAnimating = false;
    m_selectedPoint = nullptr;
    m_hoveredPoint = nullptr;

    ColorNode* realSelectedPoint = nullptr;

    for (const SimplePoint& point : m_currentConfig.points) {
        cocos2d::CCPoint pos = bottomLeft + point.pos * iconSize;

        addPoint(pos);
        
        m_points.back()->setColor(point.color);

        if (std::abs(pos.x - selectPos.x) < 0.001f && std::abs(pos.y - selectPos.y) < 0.001f) {
            m_points.back()->setSelected(true);

            realSelectedPoint = m_points.back();
            selectPos = ccp(0, 0);
        }
    }

    selectPoint(realSelectedPoint);
}
