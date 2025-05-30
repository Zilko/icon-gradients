#include "ColorPicker.hpp"

#include "../Utils/Utils.hpp"

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
    return m_picker->m_rgb;
}

void ColorPicker::setEnabled(bool enabled) {
    m_picker->getChildByType<CCControlHuePicker>(0)->setEnabled(enabled);
    m_picker->getChildByType<CCControlSaturationBrightnessPicker>(0)->setEnabled(enabled);
    
    for (CCSprite* spr : CCArrayExt<CCSprite*>(m_picker->getChildByType<CCSpriteBatchNode>(0)->getChildren()))
        spr->setOpacity(enabled ? 255 : 100);
}