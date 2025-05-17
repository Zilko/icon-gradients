#include "includes.hpp"

class Utils {

public:

    static SimplePlayer* createIcon(IconType);

    static CCMenuItemToggler* createTypeToggle(bool, cocos2d::CCPoint, CCObject*, cocos2d::SEL_MenuHandler);

    static int getIconID(IconType);

    static void setIconColors(SimplePlayer*, bool);
    
    static GradientConfig getSavedConfig(IconType, bool);

    static GradientConfig getDefaultConfig(bool);

    static std::string getTypeID(IconType);

    static void applyGradient(SimplePlayer*, GradientConfig);

    static void applyGradient(CCSprite*, GradientConfig);

};