#include "GradientLayer.hpp"
#include "LoadLayer.hpp"
#include "ColorSelectLayer.hpp"

#include "../Utils/Utils.hpp"
#include "../Utils/Cache.hpp"

#include <Geode/ui/GeodeUI.hpp>
#include <Geode/loader/Event.hpp>
#include <Geode/loader/Dispatch.hpp>

GradientLayer* layer = nullptr;

$execute {

    geode::listenForSettingChanges("disabled", +[](bool value) {
        if (layer)
            layer->updateGarage(false);
    });

    geode::listenForSettingChanges("point-opacity", +[](int64_t value) {
        if (layer)
            layer->updatePointOpacity(value);
    });

    geode::listenForSettingChanges("point-scale", +[](double value) {
        if (layer)
            layer->updatePointScale(value);
    });

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

GradientLayer::~GradientLayer() {
    updateGarage(false);

    layer = nullptr;
}

void GradientLayer::updateHover() {
    m_pointsLayer->updateHover(getMousePos());
}

void GradientLayer::updatePointOpacity(int value) {
    m_pointsLayer->updatePointOpacity(value);
}

void GradientLayer::updatePointScale(float value) {
    m_pointsLayer->updatePointScale(value);
}

void GradientLayer::updateGarage(bool quick, bool real) {
    if (!m_garage) return;

    IconType type = m_garage->getType();

    // if (m_pointsLayer->getType() != type && quick)
    //     return;

    if ((type == IconType::Robot || type == IconType::Spider) && quick)
        return; // this desnt work bup
    
    quick ? m_garage->updateQuickGradient() : m_garage->updateGradient();
}

void GradientLayer::pointMoved() {
    save();
    updateGradient();
    
    if (m_firstPoint)
        if (ccpDistance(m_firstPosition, m_firstPoint->getPosition()) > 3.f) {
            m_firstPoint = nullptr;
            m_movePointsSprite->runAction(CCFadeTo::create(0.7f, 0));
            Mod::get()->setSavedValue("moved-point", true);
        }
            
}

void GradientLayer::pointSelected(CCNode* point) {
    cocos2d::ccColor3B color = static_cast<ColorNode*>(point)->getColor();
    
    m_picker->setColor(color);
    m_colorSelector->setColor(color, 0.15f);
    
    if (m_firstPoint) {
        m_firstPoint = static_cast<ColorNode*>(point);
        m_firstPosition = m_firstPoint->getPosition();
    }
}

void GradientLayer::pointReleased() {
    updateGarage(true, true);
}

void GradientLayer::colorSelected(const cocos2d::ccColor3B& color) {
    m_rInput->setString(std::to_string(color.r).c_str());
    m_gInput->setString(std::to_string(color.g).c_str());
    m_bInput->setString(std::to_string(color.b).c_str());

    textChanged(nullptr);
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

    updateGarage(true);

    if (both)
        m_pointsLayer->updateGradient(Utils::getSavedConfig(m_selectedButton->getType(), !m_isSecondaryColor), !m_isSecondaryColor, force);
    
    m_pointsLayer->updateGradient(m_currentConfig, m_isSecondaryColor, force);

    for (IconButton* button : m_buttons) {
        button->applyGradient(force, m_isSecondaryColor, transition, both);
        button->setColor(m_isSecondaryColor, false);
    }

    m_mainColorToggle->applyGradient(Utils::getSavedConfig(m_selectedButton->getType(), false), force, transition);
    m_secondaryColorToggle->applyGradient(Utils::getSavedConfig(m_selectedButton->getType(), true), force, transition);
}

void GradientLayer::updateCountLabel() {
    m_countLabel->setString(fmt::format("{}/24", m_pointsLayer->getPointCount()).c_str());

    m_countLabel->runAction(CCSequence::create(
        CCFadeTo::create(0.05f, 95),
        CCFadeTo::create(2.f, 0),
        nullptr
    ));
}

void GradientLayer::updateUI() {
    m_currentConfig = Utils::getSavedConfig(m_selectedButton->getType(), m_isSecondaryColor);

    bool hasPoints = m_currentConfig.points.size() > 0;
    bool canAddPoints = m_currentConfig.points.size() < 24;

    m_addButton->setEnabled(canAddPoints);
    m_addButton->setOpacity(canAddPoints ? 255 : 140);

    m_removeButton->setEnabled(hasPoints);
    m_removeButton->setOpacity(hasPoints ? 255 : 140);

    m_copyButton->setEnabled(hasPoints);
    m_copyButton->setOpacity(hasPoints ? 255 : 140);

    m_saveButton->setEnabled(hasPoints);
    m_saveButton->setOpacity(hasPoints ? 255 : 140);

    m_hideToggle->setEnabled(hasPoints);
    m_hideToggle->setOpacity(hasPoints ? 140 : 70);

    m_pasteButton->setEnabled(!Cache::getCopiedConfig().points.empty());
    m_pasteButton->setOpacity(!Cache::getCopiedConfig().points.empty() ? 255 : 140);

    m_colorSelector->setEnabled(hasPoints);
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

    if (m_pointsHidden) {
        onHideToggle(nullptr);
        m_hideToggle->toggle(false);
    }
    
    cocos2d::ccColor3B color = gm->colorForIdx(m_isSecondaryColor ? gm->getPlayerColor2() : gm->getPlayerColor());
    
    m_pointsLayer->getSelectedPoint()->setColor(color);
    m_picker->setColor(color);
    m_colorSelector->setColor(color, 0.15f);
    
    if (m_addColorsSprite) {
        m_addColorsSprite->runAction(CCFadeTo::create(0.7f, 0));
        m_addColorsSprite = nullptr;
        Mod::get()->setSavedValue("added-color", true);
    }

    save();
    updateUI();
    updateGradient();
    updateCountLabel();
}

void GradientLayer::onRemovePoint(CCObject*) {
    m_pointsLayer->removeSelected();
    m_pointsLayer->selectLast();

    if (m_pointsLayer->getPointCount() <= 0 && m_pointsHidden) {
        onHideToggle(nullptr);
        m_hideToggle->toggle(false);
    }

    save();
    updateUI();
    updateGradient();
    updateCountLabel();
}

void GradientLayer::onSettings(CCObject*) {
    geode::openSettingsPopup(Mod::get(), true);
}

void GradientLayer::onCopy(CCObject*) {
    Cache::setCopiedConfig({
        m_pointsLayer->getPoints(),
        m_currentConfig.isLinear
    });

    updateUI();
}

void GradientLayer::onPaste(CCObject*) {
    load(Cache::getCopiedConfig());
}

void GradientLayer::load(GradientConfig config) {
    if (config.points.empty()) return;

    save(config, m_isSecondaryColor);
    load(m_selectedButton->getType(), m_isSecondaryColor, true, true, true);
} 

void GradientLayer::onSave(CCObject*) {
    m_currentConfig.points = m_pointsLayer->getPoints();

    if (Utils::isGradientSaved(m_currentConfig))
        return Notification::create("Gradient is already saved", NotificationIcon::Error, 0.1f)->show();

    Utils::saveConfig(m_currentConfig, "saved-gradients", "");

    Notification::create("Gradient saved", NotificationIcon::Success, 0.1f)->show();
}

void GradientLayer::onLoad(CCObject*) {
    LoadLayer::create(this)->show();
}

void GradientLayer::load(IconType type, bool secondary, bool force, bool both, bool transition) {
    GradientConfig previousConfig = m_currentConfig;

    m_currentConfig = Utils::getSavedConfig(type, secondary);

    m_pointsLayer->loadPoints(m_currentConfig, previousConfig != m_currentConfig && transition);

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
    if (!m_selectedButton) return;
    
    std::string id = !m_selectedButton->isLocked() ? "global" : Utils::getTypeID(m_selectedButton->getType());
    std::string color = secondary ? "color2" : "color1";

    Utils::saveConfig(config, id, color);
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

    load(button->getType(), m_isSecondaryColor, true, true, true);

    Cache::setLastSelected(button->getType());
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

        save(Utils::getSavedConfig(static_cast<IconType>(-1), !m_isSecondaryColor), !m_isSecondaryColor);
        save(Utils::getSavedConfig(static_cast<IconType>(-1), m_isSecondaryColor), m_isSecondaryColor);
    } else {
        std::string id = Utils::getTypeID(m_selectedButton->getType());

        if (Mod::get()->hasSavedValue(id) && id != "global")
            Mod::get()->getSaveContainer().erase(id);

        bool locked = !m_selectedButton->isLocked();
        
        m_selectedButton->setLocked(locked);

        load(static_cast<IconType>(-1), m_isSecondaryColor, true, true, true);

        Loader::get()->queueInMainThread([this, locked] {
            m_dotToggle->toggle(locked);
        });
    }
}

