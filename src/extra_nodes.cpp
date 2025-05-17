#include "extra_nodes.hpp"
#include "utils.hpp"

ColorNode* ColorNode::create() {
    ColorNode* ret = new ColorNode();

    if (ret->init()) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

bool ColorNode::init() {
    if (!CCNode::init()) return false;

    setAnchorPoint({0.5f, 0.5f});
    setScale(0.8f);

    m_dot = CCSprite::create("dot.png"_spr);
    m_dot->setPosition(m_dot->getContentSize() / 2.f);

    setContentSize(m_dot->getContentSize());

    m_circle = CCSprite::create("circle.png"_spr);
    m_circle->setScale(0.275f);
    m_circle->setPosition(getContentSize() / 2.f);
    m_circle->runAction(CCRepeatForever::create(CCRotateBy::create(8, 360)));

    m_circle->setVisible(false);

    addChild(m_dot);
    addChild(m_circle);
    
    return true;
}

void ColorNode::setSelected(bool selected) {
    m_circle->setVisible(selected);

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

void ColorNode::setColor(const cocos2d::ccColor3B& color) {
    m_dot->setColor(color);

    m_color = color;

    setHovered(m_isHovered);
}

const cocos2d::ccColor3B& ColorNode::getColor() {
    return m_color;
}

void ColorNode::setGradient(GradientConfig config) {

}

ColorToggle* ColorToggle::create(CCObject* target, cocos2d::SEL_MenuHandler callback, bool secondary) {
    ColorToggle* ret = new ColorToggle();

    if (ret->init(target, callback, secondary)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

bool ColorToggle::init(CCObject* target, cocos2d::SEL_MenuHandler callback, bool secondary) {
    m_sprite = ColorChannelSprite::create();
    m_sprite->setScale(0.6f);

    CCLabelBMFont* lbl = CCLabelBMFont::create(secondary ? "2" : "1", "bigFont.fnt");
    lbl->setScale(0.525f);
    lbl->setPosition({24, 14});

    m_sprite->addChild(lbl);

    m_select = CCSprite::createWithSpriteFrameName("GJ_select_001.png");
    m_select->setScale(0.7f);
    m_select->setPosition(m_sprite->getContentSize() * 0.6f / 2.f);
    m_select->setVisible(false);

    addChild(m_sprite);
    addChild(m_select);

    setContentSize(m_sprite->getContentSize());
    setAnchorPoint({0, 0});

    return CCMenuItemSpriteExtra::init(m_sprite, nullptr, target, callback);
}

void ColorToggle::setSelected(bool selected) {
    m_select->setVisible(selected);
}

void ColorToggle::setColor(const cocos2d::ccColor3B& color) {
    m_sprite->setColor(color);
}

IconButton* IconButton::create(CCObject* target, cocos2d::SEL_MenuHandler callback, IconType type) {
    IconButton* ret = new IconButton();

    ret->m_iconType = type;

    if (ret->init(target, callback)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
};

bool IconButton::init(CCObject* target, cocos2d::SEL_MenuHandler callback) {
    m_icon = Utils::createIcon(m_iconType);
    Utils::setIconColors(m_icon, false);

    cocos2d::CCSize buttonSize = m_icon->getChildByType<CCSprite>(0)->getContentSize() * 0.63f;

    if (m_iconType == IconType::Robot || m_iconType == IconType::Spider)
        m_icon->setScale(0.474f);
    else {
        cocos2d::CCSize size = m_icon->m_firstLayer->getContentSize();
        float max = size.width > size.height ? size.width : size.height;

        m_icon->setScale(max > 19.21f ? 19.21f / max : 0.63f);
    }

    if (m_iconType == IconType::Wave)
        m_icon->setScale(m_icon->getScale() - 0.1f);

    m_icon->setPosition(buttonSize / 2.f);

    CCNode* container = CCNode::create();
    container->setContentSize(buttonSize);
    container->addChild(m_icon);

    m_lock = CCSprite::createWithSpriteFrameName("GJ_lock_001.png");
    m_lock->setScale(0.3f);
    m_lock->setOpacity(200);
    m_lock->setPosition(buttonSize / 2.f + ccp(7.818f, -7));
    m_lock->setVisible(false);

    container->addChild(m_lock);

    m_select = CCSprite::createWithSpriteFrameName("GJ_select_001.png");
    m_select->setScale(0.7f);
    m_select->setPosition(buttonSize / 2.f);
    m_select->setVisible(false);
    
    container->addChild(m_select);

    return CCMenuItemSpriteExtra::init(container, nullptr, target, callback);
}

IconType IconButton::getType() {
    return m_iconType;
}

bool IconButton::isLocked() {
    return m_isLocked;
}

void IconButton::setSelected(bool selected) {
    m_select->setVisible(selected);
}

void IconButton::setLocked(bool locked) {
    m_isLocked = locked;
    m_lock->setVisible(locked);
}

void IconButton::applyGradient(GradientConfig config) {
    Utils::applyGradient(m_icon, config);
}

PointsLayer* PointsLayer::create(const cocos2d::CCSize& size, PointsLayerDelegate* delegate) {
    PointsLayer* ret = new PointsLayer();

    ret->m_delegate = delegate;

    if (ret->init(size)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
};

ColorPicker* ColorPicker::create() {
    ColorPicker* ret = new ColorPicker();

    if (ret->init()) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
};

bool ColorPicker::init() {
    if (!CCNode::init()) return false;

    m_picker = CCControlColourPicker::colourPicker();

    addChild(m_picker);
    setContentSize(m_picker->getContentSize());

    return true;
}

void ColorPicker::setDelegate(ColorPickerDelegate* delegate) {
    m_picker->setDelegate(delegate);
}

void ColorPicker::setColor(const cocos2d::ccColor3B& color) {
    m_picker->setColorValue(color);
}

const cocos2d::ccColor3B ColorPicker::getColor() {
    return m_picker->getColorValue();
}

void ColorPicker::setEnabled(bool enabled) {
    m_picker->getChildByType<CCControlHuePicker>(0)->setEnabled(enabled);
    m_picker->getChildByType<CCControlSaturationBrightnessPicker>(0)->setEnabled(enabled);
    
    for (CCSprite* spr : CCArrayExt<CCSprite*>(m_picker->getChildByType<CCSpriteBatchNode>(0)->getChildren()))
        spr->setOpacity(enabled ? 255 : 100);
}

bool PointsLayer::init(cocos2d::CCSize size) {
    if (!CCLayer::init()) return false;

    m_shadow = CCSprite::create("shadow.png"_spr);
    m_shadow->setOpacity(49);
    m_shadow->setPosition({178.5f, 170});
    m_shadow->setScaleY(0.8f);

    m_centerIcon = Utils::createIcon(IconType::Cube);
    m_centerIcon->setScale(1.7f);
    m_centerIcon->setPosition({178.5f, 200});

    addChild(m_shadow);
    addChild(m_centerIcon);

    updateCenter();

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
    std::vector<ColorNode*> newPoints;

    for (ColorNode* point : m_points)
        if (point != m_selectedPoint)
            newPoints.push_back(point);

    m_points = newPoints;

    m_selectedPoint->removeFromParentAndCleanup(true);

    m_selectedPoint = nullptr;
}

void PointsLayer::addPoint() {
    cocos2d::CCSize size = m_centerIcon->getContentSize() * m_centerIcon->getScale();
    cocos2d::CCPoint position = m_centerIcon->getPosition();

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
                static_cast<int>(point->getPosition().x) == static_cast<int>(corner.x) &&
                static_cast<int>(point->getPosition().y) == static_cast<int>(corner.y)
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

void PointsLayer::addPoint(const cocos2d::CCPoint& pos) {
    ColorNode* node = ColorNode::create();

    node->setPosition(pos);

    addChild(node);

    m_points.push_back(node);
}

ColorNode* PointsLayer::getNodeForPos(cocos2d::CCPoint pos) {
    pos = convertToNodeSpace(pos);

    ColorNode* ret = nullptr;
    float closest = getContentSize().width * 2;

    for (ColorNode* point : m_points) {
        float distance = ccpDistance(pos, point->getPosition());
        if (distance < closest && distance < point->getContentSize().width / 2.f) {
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
    point->setSelected(true);

    if (m_selectedPoint && m_selectedPoint != point)
        m_selectedPoint->setSelected(false);

    m_selectedPoint = point;

    m_delegate->pointSelected(point);
}

bool PointsLayer::ccTouchBegan(CCTouch* touch, CCEvent* event) { 
    cocos2d::CCPoint pos = touch->getLocation();

    if (ColorNode* point = getNodeForPos(pos)) {
        selectPoint(point);

        m_isMoving = true;
        m_moveOffset = point->getContentSize() / 2.f - point->convertToNodeSpace(pos);

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

        m_delegate->pointMoved();
    }
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

void PointsLayer::ccTouchEnded(CCTouch* touch, CCEvent* event) {
    m_isMoving = false;
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

void PointsLayer::updateGradient(GradientConfig config) {
    Utils::applyGradient(m_centerIcon, config);
}

void PointsLayer::updateCenter() {
    m_centerIcon->setContentSize(m_centerIcon->m_firstLayer->getContentSize());
    m_centerIcon->m_firstLayer->setPosition(m_centerIcon->getContentSize() / 2.f);

    if (m_centerIcon->m_robotSprite)
        m_centerIcon->m_robotSprite->setPosition(m_centerIcon->getContentSize() / 2.f);

    if (m_centerIcon->m_spiderSprite)
        m_centerIcon->m_spiderSprite->setPosition(m_centerIcon->getContentSize() / 2.f);

    Utils::setIconColors(m_centerIcon, false);
}

ColorNode* PointsLayer::getSelectedPoint() {
    return m_selectedPoint;
}

void PointsLayer::setPlayerFrame(IconType type) {
    m_centerIcon->updatePlayerFrame(
        Utils::getIconID(type),
        type
    );

    updateCenter();
}

cocos2d::CCPoint PointsLayer::getRelativePos(ColorNode* point) {
    cocos2d::CCSize iconSize = ccp(30.5f, 30) * m_centerIcon->getScale();
    cocos2d::CCPoint realPos = point->getPosition() - (m_centerIcon->getPosition() - iconSize / 2.f);

    return {
        realPos.x / iconSize.width,
        realPos.y / iconSize.height
    };
}

void PointsLayer::loadPoints(const std::vector<SimplePoint>& points) {
    for (ColorNode* point : m_points)
        point->removeFromParentAndCleanup(true);

    m_points.clear();

    m_selectedPoint = nullptr;
    m_hoveredPoint = nullptr;

    cocos2d::CCSize iconSize = ccp(30.5f, 30) * m_centerIcon->getScale();
    cocos2d::CCPoint bottomLeft = m_centerIcon->getPosition() - iconSize / 2.f;

    for (const SimplePoint& point : points) {
        addPoint(bottomLeft + point.pos * iconSize);
        m_points.back()->setColor(point.color);
    }
}