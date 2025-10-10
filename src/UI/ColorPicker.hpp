#pragma once

#include "../Includes.hpp"

class ColorPicker : public CCNode {

private:

    CCControlColourPicker* m_picker = nullptr;

    bool init() override;

public:

    static ColorPicker* create();

    void setDelegate(ColorPickerDelegate*);
    void setColor(const ccColor3B&);
    void setEnabled(bool);

    const ccColor3B getColor();

};