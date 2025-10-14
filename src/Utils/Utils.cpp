#include "Utils.hpp"
#include "Cache.hpp"

#include "../Hooks/SimplePlayer.hpp"

bool GradientConfig::isEmpty(ColorType colorType, bool secondPlayer) {
    if (points.empty()) return true;
    
    ccColor3B color = Utils::getPlayerColor(colorType, secondPlayer);
    
    for (const SimplePoint& point : points)
        if (point.color != color)
            return false;
    
    return true;
}

SimplePlayer* Utils::createIcon(IconType type, bool secondPlayer) {
    SimplePlayer* icon = SimplePlayer::create(1);

    icon->updatePlayerFrame(getIconID(type, secondPlayer), type);
    icon->disableGlowOutline();

    return icon;
}

CCMenuItemToggler* Utils::createTypeToggle(bool radial, CCPoint pos, CCObject* target, SEL_MenuHandler callback) {
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

ccColor3B Utils::getPlayerColor(ColorType colorType, bool secondPlayer) {
    GameManager* gm = GameManager::get();

    if (colorType == ColorType::White)
        return ccWHITE;
    else if (colorType == ColorType::Line)
        return ccBLACK;

    Mod* sdiMod = Loader::get()->getLoadedMod("weebify.separate_dual_icons");
    int color;
    
    if (secondPlayer && sdiMod && Utils::isSettingEnabled(P2_SEPARATE)) {
        switch (colorType) {
            case ColorType::Glow:
                color = sdiMod->getSavedValue<int>("colorglow", 14);
                break;
            case ColorType::Secondary:
                color = sdiMod->getSavedValue<int>("color2", 14);
                break;
            case ColorType::Main:
            default:
                color = sdiMod->getSavedValue<int>("color1", 13);
        }
        
        return gm->colorForIdx(color);
    }
    
    if (secondPlayer && Utils::isSettingEnabled(P2_FLIP) && colorType != ColorType::Glow)
        colorType = colorType == ColorType::Main ? ColorType::Secondary : ColorType::Main;
    
    switch (colorType) {
        case ColorType::Glow:
            color = gm->getPlayerGlowColor();
            break;
        case ColorType::Secondary:
            color = gm->getPlayerColor2();
            break;
        case ColorType::Main:
        default:
            color = gm->getPlayerColor();
    }
    
    return gm->colorForIdx(color);
}

int Utils::getIconID(IconType type, bool secondPlayer) {
    GameManager* gm = GameManager::get();

    if (!(secondPlayer && Loader::get()->isModLoaded("weebify.separate_dual_icons")))
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
    else {
        Mod* sdiMod =  Loader::get()->getLoadedMod("weebify.separate_dual_icons");
        switch (type) {
            case IconType::Cube: return sdiMod->getSavedValue<int>("cube", 1);
            case IconType::Ship: return sdiMod->getSavedValue<int>("ship", 1);
            case IconType::Ball: return sdiMod->getSavedValue<int>("roll", 1);
            case IconType::Ufo: return sdiMod->getSavedValue<int>("bird", 1);
            case IconType::Wave: return sdiMod->getSavedValue<int>("dart", 1);
            case IconType::Robot: return sdiMod->getSavedValue<int>("robot", 1);
            case IconType::Spider: return sdiMod->getSavedValue<int>("spider", 1);
            case IconType::Swing: return sdiMod->getSavedValue<int>("swing", 1);
            case IconType::Jetpack: return sdiMod->getSavedValue<int>("jetpack", 1);
            default: return sdiMod->getSavedValue<int>("cube", 1);
        }
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
        case P2_SEPARATE: return Cache::is2PSeparate();
    }

    return false;
}

GradientConfig Utils::getDefaultConfig(ColorType colorType, bool secondPlayer) {
    ccColor3B color = getPlayerColor(colorType, secondPlayer);

    return GradientConfig{
        {
            {{0.5f, 1.1f}, color},
            {{0.5f, -0.1f}, color}
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

GradientConfig Utils::getSavedConfig(IconType type, ColorType colorType, bool secondPlayer) {
    std::string id = getTypeID(type);
    std::string color = "color" + std::to_string(colorType);

    if (!Utils::isSettingEnabled(P2_SEPARATE))
        secondPlayer = false;

    if (secondPlayer)
        id += "-p2";

    GradientConfig config;

    if (!Mod::get()->hasSavedValue(id)) {
        std::string globalKey = secondPlayer ? "global-p2" : "global";

        if (!Mod::get()->hasSavedValue(globalKey)) {
            return getDefaultConfig(colorType, secondPlayer);
        } else
            id = globalKey;
    }

    matjson::Value jsonConfig = Mod::get()->getSavedValue<matjson::Value>(id);

    if (!jsonConfig.contains(color)) {
        return getDefaultConfig(colorType, secondPlayer);
    }

    config = configFromObject(jsonConfig[color]);

    return config;
}

Gradient Utils::getGradient(IconType type, bool secondPlayer) {
    Gradient gradient = {
        getSavedConfig(type, ColorType::Main, secondPlayer),
        getSavedConfig(type, ColorType::Secondary, secondPlayer),
        getSavedConfig(type, ColorType::Glow, secondPlayer),
        getSavedConfig(type, ColorType::White, secondPlayer),
        getSavedConfig(type, ColorType::Line, secondPlayer)
    };

    if (
        secondPlayer
        && Utils::isSettingEnabled(P2_FLIP)
        && !Utils::isSettingEnabled(P2_SEPARATE)
    ) {
        GradientConfig tempConfig = gradient.main;
        gradient.main = gradient.secondary;
        gradient.secondary = tempConfig;
    }

    return gradient;
}

void Utils::setIconColors(SimplePlayer* icon, ColorType colorType, bool white, bool secondPlayer) {
    GameManager* gm = GameManager::get();
    Mod* sdiMod = Loader::get()->getLoadedMod("weebify.separate_dual_icons");

    ccColor3B color1 = white ? ccc3(255, 255, 255)
        : getPlayerColor(ColorType::Main, secondPlayer);

    ccColor3B color2 = white ? ccc3(255, 255, 255)
        : getPlayerColor(ColorType::Secondary, secondPlayer);
    
    bool hasGlowOutline = gm->getPlayerGlow();
    
    if (secondPlayer && sdiMod && Utils::isSettingEnabled(P2_SEPARATE))
        hasGlowOutline = sdiMod->getSavedValue<bool>("glow", false);
            
    ccColor3B colorGlow = white ? ccc3(255, 255, 255)
        : getPlayerColor(ColorType::Glow, secondPlayer);

    icon->setColor(color1);
    icon->setSecondColor(color2);

    icon->m_hasGlowOutline = hasGlowOutline;

    if (icon->m_hasGlowOutline)
        icon->enableCustomGlowColor(colorGlow);
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

void Utils::applyGradient(SimplePlayer* icon, Gradient gradient, bool blend, bool secondPlayer, int extra) {
    applyGradient(icon, gradient.main, ColorType::Main, blend, secondPlayer, extra);
    applyGradient(icon, gradient.secondary, ColorType::Secondary, blend, secondPlayer, extra);
    applyGradient(icon, gradient.glow, ColorType::Glow, blend, secondPlayer, extra);
    applyGradient(icon, gradient.white, ColorType::White, blend, secondPlayer, extra);
    applyGradient(icon, gradient.line, ColorType::Line, blend, secondPlayer, extra);
}
    
void Utils::applyGradient(SimplePlayer* icon, GradientConfig config, ColorType colorType, bool blend, bool secondPlayer, int extra) {
    GJRobotSprite* otherSprite = nullptr;
    IconType iconType = getIconType(icon);

    if (icon->m_robotSprite) if (icon->m_robotSprite->isVisible()) otherSprite = icon->m_robotSprite;
    if (icon->m_spiderSprite) if (icon->m_spiderSprite->isVisible()) otherSprite = icon->m_spiderSprite;

    if (otherSprite) {
        switch (colorType) {
            case ColorType::Main: {
                int id = 100;
                
                for (CCSpritePart* spr : CCArrayExt<CCSpritePart*>(otherSprite->m_headSprite->getParent()->getChildren())) {
                    if (!typeinfo_cast<CCSpritePart*>(spr)) continue;
                    
                    id++;
                    
                    applyGradient(spr, config, iconType, colorType, id, blend, secondPlayer, false, extra);
                }
                
                break;
            }
            case ColorType::Secondary: {
                int id = 200;
            
                for (CCSprite* spr : CCArrayExt<CCSprite*>(otherSprite->m_secondArray)) {
                    if (!typeinfo_cast<CCSprite*>(spr) || spr == otherSprite->m_headSprite) continue;

                    id++;
                    
                    applyGradient(spr, config, iconType, colorType, id, blend, secondPlayer, false, extra);
                }
                
                break;
            }
            case ColorType::Glow: {
                int id = 300;
                
                for (CCSprite* spr : CCArrayExt<CCSprite*>(otherSprite->m_glowSprite->getChildren())) {
                    id++;
                    
                    applyGradient(spr, config, iconType, colorType, id, blend, secondPlayer, false, extra);
                }

                break;
            }
            case ColorType::White: {
                applyGradient(otherSprite->m_extraSprite, config, iconType, colorType, 400, blend, secondPlayer, false, extra);
                otherSprite->m_extraSprite->setZOrder(2);
                break;
            }
            case ColorType::Line: {
                int id = 500;
                for (CCSpritePart* spr : CCArrayExt<CCSpritePart*>(otherSprite->m_headSprite->getParent()->getChildren())) {
                    if (!typeinfo_cast<CCSpritePart*>(spr)) continue;

                    id++;

                    CCSprite* lineSprite;
                    if ((lineSprite = typeinfo_cast<CCSprite*>(spr->getChildByID("gradient-line"_spr)))) {
                        lineSprite->setDisplayFrame(spr->displayFrame());
                    } else {
                        lineSprite = CCSprite::createWithSpriteFrame(spr->displayFrame());
                        lineSprite->setID("gradient-line"_spr);

                        spr->addChild(lineSprite, 1);
                    }

                    lineSprite->setContentSize(spr->getContentSize());
                    lineSprite->setPosition(spr->getContentSize() / 2.f);

                    lineSprite->setVisible(!config.isEmpty(ColorType::Line, secondPlayer));

                    applyGradient(lineSprite, config, iconType, colorType, id, blend, secondPlayer, false, extra, true);
                }
                id = 600;
                for (CCSprite* spr : CCArrayExt<CCSprite*>(otherSprite->m_secondArray)) {
                    if (!typeinfo_cast<CCSprite*>(spr) || spr == otherSprite->m_headSprite) continue;

                    id++;

                    CCSprite* lineSprite;
                    if ((lineSprite = typeinfo_cast<CCSprite*>(spr->getChildByID("gradient-line2"_spr)))) {
                        lineSprite->setDisplayFrame(spr->displayFrame());
                    } else {
                        lineSprite = CCSprite::createWithSpriteFrame(spr->displayFrame());
                        lineSprite->setID("gradient-line2"_spr);

                        spr->addChild(lineSprite, 1);
                    }

                    lineSprite->setContentSize(spr->getContentSize());
                    lineSprite->setPosition(spr->getContentSize()/2);

                    lineSprite->setVisible(!config.isEmpty(ColorType::Line, secondPlayer));

                    applyGradient(lineSprite, config, iconType, colorType, id, blend, secondPlayer, false, extra, true);
                }
                id = 700;
                CCSprite* lineSprite;
                if ((lineSprite = typeinfo_cast<CCSprite*>(otherSprite->m_extraSprite->getChildByID("gradient-line"_spr)))) {
                    lineSprite->setDisplayFrame(otherSprite->m_extraSprite->displayFrame());
                } else {
                    lineSprite = CCSprite::createWithSpriteFrame(otherSprite->m_extraSprite->displayFrame());
                    lineSprite->setID("gradient-line"_spr);

                    otherSprite->m_extraSprite->addChild(lineSprite, 1);
                }

                lineSprite->setContentSize(otherSprite->m_extraSprite->getContentSize());
                lineSprite->setPosition(otherSprite->m_extraSprite->getContentSize() / 2.f);

                lineSprite->setVisible(!config.isEmpty(ColorType::Line, secondPlayer));

                applyGradient(lineSprite, config, iconType, colorType, id, blend, secondPlayer, false, extra, true);

                break;
            }
        }
        
        return;
    }
    
    CCSprite* sprite = nullptr;
    CCSprite* sprite2 = nullptr;
    CCSprite* sprite3 = nullptr;
    int id = 0;
    int id2 = 0;
    int id3 = 0;

    switch (colorType) {
        case ColorType::Main:
            id = 100;
            sprite = icon->m_firstLayer;
            break;
        case ColorType::Secondary:
            id = 200;
            sprite = icon->m_secondLayer;
            break;
        case ColorType::Glow:
            id = 300;
            sprite = icon->m_outlineSprite;
            break;
        case ColorType::White:
            id = 400;
            sprite = icon->m_detailSprite;
            sprite->setZOrder(2);
            break;
        case ColorType::Line: {
            id = 500;
            id2 = 600;
            id3 = 700;
            if (CCSprite* lineSprite = typeinfo_cast<CCSprite*>(icon->m_firstLayer->getChildByID("gradient-line"_spr))) {
                lineSprite->setDisplayFrame(icon->m_firstLayer->displayFrame());
                sprite = lineSprite;
            } else {
                sprite = CCSprite::createWithSpriteFrame(icon->m_firstLayer->displayFrame());
                sprite->setID("gradient-line"_spr);

                icon->m_firstLayer->addChild(sprite, 1);
                
                if (iconType == IconType::Ball && Loader::get()->isModLoaded("alphalaneous.fine_outline"))
                    sprite->runAction(CCRepeatForever::create(
                        CCSequence::create(
                            CCShow::create(),
                            nullptr
                        )
                    ));
            }
            if (CCSprite* lineSprite = typeinfo_cast<CCSprite*>(icon->m_secondLayer->getChildByID("gradient-line"_spr))) {
                lineSprite->setDisplayFrame(icon->m_secondLayer->displayFrame());
                sprite2 = lineSprite;
            } else {
                sprite2 = CCSprite::createWithSpriteFrame(icon->m_secondLayer->displayFrame());
                sprite2->setID("gradient-line"_spr);

                icon->m_secondLayer->addChild(sprite2, 1);
                
                if (iconType == IconType::Ball && Loader::get()->isModLoaded("alphalaneous.fine_outline"))
                    sprite2->runAction(CCRepeatForever::create(
                        CCSequence::create(
                            CCShow::create(),
                            nullptr
                        )
                    ));
            }
            if (CCSprite* lineSprite = typeinfo_cast<CCSprite*>(icon->m_detailSprite->getChildByID("gradient-line"_spr))) {
                lineSprite->setDisplayFrame(icon->m_detailSprite->displayFrame());
                sprite3 = lineSprite;
            } else {
                sprite3 = CCSprite::createWithSpriteFrame(icon->m_detailSprite->displayFrame());
                sprite3->setID("gradient-line"_spr);
                
                if (iconType == IconType::Ball && Loader::get()->isModLoaded("alphalaneous.fine_outline"))
                    sprite3->runAction(CCRepeatForever::create(
                        CCSequence::create(
                            CCShow::create(),
                            nullptr
                        )
                    ));

                icon->m_detailSprite->addChild(sprite3, 1);
            }

            sprite->setContentSize(icon->m_firstLayer->getContentSize());
            sprite2->setContentSize(icon->m_secondLayer->getContentSize());
            sprite3->setContentSize(icon->m_detailSprite->getContentSize());
            sprite->setPosition(icon->m_firstLayer->getContentSize()/2);
            sprite2->setPosition(icon->m_secondLayer->getContentSize()/2);
            sprite3->setPosition(icon->m_detailSprite->getContentSize() / 2.f);

            sprite->setVisible(!config.isEmpty(ColorType::Line, secondPlayer));
            sprite2->setVisible(!config.isEmpty(ColorType::Line, secondPlayer));
            sprite3->setVisible(!config.isEmpty(ColorType::Line, secondPlayer));

            break;
        }
    }

    applyGradient(sprite, config, iconType, colorType, id, blend, secondPlayer, false, extra, colorType == ColorType::Line);
    
    if (sprite2)
        applyGradient(sprite2, config, iconType, colorType, id2, blend, secondPlayer, false, extra, colorType == ColorType::Line);
    
    if (sprite3)
        applyGradient(sprite3, config, iconType, colorType, id3, blend, secondPlayer, false, extra, colorType == ColorType::Line);
}

CCGLProgram* Utils::createShader(const std::string& key, bool linear, bool blend, bool line) {
    CCShaderCache* cache = CCShaderCache::sharedShaderCache();
    CCGLProgram* program = cache->programForKey(key.c_str());
     
    if (!program || key.empty()) {
        static std::filesystem::path vertPath = Mod::get()->getResourcesDir() / "position.vert";
        std::filesystem::path shaderPath = Mod::get()->getResourcesDir() / fmt::format("{}_gradient{}.fsh", linear ? "linear" : "radial", line ? "_line" : blend ? "_blend" : "");

        if (!std::filesystem::exists(vertPath) || !std::filesystem::exists(shaderPath))
            return nullptr;

        program = new CCGLProgram();
        
        if (!key.empty())
            program->retain();
        else    
            program->autorelease();

        program->initWithVertexShaderFilename(
            string::pathToString(vertPath).c_str(),
            string::pathToString(shaderPath).c_str()
        );

        program->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
        program->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
        program->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);

        program->link();
        program->updateUniforms();

        if (!key.empty())
            cache->addProgram(program, key.c_str());
    }
    
    return program;
}

void Utils::applyGradient(CCSprite* sprite, GradientConfig config, IconType iconType, ColorType colorType, int id, bool blend, bool secondPlayer, bool playerObject, int extra, bool line) {
    if (!sprite) return;
    
    if (config.isEmpty(colorType, secondPlayer))
        return sprite->setShaderProgram(
            CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColor)
        );
    
    CCGLProgram* program = nullptr;
    
    if (extra != -4732) {
        std::string key = fmt::format(
            "{}-{}-{}-{}-{}-{}-{}-{}"_spr,
            config.isLinear,
            static_cast<int>(iconType),
            id,
            blend,
            line,
            secondPlayer,
            playerObject,
            extra
        );
        
        program = createShader(key, config.isLinear, blend, line);
    } else {
        program = createShader("", config.isLinear, blend, line);
    }
    
    if (!program) return;
    
    sprite->setShaderProgram(program);

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
    
    if (colorType == ColorType::Line) {
        GLint locPixelSize = glGetUniformLocation(program->getProgram(), "u_pixelSize");
        glUniform2f(locPixelSize, 1.f / texSize.width, 1.f / texSize.height);
        
        float threshold = -10.f;
    
        switch (GameManager::get()->m_texQuality) {
            case 1: threshold = -1.25f; break;
            case 2: threshold = -2.5f; break;
        };
    
        #ifdef GEODE_IS_MOBILE
    
        if (!Loader::get()->isModLoaded("weebify.high-graphics-android"))
            threshold = -2.5f;
    
        #endif
        
        GLint locThreshold = glGetUniformLocation(program->getProgram(), "u_threshold");
        glUniform1f(locThreshold, threshold);
    }

    std::vector<ccColor4F> colors;
    int stopAt = config.points.size();

    for (const SimplePoint& point : config.points)
        colors.push_back(ccc4FFromccc3B(point.color));

    if (config.isLinear) {
        CCPoint startPoint = ccp(0, 0);
        CCPoint endPoint = ccp(0, 0);

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
    for (const ccColor4F& color : colors) {
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

void Utils::hideSprite(CCSprite* sprite) {
    CCGLProgram* shader = CCShaderCache::sharedShaderCache()->programForKey("invis-shader"_spr);
    
    if (shader)
        return sprite->setShaderProgram(shader);
    
    shader = new CCGLProgram();
    shader->initWithVertexShaderByteArray(
        R"(
            attribute vec4 a_position;
            attribute vec2 a_texCoord;
            attribute vec4 a_color;
            
            #ifdef GL_ES
            varying lowp vec4 v_fragmentColor;
            varying mediump vec2 v_texCoord;
            #else
            varying vec4 v_fragmentColor;
            varying vec2 v_texCoord;
            #endif
            
            void main()
            {
                gl_Position = CC_MVPMatrix * a_position;
                v_fragmentColor = a_color;
                v_texCoord = a_texCoord;
            }
        )",
        R"(
            #ifdef GL_ES
            precision mediump float;
            #endif
    
            void main() {
                gl_FragColor = vec4(0.0);
            }
        )"
    );
    shader->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
    shader->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
    shader->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
    shader->link();
    shader->updateUniforms();
    shader->retain();
    
    CCShaderCache::sharedShaderCache()->addProgram(shader, "invis-shader"_spr);
    
    sprite->setShaderProgram(shader);
}