#include "../Includes.hpp"

class Cache {

protected:

    IconType m_lastSelected = IconType::Cube;

    GradientConfig m_copiedConfig;

    bool m_disabled = false;
    bool m_p2disabled = false;
    bool m_p2flip = false;
    bool m_menuGradients = false;

public:

    static Cache& get();

    static IconType getLastSelected();

    static void setLastSelected(IconType);

    static GradientConfig getCopiedConfig();

    static void setCopiedConfig(GradientConfig);

    static void setModDisabled(bool);

    static bool isModDisabled();

    static void set2PFlip(bool);

    static bool is2PFlip();

    static void set2PDisabled(bool);

    static bool is2PDisabled();

    static void setMenuGradientsEnabled(bool);

    static bool isMenuGradientsEnabled();

};