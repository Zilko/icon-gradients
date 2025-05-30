#include "../Includes.hpp"

#include "../Hooks/GJGarageLayer.hpp"

#include "PointsLayer.hpp"
#include "ColorPicker.hpp"
#include "ColorToggle.hpp"
#include "IconButton.hpp"
#include "ColorNode.hpp"

class GradientLayer : public geode::Popup<>, public ColorPickerDelegate, public TextInputDelegate {

private:

    ColorPicker* m_picker = nullptr;

    TextInput* m_rInput = nullptr;
    TextInput* m_gInput = nullptr;
    TextInput* m_bInput = nullptr;

    CCMenuItemSpriteExtra* m_addButton = nullptr;
    CCMenuItemSpriteExtra* m_removeButton = nullptr;
    CCMenuItemSpriteExtra* m_copyButton = nullptr;
    CCMenuItemSpriteExtra* m_pasteButton = nullptr;
    CCMenuItemSpriteExtra* m_saveButton = nullptr;
    CCMenuItemSpriteExtra* m_loadButton = nullptr;

    CCMenuItemToggler* m_linearToggle = nullptr;
    CCMenuItemToggler* m_radialToggle = nullptr;
    CCMenuItemToggler* m_dotToggle = nullptr;
    CCMenuItemToggler* m_hideToggle = nullptr;

    CCLabelBMFont* m_countLabel = nullptr;

    ProGJGarageLayer* m_garage = nullptr;

    ColorToggle* m_mainColorToggle = nullptr;
    ColorToggle* m_secondaryColorToggle = nullptr;

    PointsLayer* m_pointsLayer = nullptr;

    IconButton* m_selectedButton = nullptr;

    std::vector<IconButton*> m_buttons;

    std::unordered_map<IconType, std::vector<cocos2d::CCPoint>> m_iconPoints;

    cocos2d::CCPoint m_moveOffset = ccp(0, 0);

    GradientConfig m_currentConfig;

    bool m_isSecondaryColor = false;
    bool m_ignoreColorChange = false;
    bool m_pointsHidden = false;

    ~GradientLayer();

    bool setup() override;

    void onTypeToggle(CCObject*);
    void onLockToggle(CCObject*);
    void onColorToggle(CCObject*);
    void onHideToggle(CCObject*);

    void onIconButton(CCObject*);
    void onAddPoint(CCObject*);
    void onRemovePoint(CCObject*);
    void onSettings(CCObject*);
    void onCopy(CCObject*);
    void onPaste(CCObject*);
    void onSave(CCObject*);
    void onLoad(CCObject*);

    void load(IconType, bool, bool = false, bool = false, bool = false);
    void save(GradientConfig, bool);
    void save();

    void updateGradient(bool = false, bool = false, bool = false);
    void updateUI();
    void updateCountLabel();

    void colorValueChanged(cocos2d::ccColor3B) override;
    void textChanged(CCTextInputNode*) override;
    void keyDown(cocos2d::enumKeyCodes) override;
    void scrollWheel(float, float) override;

public:

    static GradientLayer* create();

    ProGJGarageLayer* getGarage();

    void updateHover();
    void updatePointOpacity(int);
    void updatePointScale(float);
    void updateGarage(bool, bool = false);

    void pointMoved();
    void pointSelected(CCNode*);
    void pointReleased();
    
    void load(GradientConfig);

};