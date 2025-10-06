#include "Cache.hpp"

#include <Geode/loader/Event.hpp>

$on_mod(Loaded) {

    Cache::setModDisabled(Mod::get()->getSettingValue<bool>("disabled"));
    Cache::set2PDisabled(Mod::get()->getSettingValue<bool>("disable-2p"));
    Cache::set2PSeparate(Mod::get()->getSettingValue<bool>("separate-2p"));
    Cache::set2PFlip(Mod::get()->getSettingValue<bool>("flip-2p"));
    Cache::setMenuGradientsEnabled(Mod::get()->getSettingValue<bool>("menu-gradients"));

    listenForSettingChanges("disabled", +[](bool value) {
        Cache::setModDisabled(value);
    });

    listenForSettingChanges("disable-2p", +[](bool value) {
        Cache::set2PDisabled(value);
    });

    listenForSettingChanges("separate-2p", +[](bool value) {
        Cache::set2PSeparate(value);
    });
    
    listenForSettingChanges("flip-2p", +[](bool value) {
        Cache::set2PFlip(value);
    });

    listenForSettingChanges("menu-gradients", +[](bool value) {
        Cache::setMenuGradientsEnabled(value);
    });

}


Cache& Cache::get()  {
    static Cache instance;
    return instance;
}

IconType Cache::getLastSelected() {
    return get().m_lastSelected;
}

void Cache::setLastSelected(IconType type) {
    get().m_lastSelected = type;
}

GradientConfig Cache::getCopiedConfig() {
    return get().m_copiedConfig;
}

void Cache::setCopiedConfig(GradientConfig config) {
    get().m_copiedConfig = config;
}

void Cache::setModDisabled(bool disabled) {
    get().m_disabled = disabled;
}

bool Cache::isModDisabled() {
    return get().m_disabled;
}

void Cache::set2PFlip(bool flip) {
    get().m_p2flip = flip;
}

bool Cache::is2PFlip() {
    return get().m_p2flip;
}

void Cache::set2PSeparate(bool separate) {
    get().m_p2separate = separate && !get().m_disabled;
}

bool Cache::is2PSeparate() {
    return get().m_p2separate;
}

void Cache::set2PDisabled(bool disabled) {
    get().m_p2disabled = disabled;
    get().m_p2separate = Mod::get()->getSettingValue<bool>("separate-2p") && !disabled;
}

bool Cache::is2PDisabled() {
    return get().m_p2disabled;
}

void Cache::setMenuGradientsEnabled(bool enabled) {
    get().m_menuGradients = enabled;
}

bool Cache::isMenuGradientsEnabled() {
    return get().m_menuGradients;
}