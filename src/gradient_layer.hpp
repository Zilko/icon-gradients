#include "includes.hpp"
#include "extra_nodes.hpp"

class GradientLayer : public geode::Popup<>, public ColorPickerDelegate, public TextInputDelegate, public PointsLayerDelegate {

private:

    ColorPicker* m_picker = nullptr;

    TextInput* m_rInput = nullptr;
    TextInput* m_gInput = nullptr;
    TextInput* m_bInput = nullptr;

    CCMenuItemSpriteExtra* m_addButton = nullptr;
    CCMenuItemSpriteExtra* m_removeButton = nullptr;

    CCMenuItemToggler* m_linearToggle = nullptr;
    CCMenuItemToggler* m_radialToggle = nullptr;
    CCMenuItemToggler* m_lockToggle = nullptr;

    ColorToggle* m_mainColorToggle = nullptr;
    ColorToggle* m_secondaryColorToggle = nullptr;

    PointsLayer* m_pointsLayer = nullptr;

    IconButton* m_selectedButton = nullptr;

    std::vector<IconButton*> m_buttons;

    std::unordered_map<IconType, std::vector<cocos2d::CCPoint>> m_iconPoints;

    cocos2d::CCPoint m_moveOffset = ccp(0, 0);

    GradientConfig m_currentConfig;

    bool m_isLinear = true;
    bool m_isSecondaryColor = false;
    bool m_ignoreColorChange = false;

    ~GradientLayer();

    bool setup() override;

    void onIconButton(CCObject*);
    void onTypeToggle(CCObject*);
    void onLockToggle(CCObject*);
    void onColorToggle(CCObject*);
    void onAddPoint(CCObject*);
    void onRemovePoint(CCObject*);

    void load(IconType, bool);
    void save();

    void updateGradient();
    void updateUI();
    void updatePositions();

    void colorValueChanged(cocos2d::ccColor3B) override;
    void textChanged(CCTextInputNode*) override;

public:

    static GradientLayer* create();

    void updateHover();

    void pointMoved() override;
    void pointSelected(CCNode*) override;

};