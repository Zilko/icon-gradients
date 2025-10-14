#include "Cache.hpp"
#include "Utils.hpp"

#include <Geode/loader/Event.hpp>

constexpr static std::array cacheIDs = std::to_array<std::string_view>({
    "{}-5-101-false-{}-false-true-55","{}-5-102-false-{}-false-true-55","{}-5-103-false-{}-false-true-55","{}-5-104-false-{}-false-true-55","{}-5-105-false-{}-false-true-55","{}-5-106-false-{}-false-true-55","{}-5-107-false-{}-false-true-55","{}-5-201-false-{}-false-true-55","{}-5-202-false-{}-false-true-55","{}-5-203-false-{}-false-true-55","{}-5-204-false-{}-false-true-55","{}-5-205-false-{}-false-true-55","{}-5-206-false-{}-false-true-55","{}-5-207-false-{}-false-true-55","{}-5-301-false-{}-false-true-55","{}-5-302-false-{}-false-true-55","{}-5-303-false-{}-false-true-55","{}-5-304-false-{}-false-true-55","{}-5-305-false-{}-false-true-55","{}-5-306-false-{}-false-true-55","{}-5-307-false-{}-false-true-55","{}-5-400-false-{}-false-true-55","{}-5-501-false-{}-false-true-55","{}-5-502-false-{}-false-true-55","{}-5-503-false-{}-false-true-55","{}-5-504-false-{}-false-true-55","{}-5-505-false-{}-false-true-55","{}-5-506-false-{}-false-true-55","{}-5-507-false-{}-false-true-55","{}-5-601-false-{}-false-true-55","{}-5-602-false-{}-false-true-55","{}-5-603-false-{}-false-true-55","{}-5-604-false-{}-false-true-55","{}-5-605-false-{}-false-true-55","{}-5-606-false-{}-false-true-55","{}-5-607-false-{}-false-true-55","{}-5-700-false-{}-false-true-55","{}-6-101-false-{}-false-true-55","{}-6-102-false-{}-false-true-55","{}-6-103-false-{}-false-true-55","{}-6-104-false-{}-false-true-55","{}-6-105-false-{}-false-true-55","{}-6-106-false-{}-false-true-55","{}-6-201-false-{}-false-true-55","{}-6-202-false-{}-false-true-55","{}-6-203-false-{}-false-true-55","{}-6-204-false-{}-false-true-55","{}-6-205-false-{}-false-true-55","{}-6-206-false-{}-false-true-55","{}-6-301-false-{}-false-true-55","{}-6-302-false-{}-false-true-55","{}-6-303-false-{}-false-true-55","{}-6-304-false-{}-false-true-55","{}-6-305-false-{}-false-true-55","{}-6-306-false-{}-false-true-55","{}-6-400-false-{}-false-true-55","{}-6-501-false-{}-false-true-55","{}-6-502-false-{}-false-true-55","{}-6-503-false-{}-false-true-55","{}-6-504-false-{}-false-true-55","{}-6-505-false-{}-false-true-55","{}-6-506-false-{}-false-true-55","{}-6-601-false-{}-false-true-55","{}-6-602-false-{}-false-true-55","{}-6-603-false-{}-false-true-55","{}-6-604-false-{}-false-true-55","{}-6-605-false-{}-false-true-55","{}-6-606-false-{}-false-true-55","{}-6-700-false-{}-false-true-55","{}-0-105-false-{}-false-true-2","{}-0-205-false-{}-false-true-2","{}-0-305-false-{}-false-true-2","{}-0-405-false-{}-false-true-2","{}-0-505-false-{}-false-true-2","{}-0-605-false-{}-false-true-2","{}-0-705-false-{}-false-true-2","{}-5-101-false-{}-true-true-55","{}-5-102-false-{}-true-true-55","{}-5-103-false-{}-true-true-55","{}-5-104-false-{}-true-true-55","{}-5-105-false-{}-true-true-55","{}-5-106-false-{}-true-true-55","{}-5-107-false-{}-true-true-55","{}-5-201-false-{}-true-true-55","{}-5-202-false-{}-true-true-55","{}-5-203-false-{}-true-true-55","{}-5-204-false-{}-true-true-55","{}-5-205-false-{}-true-true-55","{}-5-206-false-{}-true-true-55","{}-5-207-false-{}-true-true-55","{}-5-301-false-{}-true-true-55","{}-5-302-false-{}-true-true-55","{}-5-303-false-{}-true-true-55","{}-5-304-false-{}-true-true-55","{}-5-305-false-{}-true-true-55","{}-5-306-false-{}-true-true-55","{}-5-307-false-{}-true-true-55","{}-5-400-false-{}-true-true-55","{}-5-501-false-{}-true-true-55","{}-5-502-false-{}-true-true-55","{}-5-503-false-{}-true-true-55","{}-5-504-false-{}-true-true-55","{}-5-505-false-{}-true-true-55","{}-5-506-false-{}-true-true-55","{}-5-507-false-{}-true-true-55","{}-5-601-false-{}-true-true-55","{}-5-602-false-{}-true-true-55","{}-5-603-false-{}-true-true-55","{}-5-604-false-{}-true-true-55","{}-5-605-false-{}-true-true-55","{}-5-606-false-{}-true-true-55","{}-5-607-false-{}-true-true-55","{}-5-700-false-{}-true-true-55","{}-6-101-false-{}-true-true-55","{}-6-102-false-{}-true-true-55","{}-6-103-false-{}-true-true-55","{}-6-104-false-{}-true-true-55","{}-6-105-false-{}-true-true-55","{}-6-106-false-{}-true-true-55","{}-6-201-false-{}-true-true-55","{}-6-202-false-{}-true-true-55","{}-6-203-false-{}-true-true-55","{}-6-204-false-{}-true-true-55","{}-6-205-false-{}-true-true-55","{}-6-206-false-{}-true-true-55","{}-6-301-false-{}-true-true-55","{}-6-302-false-{}-true-true-55","{}-6-303-false-{}-true-true-55","{}-6-304-false-{}-true-true-55","{}-6-305-false-{}-true-true-55","{}-6-306-false-{}-true-true-55","{}-6-400-false-{}-true-true-55","{}-6-501-false-{}-true-true-55","{}-6-502-false-{}-true-true-55","{}-6-503-false-{}-true-true-55","{}-6-504-false-{}-true-true-55","{}-6-505-false-{}-true-true-55","{}-6-506-false-{}-true-true-55","{}-6-601-false-{}-true-true-55","{}-6-602-false-{}-true-true-55","{}-6-603-false-{}-true-true-55","{}-6-604-false-{}-true-true-55","{}-6-605-false-{}-true-true-55","{}-6-606-false-{}-true-true-55","{}-6-700-false-{}-true-true-55","{}-0-105-false-{}-true-true-2","{}-0-205-false-{}-true-true-2","{}-0-305-false-{}-true-true-2","{}-0-405-false-{}-true-true-2","{}-0-505-false-{}-true-true-2","{}-0-605-false-{}-true-true-2","{}-0-705-false-{}-true-true-2","{}-7-105-false-{}-false-true-2","{}-7-205-false-{}-false-true-2","{}-7-305-false-{}-false-true-2","{}-7-405-false-{}-false-true-2","{}-7-505-false-{}-false-true-2","{}-7-605-false-{}-false-true-2","{}-7-705-false-{}-false-true-2","{}-7-105-false-{}-true-true-2","{}-7-205-false-{}-true-true-2","{}-7-305-false-{}-true-true-2","{}-7-405-false-{}-true-true-2","{}-7-505-false-{}-true-true-2","{}-7-605-false-{}-true-true-2","{}-7-705-false-{}-true-true-2","{}-2-105-false-{}-true-true-2","{}-2-205-false-{}-true-true-2","{}-2-305-false-{}-true-true-2","{}-2-405-false-{}-true-true-2","{}-2-505-false-{}-true-true-2","{}-2-605-false-{}-true-true-2","{}-2-705-false-{}-true-true-2","{}-1-104-false-{}-false-true-44","{}-1-204-false-{}-false-true-44","{}-1-304-false-{}-false-true-44","{}-1-404-false-{}-false-true-44","{}-1-504-false-{}-false-true-44","{}-1-604-false-{}-false-true-44","{}-1-704-false-{}-false-true-44","{}-1-105-false-{}-false-true-2","{}-1-205-false-{}-false-true-2","{}-1-305-false-{}-false-true-2","{}-1-405-false-{}-false-true-2","{}-1-505-false-{}-false-true-2","{}-1-605-false-{}-false-true-2","{}-1-705-false-{}-false-true-2","{}-2-105-false-{}-false-true-2","{}-2-205-false-{}-false-true-2","{}-2-305-false-{}-false-true-2","{}-2-405-false-{}-false-true-2","{}-2-505-false-{}-false-true-2","{}-2-605-false-{}-false-true-2","{}-2-705-false-{}-false-true-2","{}-1-104-false-{}-true-true-44","{}-1-204-false-{}-true-true-44","{}-1-304-false-{}-true-true-44","{}-1-404-false-{}-true-true-44","{}-1-504-false-{}-true-true-44","{}-1-604-false-{}-true-true-44","{}-1-704-false-{}-true-true-44","{}-1-105-false-{}-true-true-2","{}-1-205-false-{}-true-true-2","{}-1-305-false-{}-true-true-2","{}-1-405-false-{}-true-true-2","{}-1-505-false-{}-true-true-2","{}-1-605-false-{}-true-true-2","{}-1-705-false-{}-true-true-2","{}-4-105-false-{}-false-true-2","{}-4-205-false-{}-false-true-2","{}-4-305-false-{}-false-true-2","{}-4-405-false-{}-false-true-2","{}-4-505-false-{}-false-true-2","{}-4-605-false-{}-false-true-2","{}-4-705-false-{}-false-true-2","{}-3-104-false-{}-true-true-44","{}-3-204-false-{}-true-true-44","{}-3-304-false-{}-true-true-44","{}-3-105-false-{}-true-true-2","{}-3-205-false-{}-true-true-2","{}-3-305-false-{}-true-true-2","{}-3-405-false-{}-true-true-2","{}-3-505-false-{}-true-true-2","{}-3-605-false-{}-true-true-2","{}-3-705-false-{}-true-true-2","{}-4-105-false-{}-true-true-2","{}-4-205-false-{}-true-true-2","{}-4-305-false-{}-true-true-2","{}-4-405-false-{}-true-true-2","{}-4-505-false-{}-true-true-2","{}-4-605-false-{}-true-true-2","{}-4-705-false-{}-true-true-2","{}-3-104-false-{}-false-true-44","{}-3-204-false-{}-false-true-44","{}-3-304-false-{}-false-true-44","{}-3-404-false-{}-false-true-44","{}-3-504-false-{}-false-true-44","{}-3-604-false-{}-false-true-44","{}-3-704-false-{}-false-true-44","{}-3-105-false-{}-false-true-2","{}-3-205-false-{}-false-true-2","{}-3-305-false-{}-false-true-2","{}-3-405-false-{}-false-true-2","{}-3-505-false-{}-false-true-2","{}-3-605-false-{}-false-true-2","{}-3-705-false-{}-false-true-2"
});

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
    
    if (Mod::get()->getSettingValue<bool>("preload-shaders")) {
        Utils::hideSprite(CCSprite::create());
        
        for (const auto& str : cacheIDs) {
            Utils::createShader(fmt::format(fmt::runtime(fmt::format("{}"_spr, str)), true, false), true, false, false);
            Utils::createShader(fmt::format(fmt::runtime(fmt::format("{}"_spr, str)), false, true), false, false, true);
            Utils::createShader(fmt::format(fmt::runtime(fmt::format("{}"_spr, str)), false, false), false, false, false);
            Utils::createShader(fmt::format(fmt::runtime(fmt::format("{}"_spr, str)), true, true), true, false, true);
        }
    }

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