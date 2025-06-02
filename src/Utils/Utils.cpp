#include "Utils.hpp"
#include "Cache.hpp"

#include "../Hooks/SimplePlayer.hpp"

SimplePlayer* Utils::createIcon(IconType type) {
    SimplePlayer* icon = SimplePlayer::create(1);

    icon->updatePlayerFrame(getIconID(type), type);

    return icon;
}

CCMenuItemToggler* Utils::createTypeToggle(bool radial, cocos2d::CCPoint pos, CCObject* target, cocos2d::SEL_MenuHandler callback) {
    CCSprite* spr = CCSprite::create("GJ_button_04.png");

    CCSprite* spr2 = CCSprite::createWithSpriteFrameName(radial ? "edit_areaModeBtn04_001.png" : "edit_areaModeBtn03_001.png");
    spr2->setScale(1.6f);
    spr2->setPosition(spr->getContentSize() / 2.f);

    spr->addChild(spr2);

    CCSprite* spr3 = CCSprite::create("GJ_button_02.png");

    CCSprite* spr4 = CCSprite::createWithSpriteFrameName(radial ? "edit_areaModeBtn04_001.png" : "edit_areaModeBtn03_001.png");
    spr4->setScale(1.6f);
    spr4->setPosition(spr3->getContentSize() / 2.f);

    spr3->addChild(spr4);

    CCMenuItemToggler* toggle = CCMenuItemToggler::create(spr, spr3, target, callback);
    toggle->setScale(0.475f);
    toggle->setPosition(pos);

    return toggle;
}

int Utils::getIconID(IconType type) {
    GameManager* gm = GameManager::get();

    switch (type) {
        case IconType::Cube: return gm->getPlayerFrame();
        case IconType::Ship: return gm->getPlayerShip();
        case IconType::Ball: return gm->getPlayerBall();
        case IconType::Ufo: return gm->getPlayerBird();
        case IconType::Wave: return gm->getPlayerDart();
        case IconType::Robot: return gm->getPlayerRobot();
        case IconType::Spider: return gm->getPlayerSpider();
        case IconType::Swing: return gm->getPlayerSwing();
        case IconType::Jetpack: return gm->getPlayerJetpack();
        default: return gm->getPlayerFrame();
    }
}

bool Utils::isGradientSaved(GradientConfig config) {
    for (const matjson::Value& obj : Mod::get()->getSavedValue<matjson::Value>("saved-gradients"))
        if (configFromObject(obj) == config)
            return true;

    return false;
}

bool Utils::isSettingEnabled(int setting) {
    switch (setting) {
        case MOD_DISABLED: return Cache::isModDisabled();
        case P2_DISABLED: return Cache::is2PDisabled();
        case P2_FLIP: return Cache::is2PFlip();
        case MENU_GRADIENTS: return Cache::isMenuGradientsEnabled();
    }

    return false;
}

GradientConfig Utils::getDefaultConfig(bool secondary) {
    GameManager* gm = GameManager::get();
    int color = secondary ? gm->getPlayerColor2() : gm->getPlayerColor();
    
    return {
        {
            {{0.5f, 1.1f}, gm->colorForIdx(color)},
            {{0.5f, -0.1f}, gm->colorForIdx(color)}
        },
        true
    };
}

matjson::Value Utils::getSaveObject(GradientConfig config) {
    matjson::Value ret = matjson::Value{};
    matjson::Value pointsObject = matjson::Value::array();

    for (SimplePoint point : config.points) {
        matjson::Value object = matjson::Value{};

        object["pos"]["x"] = point.pos.x;
        object["pos"]["y"] = point.pos.y;

        object["color"]["r"] = point.color.r;
        object["color"]["g"] = point.color.g;
        object["color"]["b"] = point.color.b;

        pointsObject.push(object);
    }

    ret["points"] = pointsObject;
    ret["linear"] = config.isLinear;

    return ret;
}

void Utils::removeSavedGradient(GradientConfig config) {
    matjson::Value newArray = matjson::Value::array();

    for (const matjson::Value& obj : Mod::get()->getSavedValue<matjson::Value>("saved-gradients"))
        if (configFromObject(obj) != config)
            newArray.push(obj);
    
    Mod::get()->setSavedValue("saved-gradients", newArray);
}

void Utils::saveConfig(GradientConfig config, const std::string& id, const std::string& secondId) {
    matjson::Value container = Mod::get()->getSavedValue<matjson::Value>(id);

    if (secondId.empty()) {
        if (!container.isArray()) 
            container = matjson::Value::array();

        container.push(getSaveObject(config));
    } else
        container[secondId] = getSaveObject(config);

    Mod::get()->setSavedValue(id, container);
}

