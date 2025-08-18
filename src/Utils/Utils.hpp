#include "../Includes.hpp"

class Utils {

public:

    static SimplePlayer* createIcon(IconType);

    static CCMenuItemToggler* createTypeToggle(bool, cocos2d::CCPoint, CCObject*, cocos2d::SEL_MenuHandler);

    static int getIconID(IconType);
    static bool isGradientSaved(GradientConfig);
    static bool isSettingEnabled(int);

    static std::string getTypeID(IconType);
    static std::string getTypeID(SpriteType);

    static IconType getIconType(SimplePlayer*);
    static std::vector<GradientConfig> getSavedGradients();

    static GradientConfig configFromObject(const matjson::Value&);
    static GradientConfig getSavedConfig(IconType, ColorType, bool = false);
    static GradientConfig getDefaultConfig(ColorType);
    static matjson::Value getSaveObject(GradientConfig);

    static Gradient getGradient(IconType, bool);

    static void removeSavedGradient(GradientConfig);
    static void saveConfig(GradientConfig, const std::string&, const std::string&);
    static void setIconColors(SimplePlayer*, ColorType, bool);
    static void applyGradient(SimplePlayer*, GradientConfig, ColorType, bool, bool = false);
    static void applyGradient(CCSprite*, GradientConfig, bool, bool = false);
    static void patchBatchNode(CCSpriteBatchNode*);

};