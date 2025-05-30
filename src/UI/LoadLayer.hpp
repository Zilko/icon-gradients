#include "../Includes.hpp"

#include "ColorToggle.hpp"

class GradientLayer;

class LoadLayer : public geode::Popup<> {

private:

    GradientLayer* m_layer = nullptr;

    ScrollLayer* m_scrollLayer = nullptr;

    ColorToggle* m_selected = nullptr;

    std::vector<ColorToggle*> m_toggles;
    std::unordered_map<ColorToggle*, GradientConfig> m_toggleGradients;
    
    int m_updatedIndex = 100;

    bool setup() override;

    void updateGradient(float);
    void updateUI();

    void onSelect(CCObject*);
    void onLoad(CCObject*);
    void onDelete(CCObject*);

public:

    static LoadLayer* create(GradientLayer*);

};