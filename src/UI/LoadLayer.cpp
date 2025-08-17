#include "../Utils/Utils.hpp"

#include "LoadLayer.hpp"
#include "GradientLayer.hpp"

LoadLayer* LoadLayer::create(GradientLayer* layer) {
    LoadLayer* ret = new LoadLayer();

    ret->m_layer = layer;

    if (ret->initAnchored(246, 233)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

void LoadLayer::updateGradient(float dt) {
    if (m_updatedIndex >= m_toggles.size())
        return unscheduleAllSelectors();

    for (int i = 0; i < 10; i++) {
        if (m_updatedIndex >= m_toggles.size())
            return unscheduleAllSelectors();

        ColorToggle* toggle = m_toggles[m_updatedIndex];

        if (m_toggleGradients.contains(toggle))
            toggle->applyGradient(m_toggleGradients.at(toggle), false, false);

        m_updatedIndex++;
    }
}

void LoadLayer::onSelect(CCObject* sender) {
    ColorToggle* toggle = static_cast<ColorToggle*>(sender);

    if (toggle == m_selected) return;

    if (m_selected)
        m_selected->setSelected(false);

    toggle->setSelected(true);

    m_selected = toggle;
}

void LoadLayer::onLoad(CCObject*) {
    if (m_toggleGradients.contains(m_selected) && m_selected)
        m_layer->load(m_toggleGradients.at(m_selected));

    onClose(nullptr);
    
    Notification::create("Gradient Loaded", NotificationIcon::Success, 0.1f)->show();
}

void LoadLayer::onDelete(CCObject*) {
    if (!m_toggleGradients.contains(m_selected)) return;

    Utils::removeSavedGradient(m_toggleGradients.at(m_selected));

    m_selected->getParent()->removeFromParent();
    m_scrollLayer->m_contentLayer->updateLayout();

    m_toggles.erase(std::remove(m_toggles.begin(), m_toggles.end(), m_selected), m_toggles.end());
    m_selected = nullptr;

    if (!m_toggles.empty())
        onSelect(m_toggles.front());
    else {
        // lazy ass

        onClose(nullptr);

        LoadLayer* newLayer = create(m_layer);
        newLayer->m_noElasticity = true;

        newLayer->show();
    }
}

bool LoadLayer::setup() {
    std::vector<GradientConfig> gradients = Utils::getSavedGradients();

    setTitle("Load Gradient");

    CCScale9Sprite* bg = CCScale9Sprite::create("square02b_001.png");
	bg->setColor({0, 0, 0});
	bg->setOpacity(49);
	bg->setContentSize({ 204, 159 });

    Border* border = Border::create(bg, {0, 0, 0}, {204, 159}, {0, 0});
    border->setPosition(m_size / 2.f + ccp(0, 1.5f) - bg->getContentSize() / 2.f);

    m_mainLayer->addChild(border);

    ButtonSprite* btnSpr = ButtonSprite::create("Load");
    btnSpr->setScale(0.625f);
    btnSpr->setCascadeOpacityEnabled(true);
    btnSpr->setOpacity(!gradients.empty() ? 255 : 120);

    CCMenuItemSpriteExtra* btn = CCMenuItemSpriteExtra::create(btnSpr, this, menu_selector(LoadLayer::onLoad));
    btn->setPosition({211, 21});
    btn->setCascadeOpacityEnabled(true);
    btn->setEnabled(!gradients.empty());

    m_buttonMenu->addChild(btn);

    btnSpr = ButtonSprite::create("Delete");
    btnSpr->setScale(0.625f);
    btnSpr->setCascadeOpacityEnabled(true);
    btnSpr->setOpacity(!gradients.empty() ? 255 : 120);

    btn = CCMenuItemSpriteExtra::create(btnSpr, this, menu_selector(LoadLayer::onDelete));
    btn->setPosition({141, 21});
    btn->setCascadeOpacityEnabled(true);
    btn->setEnabled(!gradients.empty());
    
    m_buttonMenu->addChild(btn);

    CCLabelBMFont* lbl = CCLabelBMFont::create("No Gradients", "bigFont.fnt");
    lbl->setPosition(border->getPosition() + bg->getContentSize() / 2.f);
    lbl->setScale(0.6f);
    lbl->setOpacity(!gradients.empty() ? 0 : 140);

    m_mainLayer->addChild(lbl);

    m_scrollLayer = ScrollLayer::create({204, 159, 204, 159}, true, true);
    m_scrollLayer->setPosition(border->getPosition());

    m_scrollLayer->m_contentLayer->setLayout(
        RowLayout::create()
            ->setGrowCrossAxis(true)
            ->setAxisAlignment(AxisAlignment::Start)
            ->setGap(1.1f)
    );

    m_mainLayer->addChild(m_scrollLayer);

    cocos2d::CCSize size = {30, 30};
    float scale = 1.1f;

    for (int i = 0; i < gradients.size(); i++) {
        ColorToggle* toggle = ColorToggle::create(this, menu_selector(LoadLayer::onSelect), ColorType::Main, false, scale);
        toggle->setPosition(size * scale * 0.5f);

        if (i < 100)
            toggle->applyGradient(gradients[i], false, false);

        CCMenu* container = CCMenu::create();
        container->addChild(toggle);
        container->setContentSize(size * scale);

        m_scrollLayer->m_contentLayer->addChild(container);

        if (!m_selected) {
            toggle->setSelected(true);
            m_selected = toggle;
        }

        m_toggles.push_back(toggle);
        m_toggleGradients[toggle] = gradients[i];
    }

    m_scrollLayer->m_contentLayer->updateLayout();
    m_scrollLayer->moveToTop();

    if (!gradients.empty()) {
        Scrollbar* scrollbar = Scrollbar::create(m_scrollLayer);
        scrollbar->setPosition({233, m_size.height / 2.f});
        scrollbar->setVisible(!gradients.empty());

        m_mainLayer->addChild(scrollbar);
    }

    if (m_toggles.size() > 100)
        schedule(schedule_selector(LoadLayer::updateGradient), 0, kCCRepeatForever, 0);

    return true;
}