GradientConfig Utils::configFromObject(const matjson::Value& object) {
    GradientConfig config;

    config.isLinear = object["linear"].asBool().unwrapOr(true);

    for (const matjson::Value& point : object["points"])
        config.points.push_back({
            ccp(
                point["pos"]["x"].asDouble().unwrapOr(0.0), 
                point["pos"]["y"].asDouble().unwrapOr(0.0)
            ),
            ccc3(
                point["color"]["r"].asInt().unwrapOr(0),
                point["color"]["g"].asInt().unwrapOr(0),
                point["color"]["b"].asInt().unwrapOr(0)
            )
        });

    return config;
}

GradientConfig Utils::getSavedConfig(IconType type, bool secondary) {
    std::string id = getTypeID(type);
    std::string color = secondary ? "color2" : "color1";

    GradientConfig config;

    if (!Mod::get()->hasSavedValue(id)) {
        if (!Mod::get()->hasSavedValue("global"))
            return getDefaultConfig(secondary);
        else
            id = "global";
    }

    matjson::Value jsonConfig = Mod::get()->getSavedValue<matjson::Value>(id);

    if (!jsonConfig.contains(color))
        return getDefaultConfig(secondary);

    config = configFromObject(jsonConfig[color]);

    return config;
}

Gradient Utils::getGradient(IconType type, bool flip) {
    Gradient gradient = {
        getSavedConfig(type, false),
        getSavedConfig(type, true)
    };

    if (flip) {
        GradientConfig tempConfig = gradient.main;
        gradient.main = gradient.secondary;
        gradient.secondary = tempConfig;
    }

    return gradient;
}

void Utils::setIconColors(SimplePlayer* icon, bool secondary, bool white) {
    GameManager* gm = GameManager::get();

    cocos2d::ccColor3B color1 = white ? ccc3(255, 255, 255)
        : gm->colorForIdx(gm->getPlayerColor());

    cocos2d::ccColor3B color2 = white ? ccc3(255, 255, 255)
        : gm->colorForIdx(gm->getPlayerColor2());

    icon->setColor(color1);
    icon->setSecondColor(color2);

    icon->m_hasGlowOutline = gm->getPlayerGlow();;

    if (icon->m_hasGlowOutline)
        icon->enableCustomGlowColor(gm->colorForIdx(gm->getPlayerGlowColor()));
    else
        icon->disableCustomGlowColor();

    icon->updateColors();
}

std::string Utils::getTypeID(IconType type) {
    switch (type) {
        case IconType::Cube: return "cube";
        case IconType::Ship: return "ship";
        case IconType::Ball: return "ball";
        case IconType::Ufo: return "ufo";
        case IconType::Wave: return "wave";
        case IconType::Robot: return "robot";
        case IconType::Spider: return "spider";
        case IconType::Swing: return "swing";
        case IconType::Jetpack: return "jetpack";
        default: return "global";
    }
}

std::string Utils::getTypeID(SpriteType type) {
    switch (type) {
        case SpriteType::Icon: return "icon";
        case SpriteType::Vehicle: return "vehicle";
        case SpriteType::Animation: return "animation";
        default: return "icon";
    }
}

IconType Utils::getIconType(SimplePlayer* icon) {
    return static_cast<ProSimplePlayer*>(icon)->m_fields->m_type; 
}

std::vector<GradientConfig> Utils::getSavedGradients() {
    std::vector<GradientConfig> ret;

    for (const matjson::Value obj : Mod::get()->getSavedValue<matjson::Value>("saved-gradients"))
        ret.push_back(configFromObject(obj));

    return ret;
}

void Utils::applyGradient(SimplePlayer* icon, GradientConfig config, bool secondary, bool force, bool blend) {
    GJRobotSprite* otherSprite = nullptr;

    if (icon->m_robotSprite) if (icon->m_robotSprite->isVisible()) otherSprite = icon->m_robotSprite;
    if (icon->m_spiderSprite) if (icon->m_spiderSprite->isVisible()) otherSprite = icon->m_spiderSprite;

    if (otherSprite) {
        if (secondary)
            for (CCSprite* spr : CCArrayExt<CCSprite*>(otherSprite->m_secondArray)) {
                if (!typeinfo_cast<CCSprite*>(spr) || spr == otherSprite->m_headSprite) continue;

                applyGradient(spr, config, force, blend);
            }
        else
            for (CCSpritePart* spr : CCArrayExt<CCSpritePart*>(otherSprite->m_headSprite->getParent()->getChildren())) {
                if (!typeinfo_cast<CCSpritePart*>(spr)) continue;
                applyGradient(spr, config, force, blend);
            }
    } else
        applyGradient(secondary ? icon->m_secondLayer : icon->m_firstLayer, config, force);
}

