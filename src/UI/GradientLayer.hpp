#include "../Includes.hpp"

#include "../Hooks/GJGarageLayer.hpp"

#include "PointsLayer.hpp"
#include "ColorPicker.hpp"
#include "ColorToggle.hpp"
#include "IconButton.hpp"
#include "ColorNode.hpp"
#include "PlayerToggle.hpp"

class GradientLayer : public geode::Popup<>, public ColorPickerDelegate, public TextInputDelegate {

private:

    CCSprite* m_movePointsSprite = nullptr;
    CCSprite* m_addColorsSprite = nullptr;

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

    ColorPicker* m_picker = nullptr;

    ColorToggle* m_mainColorToggle = nullptr;
    ColorToggle* m_secondaryColorToggle = nullptr;
    ColorToggle* m_glowColorToggle = nullptr;
    ColorToggle* m_whiteColorToggle = nullptr;
    ColorToggle* m_colorSelector = nullptr;
    
    PlayerToggle* m_playerToggle = nullptr;

    PointsLayer* m_pointsLayer = nullptr;
    
    ColorNode* m_firstPoint = nullptr;

    IconButton* m_selectedButton = nullptr;

    std::vector<IconButton*> m_buttons;

    std::unordered_map<IconType, std::vector<CCPoint>> m_iconPoints;

    CCPoint m_firstPosition = ccp(0, 0);

    GradientConfig m_currentConfig;

    ColorType m_currentColor = ColorType::Main;
    
    bool m_isSecondPlayer = false;
    bool m_ignoreColorChange = false;
    bool m_pointsHidden = false;
    bool m_smoothScroll = false;
    bool m_noTransition = false;
    
    float m_scroll = 0.f;

    ~GradientLayer();

    bool setup() override;

    void onTypeToggle(CCObject*);
    void onLockToggle(CCObject*);
    void onColorToggle(CCObject*);
    void onHideToggle(CCObject*);
    void onColorSelector(CCObject*);
    
    void onIconButton(CCObject*);
    void onAddPoint(CCObject*);
    void onRemovePoint(CCObject*);
    void onSettings(CCObject*);
    void onCopy(CCObject*);
    void onPaste(CCObject*);
    void onSave(CCObject*);
    void onLoad(CCObject*);

    void load(IconType, ColorType, bool = false, bool = false, bool = false);
    void save(GradientConfig, ColorType);
    void save();

    void updateGradient(bool = false, bool = false, bool = false);
    void updateCountLabel();
    void updateUI();

    void colorValueChanged(ccColor3B) override;
    void textChanged(CCTextInputNode*) override;
    void keyDown(enumKeyCodes) override;
    void scrollWheel(float, float) override;

public:

    static GradientLayer* create();

    ProGJGarageLayer* getGarage();

    void updateHover();
    void updatePointOpacity(int);
    void updatePointScale(float);
    void updateGarage();
    void updatePlayer(bool);
    void updatePlayerToggle();
    void updateGlowToggle();
    
    bool isSecondPlayer();

    void pointMoved();
    void pointSelected(CCNode*);
    void pointReleased();
    void colorSelected(const ccColor3B&);
    
    void load(GradientConfig);

    void onPlayerToggle(PlayerToggle*);
    
};
