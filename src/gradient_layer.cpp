#include "gradient_layer.hpp"
#include "utils.hpp"

#include <Geode/ui/GeodeUI.hpp>

GradientLayer* layer = nullptr;

GradientLayer::~GradientLayer() {
    layer = nullptr;
}

#ifdef GEODE_IS_WINDOWS

#include <Geode/modify/CCEGLView.hpp>

class $modify(CCEGLView) {
    void onGLFWMouseMoveCallBack(GLFWwindow* v1, double v2, double v3) {
        CCEGLView::onGLFWMouseMoveCallBack(v1, v2, v3);

        if (!layer) {
            CCScene* scene = CCDirector::get()->getRunningScene();
            layer = scene->getChildByType<GradientLayer>(0);

            if (!layer) return;
        }

        layer->updateHover();

    }
};

#endif

void GradientLayer::updateHover() {
    m_pointsLayer->updateHover(getMousePos());
}

void GradientLayer::pointMoved() {
    save();
    updateGradient();
}

void GradientLayer::pointSelected(CCNode* point) {
    m_picker->setColor(static_cast<ColorNode*>(point)->getColor());
}
 
GradientLayer* GradientLayer::create() {
    GradientLayer* ret = new GradientLayer();

    if (ret->initAnchored(320, 280)) {
        ret->autorelease();
        layer = ret;
        return ret;
    }

    delete ret;
    return nullptr;
}

void GradientLayer::updateGradient(bool force, bool both, bool transition) {
    m_currentConfig = Utils::getSavedConfig(m_selectedButton->getType(), m_isSecondaryColor);

    if (both) {
        m_pointsLayer->updateGradient(Utils::getSavedConfig(m_selectedButton->getType(), !m_isSecondaryColor), !m_isSecondaryColor, force);
        m_pointsLayer->updateGradient(Utils::getSavedConfig(m_selectedButton->getType(), m_isSecondaryColor), m_isSecondaryColor, force);
    } else
        m_pointsLayer->updateGradient(m_currentConfig, m_isSecondaryColor, force);

    for (IconButton* button : m_buttons) {    
        if (both) {
            button->applyGradient(force, !m_isSecondaryColor, transition);
            button->applyGradient(force, m_isSecondaryColor, transition);
        } else
            button->applyGradient(force, m_isSecondaryColor, transition);

        button->setColor(m_isSecondaryColor, false);
    }

    m_mainColorToggle->applyGradient(Utils::getSavedConfig(m_selectedButton->getType(), false), force, transition);
    m_secondaryColorToggle->applyGradient(Utils::getSavedConfig(m_selectedButton->getType(), true), force, transition);
}

void GradientLayer::updateUI() {
    m_currentConfig = Utils::getSavedConfig(m_selectedButton->getType(), m_isSecondaryColor);

    bool hasPoints = m_currentConfig.points.size() > 0;
    bool canAddPoints = m_currentConfig.points.size() < 24;

    m_addButton->setEnabled(canAddPoints);
    m_addButton->setOpacity(canAddPoints ? 255 : 140);

    m_removeButton->setEnabled(hasPoints);
    m_removeButton->setOpacity(hasPoints ? 255 : 140);

    m_picker->setEnabled(hasPoints);
    m_rInput->setEnabled(hasPoints);
    m_gInput->setEnabled(hasPoints);
    m_bInput->setEnabled(hasPoints);

    m_pointsLayer->setPointsHidden(m_pointsHidden, 0.f);
}

void GradientLayer::onAddPoint(CCObject*) {
    GameManager* gm = GameManager::get();

    m_pointsLayer->addPoint();
    m_pointsLayer->selectLast();
    m_pointsLayer->getSelectedPoint()->flash();
    m_pointsLayer->getSelectedPoint()->setColor(
        gm->colorForIdx(m_isSecondaryColor ? gm->getPlayerColor2() : gm->getPlayerColor())
    );

    save();
    updateUI();
    updateGradient();
}

void GradientLayer::onRemovePoint(CCObject*) {
    m_pointsLayer->removeSelected();
    m_pointsLayer->selectLast();

    save();
    updateUI();
    updateGradient();
}

void GradientLayer::onSettings(CCObject*) {
    geode::openSettingsPopup(Mod::get(), true);
}