void GradientLayer::onColorToggle(CCObject* sender) {
    ColorToggle* toggle = static_cast<ColorToggle*>(sender);
    
    if (toggle == m_mainColorToggle && m_mainColorToggle->isSelected()) return;
    if (toggle == m_secondaryColorToggle && m_secondaryColorToggle->isSelected()) return;

    m_mainColorToggle->setSelected(false);
    m_secondaryColorToggle->setSelected(false);

    toggle->setSelected(true);

    m_isSecondaryColor = m_secondaryColorToggle == toggle;

    load(m_selectedButton->getType(), m_isSecondaryColor, true, true, true);
}

void GradientLayer::onColorSelector(CCObject*) {
    ColorSelectLayer::create(this)->show();
}

void GradientLayer::onHideToggle(CCObject* sender) {
    m_pointsHidden = !m_hideToggle->isToggled();
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
    
    cocos2d::ccColor3B color = ccc3(r, g, b);
    
    m_picker->setColor(color);
    m_colorSelector->setColor(color, 0.15f);

    m_ignoreColorChange = false;
}

void GradientLayer::colorValueChanged(cocos2d::ccColor3B color) {
    if (!m_ignoreColorChange) {
        m_rInput->setString(std::to_string(color.r).c_str());
        m_gInput->setString(std::to_string(color.g).c_str());
        m_bInput->setString(std::to_string(color.b).c_str());
        
        m_colorSelector->setColor(color);
    }


    if (ColorNode* point = m_pointsLayer->getSelectedPoint())
        point->setColor(color);

    save();
    updateGradient();
}

