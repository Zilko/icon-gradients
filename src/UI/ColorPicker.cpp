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
    
    if (Loader::get()->isModLoaded("flow.betterpicker")) {
        if (CCMenu* menu = m_picker->getChildByType<CCMenu*>(0))
            menu->setVisible(false);
            
        for (int i = 0; i < 3; i++)
            if (TextInput* input = m_picker->getChildByType<TextInput>(i))
                input->setVisible(false);
        
        for (int i = 0; i < 3; i++)
            if (CCLabelBMFont* lbl = m_picker->getChildByType<CCLabelBMFont>(i))
                lbl->setVisible(false);
    }

    return true;
}

void ColorPicker::setDelegate(ColorPickerDelegate* delegate) {
    m_picker->setDelegate(delegate);
}

void ColorPicker::setColor(const ccColor3B& color) {
    m_picker->setColorValue(color);
    
    if (color.r == color.g && color.r == color.b && color.g == color.b) {
        m_picker->m_hsv.h = 0.f;
        m_picker->m_huePicker->setHue(m_picker->m_hsv.h);
        m_picker->m_colourPicker->updateWithHSV(m_picker->m_hsv);
        m_picker->m_colourPicker->updateDraggerWithHSV(m_picker->m_hsv);
    }
}

const ccColor3B ColorPicker::getColor() {
    return m_picker->m_rgb;
}

void ColorPicker::setEnabled(bool enabled) {
    m_picker->m_huePicker->setEnabled(enabled);
    m_picker->m_colourPicker->setEnabled(enabled);

    for (CCSprite* spr : CCArrayExt<CCSprite*>(m_picker->getChildByType<CCSpriteBatchNode>(0)->getChildren()))
        spr->setOpacity(enabled ? 255 : 100);
}