void GradientLayer::load(IconType type, bool secondary, bool force, bool both, bool transition) {
    m_currentConfig = Utils::getSavedConfig(type, secondary);

    m_pointsLayer->loadPoints(m_currentConfig);

    for (IconButton* button : m_buttons)
        if (type == button->getType()) {
            button->setSelected(true);
            break;
        }

    updateUI();
    updateGradient(force, both, transition);

    m_linearToggle->toggle(m_currentConfig.isLinear);
    m_radialToggle->toggle(!m_currentConfig.isLinear);
    m_dotToggle->toggle(m_selectedButton->isLocked());
}

void GradientLayer::save() {
    m_currentConfig.points = m_pointsLayer->getPoints();
    save(m_currentConfig, m_isSecondaryColor);
}

void GradientLayer::save(GradientConfig config, bool secondary) {
    std::string id = !m_selectedButton->isLocked() ? "global" : Utils::getTypeID(m_selectedButton->getType());
    std::string color = secondary ? "color2" : "color1";

    matjson::Value container = Mod::get()->getSavedValue<matjson::Value>(id);
    matjson::Value pointsObject = matjson::Value::array();

    for (SimplePoint point : config.points) {
        matjson::Value object = matjson::Value{};

        object["pos"]["x"] = point.pos.x;
        object["pos"]["y"] = point.pos.y;

        object["color"]["r"] = point.color.r;
        object["color"]["g"] = point.color.g;
        object["color"]["b"] = point.color.b;

        pointsObject.push(object);
    }

    container[color]["points"] = pointsObject;
    container[color]["linear"] = config.isLinear;

    Mod::get()->setSavedValue(id, container);
}

void GradientLayer::onIconButton(CCObject* sender) {
    IconButton* button = static_cast<IconButton*>(sender);

    if (button == m_selectedButton) return;

    m_pointsLayer->setPlayerFrame(button->getType());
    m_dotToggle->toggle(button->isLocked());
    button->setSelected(true);

    if (m_selectedButton && m_selectedButton != button)
        m_selectedButton->setSelected(false);

    m_selectedButton = button;

    load(button->getType(), m_isSecondaryColor, true, true);
}

void GradientLayer::onTypeToggle(CCObject* sender) {
    CCMenuItemToggler* toggler = static_cast<CCMenuItemToggler*>(sender);

    bool isLinear = toggler == m_linearToggle;

    if (isLinear == m_currentConfig.isLinear)
        return toggler->toggle(!toggler->isToggled());

    m_linearToggle->toggle(false);
    m_radialToggle->toggle(false);

    m_currentConfig.isLinear = isLinear;

    Loader::get()->queueInMainThread([this] {
        m_linearToggle->toggle(m_currentConfig.isLinear);
        m_radialToggle->toggle(!m_currentConfig.isLinear);
    });

    save();
    updateGradient(true, false, true);
}

void GradientLayer::onLockToggle(CCObject* sender) {
    if (!m_selectedButton) return;

    if (!m_selectedButton->isLocked()) {
        m_selectedButton->setLocked(!m_selectedButton->isLocked());

        save(Utils::getSavedConfig(static_cast<IconType>(-1), false), false);
        save(Utils::getSavedConfig(static_cast<IconType>(-1), true), true);
    } else {
        Mod::get()->getSaveContainer().erase(Utils::getTypeID(m_selectedButton->getType()));
        // load(static_cast<IconType>(-1), m_isSecondaryColor, true, true, true);
        
        m_selectedButton->setLocked(!m_selectedButton->isLocked());
    }
}

void GradientLayer::onColorToggle(CCObject* sender) {
    ColorToggle* toggle = static_cast<ColorToggle*>(sender);

    m_mainColorToggle->setSelected(false);
    m_secondaryColorToggle->setSelected(false);

    toggle->setSelected(true);

    m_isSecondaryColor = m_secondaryColorToggle == toggle;

    load(m_selectedButton->getType(), m_isSecondaryColor, true, true, true);
}

void GradientLayer::onHideToggle(CCObject* sender) {
    m_pointsHidden = !static_cast<CCMenuItemToggler*>(sender)->isToggled();
    m_pointsLayer->setPointsHidden(m_pointsHidden, 0.15f);
}