void GradientLayer::keyDown(cocos2d::enumKeyCodes key) {
	if (key == cocos2d::enumKeyCodes::KEY_Escape)
		return onClose(nullptr);

    if (Mod::get()->getSettingValue<bool>("disable-keys")) return;

    if (key == cocos2d::enumKeyCodes::KEY_Backspace)
        return onRemovePoint(nullptr);

    if (key == cocos2d::enumKeyCodes::KEY_Enter)
        return onAddPoint(nullptr);

    if (key == cocos2d::enumKeyCodes::KEY_One)
        return onColorToggle(m_mainColorToggle);

    if (key == cocos2d::enumKeyCodes::KEY_Two)
        return onColorToggle(m_secondaryColorToggle);

    if (key == cocos2d::enumKeyCodes::KEY_C)
        return onCopy(nullptr);

    if (key == cocos2d::enumKeyCodes::KEY_V)
        return onPaste(nullptr);

    if (key == cocos2d::enumKeyCodes::KEY_S)
        return onSave(nullptr);

    if (key == cocos2d::enumKeyCodes::KEY_O)
        return onLoad(nullptr);

    if (key == cocos2d::enumKeyCodes::KEY_L) {
        Loader::get()->queueInMainThread([this] {
            m_dotToggle->toggle(!m_dotToggle->isToggled());
        });
        
        return onLockToggle(nullptr);
    }

    cocos2d::CCPoint move = {0, 0};
    float amount = Mod::get()->getSettingValue<float>("move-step");

    switch (key) {
        case cocos2d::enumKeyCodes::KEY_Up: move = ccp(0, amount); break;
        case cocos2d::enumKeyCodes::KEY_Down: move = ccp(0, -amount); break;
        case cocos2d::enumKeyCodes::KEY_Right: move = ccp(amount, 0); break;
        case cocos2d::enumKeyCodes::KEY_Left: move = ccp(-amount, 0); break;
        default: return;
    }

    m_pointsLayer->moveSelected(move);

	return FLAlertLayer::keyDown(key);
}

void GradientLayer::scrollWheel(float y, float) {
    if (m_buttons.empty() || Mod::get()->getSettingValue<bool>("disable-keys")) return;
    
    m_scroll = m_smoothScroll ? m_scroll + y : y;
    
    if (m_scroll < 12 && m_scroll > -12 && m_smoothScroll) return;
    
    int index = 0;

    for (IconButton* button : m_buttons) {
        if (button == m_selectedButton)
            break;
     
        index++;
    }
    
    index += m_scroll > 0 ? 1 : -1;
    
    m_scroll = 0;

    if (index >= static_cast<int>(m_buttons.size())) index = 0;
    if (index < 0) index = static_cast<int>(m_buttons.size()) - 1;

    onIconButton(m_buttons[index]);
} 

