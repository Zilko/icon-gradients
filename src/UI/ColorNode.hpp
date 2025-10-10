#pragma once

#include "../Includes.hpp"

class ColorNode : public CCNode {

private:

    CCSprite* m_dot = nullptr;
    CCSprite* m_circle = nullptr;
    CCSprite* m_select = nullptr;

    CCPoint m_relativePos = {0, 0};

    ccColor3B m_color = ccc3(255, 255, 255);

    bool m_isHovered = false;
    bool m_isSelected = false;
    bool m_isHidden = false;
    bool m_isAnimating = false;

    int m_opacity = 255;

    bool init(bool);

public:

    static ColorNode* create(bool, int = 255);

    void setColor(const ccColor3B&, float = 0.f);
    void setOpacity(int);
    void setSelected(bool);
    void setHovered(bool);
    void setHidden(bool, float, bool = false);

    ccColor3B getColor();
    CCSprite* getSprite();

    bool isSelected();
    bool isHidden();
    bool isAnimating();

    void flash(float = 0.3f);

    void onAnimationEnded();

};