void GradientLayer::textChanged(CCTextInputNode* input) {
    int r = numFromString<int>(m_rInput->getString()).unwrapOr(0);
    int g = numFromString<int>(m_gInput->getString()).unwrapOr(0);
    int b = numFromString<int>(m_bInput->getString()).unwrapOr(0);

    if (r > 255 || r < 0) return m_rInput->setString(std::to_string(m_picker->getColor().r).c_str());
    if (g > 255 || g < 0) return m_gInput->setString(std::to_string(m_picker->getColor().g).c_str());
    if (b > 255 || b < 0) return m_bInput->setString(std::to_string(m_picker->getColor().b).c_str());

    m_ignoreColorChange = true;
    
    m_picker->setColor(ccc3(r, g, b));

    m_ignoreColorChange = false;
}

void GradientLayer::colorValueChanged(cocos2d::ccColor3B color) {
    if (!m_ignoreColorChange) {
        m_rInput->setString(std::to_string(color.r).c_str());
        m_gInput->setString(std::to_string(color.g).c_str());
        m_bInput->setString(std::to_string(color.b).c_str());
    }

    if (ColorNode* point = m_pointsLayer->getSelectedPoint())
        point->setColor(color);

    save();
    updateGradient();
}

bool GradientLayer::setup() {
    m_closeBtn->setPosition(m_closeBtn->getPosition() + ccp(-3, 3));
    m_closeBtn->getChildByType<CCSprite>(0)->setScale(0.6f);

    CCSprite* spr = CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
    spr->setScale(0.57f);

    CCMenuItemSpriteExtra* btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(GradientLayer::onSettings));
    btn->setPosition(m_size);

    m_buttonMenu->addChild(btn);

    setTitle("Icon Gradients");
    m_title->setPosition({178.5f, 262});
    m_title->setScale(0.675f);

    CCScale9Sprite* bg = CCScale9Sprite::create("square02b_001.png");
	bg->setColor({0, 0, 0});
	bg->setOpacity(49);
	bg->setContentSize({ 60, 518 });
    bg->setAnchorPoint({0, 0});
    bg->setScale(0.5f);
    bg->setPosition({12, 12});

    m_mainLayer->addChild(bg);

    bg = CCScale9Sprite::create("square02b_001.png");
	bg->setColor({0, 0, 0});
	bg->setOpacity(49);
	bg->setContentSize({ 259, 126 });
    bg->setAnchorPoint({0, 0});
    bg->setPosition({49, 12});
    
    m_mainLayer->addChild(bg);

    for (size_t i = 0; i < 9; ++i) {
        IconType type = static_cast<IconType>(i);

        IconButton* btn = IconButton::create(this, menu_selector(GradientLayer::onIconButton), type);

        m_buttons.push_back(btn);

        btn->setPosition({27, 249.f - 27.4f * i});

        m_buttonMenu->addChild(btn);
    }

    m_selectedButton = m_buttons.front();

    m_pointsLayer = PointsLayer::create(m_size, static_cast<PointsLayerDelegate*>(this));
    m_mainLayer->addChild(m_pointsLayer, 100);

    Loader::get()->queueInMainThread([this] {
        if (CCTouchHandler* handler = CCTouchDispatcher::get()->findHandler(m_pointsLayer))
            CCTouchDispatcher::get()->setPriority(-1000, handler->getDelegate());
    });

    m_picker = ColorPicker::create();
    m_picker->setScale(0.575f);
    m_picker->setPosition({112, 75});
    m_picker->setDelegate(this);

    m_mainLayer->addChild(m_picker);

    CCLabelBMFont* lbl = CCLabelBMFont::create("R", "bigFont.fnt");
    lbl->setOpacity(140);
    lbl->setScale(0.425f);
    lbl->setPosition({175, 100});

    m_mainLayer->addChild(lbl);

    lbl = CCLabelBMFont::create("G", "bigFont.fnt");
    lbl->setOpacity(140);
    lbl->setScale(0.425f);
    lbl->setPosition({175, 75});

    m_mainLayer->addChild(lbl);

    lbl = CCLabelBMFont::create("B", "bigFont.fnt");
    lbl->setOpacity(140);
    lbl->setScale(0.425f);
    lbl->setPosition({175, 50});

    m_mainLayer->addChild(lbl);

    m_rInput = TextInput::create(50, "R");
    m_rInput->setScale(0.625f);
    m_rInput->setPosition({205, 100});
    m_rInput->setString("255");
    m_rInput->getInputNode()->setDelegate(this);
    m_rInput->getInputNode()->setAllowedChars("0123456789");

    m_mainLayer->addChild(m_rInput);

    m_gInput = TextInput::create(50, "G");
    m_gInput->setScale(0.625f);
    m_gInput->setPosition({205, 75});
    m_gInput->setString("255");
    m_gInput->getInputNode()->setDelegate(this);
    m_gInput->getInputNode()->setAllowedChars("0123456789");

    m_mainLayer->addChild(m_gInput);

    m_bInput = TextInput::create(50, "B");
    m_bInput->setScale(0.625f);
    m_bInput->setPosition({205, 50});
    m_bInput->setString("255");
    m_bInput->getInputNode()->setDelegate(this);
    m_bInput->getInputNode()->setAllowedChars("0123456789");

    m_mainLayer->addChild(m_bInput);

    spr = CCSprite::createWithSpriteFrameName("GJ_plusBtn_001.png");
    spr->setScale(0.475f);

    m_addButton = CCMenuItemSpriteExtra::create(spr, this, menu_selector(GradientLayer::onAddPoint));
    m_addButton->setPosition({58, 154});
    m_addButton->setCascadeOpacityEnabled(true);

    m_buttonMenu->addChild(m_addButton);

    spr = CCSprite::createWithSpriteFrameName("GJ_trashBtn_001.png");
    spr->setScale(0.55f);

    m_removeButton = CCMenuItemSpriteExtra::create(spr, this, menu_selector(GradientLayer::onRemovePoint));
    m_removeButton->setPosition({58, 180});
    m_removeButton->setCascadeOpacityEnabled(true);

    m_buttonMenu->addChild(m_removeButton);

    CCMenuItemToggler* toggle = CCMenuItemToggler::create(
        CCSprite::createWithSpriteFrameName("hideBtn_001.png"),
        CCSprite::create("show.png"_spr),
        this,
        menu_selector(GradientLayer::onHideToggle)
    );
    toggle->setPosition({304, 147});
    toggle->setScale(0.6f);
    toggle->setCascadeOpacityEnabled(true);
    toggle->setOpacity(140);

    m_buttonMenu->addChild(toggle);

    m_linearToggle = Utils::createTypeToggle(false, {264.3f, 116}, this, menu_selector(GradientLayer::onTypeToggle));
    m_buttonMenu->addChild(m_linearToggle);

    m_radialToggle = Utils::createTypeToggle(true, {264.3f, 76}, this, menu_selector(GradientLayer::onTypeToggle));
    m_buttonMenu->addChild(m_radialToggle);

    lbl = CCLabelBMFont::create("Linear", "bigFont.fnt");
    lbl->setScale(0.255f);
    lbl->setPosition({264.3f, 99});

    m_mainLayer->addChild(lbl);

    lbl = CCLabelBMFont::create("Radial", "bigFont.fnt");
    lbl->setScale(0.255f);
    lbl->setPosition({264.3f, 58});

    m_mainLayer->addChild(lbl);

    m_dotToggle = CCMenuItemToggler::create(
        CCSprite::createWithSpriteFrameName("GJ_lock_open_001.png"),
        CCSprite::createWithSpriteFrameName("GJ_lock_001.png"),
        this,
        menu_selector(GradientLayer::onLockToggle)
    );

    m_dotToggle->setScale(0.525f);
    m_dotToggle->setPosition({297, 128});

    m_buttonMenu->addChild(m_dotToggle);

    m_mainColorToggle = ColorToggle::create(this, menu_selector(GradientLayer::onColorToggle), false);
    m_mainColorToggle->setPosition({247, 36});

    m_buttonMenu->addChild(m_mainColorToggle);

    m_secondaryColorToggle = ColorToggle::create(this, menu_selector(GradientLayer::onColorToggle), true);
    m_secondaryColorToggle->setPosition({282, 36});

    m_buttonMenu->addChild(m_secondaryColorToggle);

    for (IconButton* button : m_buttons)
        button->setLocked(Mod::get()->hasSavedValue(Utils::getTypeID(button->getType())));

    m_mainColorToggle->setSelected(true);

    load(IconType::Cube, false, true, true);

    m_pointsLayer->selectLast();

    return true;
} 