void Utils::applyGradient(CCSprite* sprite, GradientConfig config, bool force, bool blend) {
    if (!sprite) return;
    
    CCGLProgram* defaultProgram = CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColor);

    if (config.points.empty())
        return sprite->setShaderProgram(defaultProgram);

    CCGLProgram* program = sprite->getShaderProgram();
     
    if (program == defaultProgram || force || blend) {
        std::string vertPath = (Mod::get()->getResourcesDir() / "position.vert").string();
        std::string shaderPath = (Mod::get()->getResourcesDir() / fmt::format("{}_gradient{}.fsh", config.isLinear ? "linear" : "radial", blend ? "_blend" : "")).string();

        if (!std::filesystem::exists(vertPath) || !std::filesystem::exists(shaderPath))
            return;

        program = new CCGLProgram();
        program->autorelease();

        program->initWithVertexShaderFilename(vertPath.c_str(), shaderPath.c_str());

        program->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
        program->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
        program->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);

        program->link();
        program->updateUniforms();

        sprite->setShaderProgram(program);
    }

    program->use();
    program->setUniformsForBuiltins();

    CCSpriteFrame* frame = sprite->displayFrame();
    CCRect rectInPixels = frame->getRectInPixels();
    CCSize texSize = frame->getTexture()->getContentSizeInPixels();

    bool rot = frame->m_bRotated;

    float uMin = rectInPixels.origin.x / texSize.width;
    float vMin = rectInPixels.origin.y / texSize.height;
    float uMax = (rectInPixels.origin.x + rectInPixels.size.width) / texSize.width;
    float vMax = (rectInPixels.origin.y + rectInPixels.size.height) / texSize.height;

    GLint locMin = glGetUniformLocation(program->getProgram(), "uvMin");
    GLint locMax = glGetUniformLocation(program->getProgram(), "uvMax");
    glUniform2f(locMin, uMin, vMin);
    glUniform2f(locMax, uMax, vMax);

    int stopAt = config.points.size();

    std::vector<cocos2d::ccColor4F> colors;

    for (const SimplePoint& point : config.points)
        colors.push_back(ccc4FFromccc3B(point.color));

    if (config.isLinear) {
        cocos2d::CCPoint startPoint = ccp(0, 0);
        cocos2d::CCPoint endPoint = ccp(0, 0);

        float distance = 0.f;

        for (const SimplePoint& point : config.points) {
            float currentDistance = ccpDistance(point.pos, {0.5f, 0.5f});
            if (currentDistance > distance) {
                startPoint = point.pos;
                distance = currentDistance;
            }
        }

        distance = 0.f;

        for (const SimplePoint& point : config.points) {
            float currentDistance = ccpDistance(point.pos, startPoint);
            if (currentDistance > distance) {
                endPoint = point.pos;
                distance = currentDistance;
            }
        }

        std::vector<float> stops;
        distance = ccpDistance(startPoint, endPoint);

        for (const SimplePoint& point : config.points)
            stops.push_back(ccpDistance(point.pos, startPoint) / distance);

        for (size_t i = 0; i < stops.size(); ++i)
            for (size_t j = i + 1; j < stops.size(); ++j)
                if (stops[i] > stops[j]) {
                    std::swap(stops[i], stops[j]);
                    std::swap(colors[i], colors[j]);
                }

        GLint startPointLoc = glGetUniformLocation(program->getProgram(), "startPoint");
        GLint endPointLoc   = glGetUniformLocation(program->getProgram(), "endPoint");

        glUniform2f(startPointLoc, rot ? startPoint.y : startPoint.x, rot ? startPoint.x : (1 - startPoint.y));
        glUniform2f(endPointLoc, rot ? endPoint.y : endPoint.x, rot ? endPoint.x : (1 - endPoint.y));

        GLint stopsLoc = glGetUniformLocation(program->getProgram(), "stops");
        glUniform1fv(stopsLoc, stopAt, stops.data());
    } else {
        std::vector<float> positions;

        for (const SimplePoint& point : config.points) {
            if (rot) {
                positions.push_back(point.pos.y);
                positions.push_back(point.pos.x);
            } else {
                positions.push_back(point.pos.x);
                positions.push_back(1 - point.pos.y);
            }
        }

        GLint loc = glGetUniformLocation(program->getProgram(), "positions");
        glUniform2fv(loc, stopAt, positions.data());
    }

    GLint stopAtLoc = glGetUniformLocation(program->getProgram(), "stopAt");
    glUniform1i(stopAtLoc, stopAt);

    std::vector<GLfloat> colorsData;
    for (const cocos2d::ccColor4F& color : colors) {
        colorsData.push_back(color.r);
        colorsData.push_back(color.g);
        colorsData.push_back(color.b);
        colorsData.push_back(color.a);
    }

    GLint colorsLoc = glGetUniformLocation(program->getProgram(), "colors");
    glUniform4fv(colorsLoc, stopAt, colorsData.data());
}

void Utils::patchBatchNode(CCSpriteBatchNode* node) {
    if (!node) return;
    
    static void* vtable = []() -> void* {
        FakeSpriteBatchNode temp;
        return *(void**)&temp;
    }();

    *(void**)node = vtable;
}