bool GradientLayer::setup() {
    geode::DispatchEvent<CCNode*, CCRect>("timestepyt.gdneko/create-neko-rect", m_mainLayer, {178.5f, 75, 259, 126}).post();
    
    setMouseEnabled(true);

    m_smoothScroll = Loader::get()->isModLoaded("prevter.smooth-scroll");

    CCScene* scene = CCDirector::get()->getRunningScene();
        
    if (GJGarageLayer* garage = scene->getChildByType<GJGarageLayer>(0))
        m_garage = static_cast<ProGJGarageLayer*>(garage);

    m_closeBtn->setPosition(m_closeBtn->getPosition() + ccp(-3, 3));
    m_closeBtn->getChildByType<CCSprite>(0)->setScale(0.6f);
    
    if (!Mod::get()->getSavedValue<bool>("moved-point")) {
        m_movePointsSprite = CCSprite::create("move_points.png"_spr);
        m_movePointsSprite->setPosition({124, 240});
        m_movePointsSprite->setScale(0.7f);
        m_mainLayer->addChild(m_movePointsSprite);
    }
    
    if (!Mod::get()->getSavedValue<bool>("added-color")) {
        m_addColorsSprite = CCSprite::create("add_colors.png"_spr);
        m_addColorsSprite->setPosition({105, 146});
        m_addColorsSprite->setScale(0.7f);
        m_mainLayer->addChild(m_addColorsSprite);
    }

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

    m_pointsLayer = PointsLayer::create(m_size, this);
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

    m_hideToggle = CCMenuItemToggler::create(
        CCSprite::createWithSpriteFrameName("hideBtn_001.png"),
        CCSprite::create("show.png"_spr),
        this,
        menu_selector(GradientLayer::onHideToggle)
    );
    m_hideToggle->setPosition({304, 147});
    m_hideToggle->setScale(0.55f);
    m_hideToggle->setCascadeOpacityEnabled(true);
    m_hideToggle->setOpacity(140);

    m_buttonMenu->addChild(m_hideToggle);

    spr = CCSprite::create("copy.png"_spr);
    spr->setScale(0.44f);
    spr->setOpacity(140);

    m_copyButton = CCMenuItemSpriteExtra::create(spr, this, menu_selector(GradientLayer::onCopy));
    m_copyButton->setPosition({229, 147});
    m_copyButton->setCascadeOpacityEnabled(true);

    m_buttonMenu->addChild(m_copyButton);
    
    spr = CCSprite::create("paste.png"_spr);
    spr->setScale(0.43f);
    spr->setOpacity(140);

    m_pasteButton = CCMenuItemSpriteExtra::create(spr, this, menu_selector(GradientLayer::onPaste));
    m_pasteButton->setPosition({247, 147});
    m_copyButton->setCascadeOpacityEnabled(true);

    m_buttonMenu->addChild(m_pasteButton);

    spr = CCSprite::create("save(1).png"_spr);
    spr->setScale(0.4f);
    spr->setOpacity(140);

    m_saveButton = CCMenuItemSpriteExtra::create(spr, this, menu_selector(GradientLayer::onSave));
    m_saveButton->setPosition({265, 147});
    m_copyButton->setCascadeOpacityEnabled(true);

    m_buttonMenu->addChild(m_saveButton);

    spr = CCSprite::create("load(2)(1).png"_spr);
    spr->setScale(0.45f);
    spr->setOpacity(140);

    m_loadButton = CCMenuItemSpriteExtra::create(spr, this, menu_selector(GradientLayer::onLoad));
    m_loadButton->setPosition({284, 147});
    m_loadButton->setCascadeOpacityEnabled(true);

    m_buttonMenu->addChild(m_loadButton);

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

    m_countLabel = CCLabelBMFont::create("3/24", "chatFont.fnt");
    m_countLabel->setOpacity(0);
    m_countLabel->setScale(0.35f);
    m_countLabel->setPosition({77, 144});

    m_mainLayer->addChild(m_countLabel);

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

    m_mainColorToggle->applyGradient(Utils::getDefaultConfig(false), true, true);
    m_secondaryColorToggle->applyGradient(Utils::getDefaultConfig(true), true, true);

    m_colorSelector = ColorToggle::create(this, menu_selector(GradientLayer::onColorSelector), false, false);
    m_colorSelector->setPosition({65, 28});

    m_buttonMenu->addChild(m_colorSelector);

    for (IconButton* button : m_buttons)
        button->setLocked(Mod::get()->hasSavedValue(Utils::getTypeID(button->getType())), true);

    m_mainColorToggle->setSelected(true);

    load(IconType::Cube, false, true, true);

    if (Cache::getLastSelected() != IconType::Cube)
        Loader::get()->queueInMainThread([this] {
            for (IconButton* button : m_buttons)
                if (button->getType() == Cache::getLastSelected())
                    onIconButton(button);
        });
    
    Loader::get()->queueInMainThread([this] {
        m_pointsLayer->selectLast();
        
        if (!Mod::get()->getSavedValue<bool>("moved-point")) {
            m_firstPoint = m_pointsLayer->getSelectedPoint();
            m_firstPosition = m_firstPoint->getPosition();
        }
    });

    return true;
}
