#include "../Includes.hpp"

class GradientLayer;

class ColorSelectLayer : public geode::Popup<> {

private:

    GradientLayer* m_layer = nullptr;

    bool setup() override;

    void createButton(int, const cocos2d::CCPoint&);

    void onColor(CCObject*);

public:

    static ColorSelectLayer* create(GradientLayer*);

};