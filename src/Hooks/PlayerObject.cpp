#include "../Utils/Utils.hpp"

#include "PlayerObject.hpp"

bool ProPlayerObject::shouldReturn(GJBaseGameLayer* bgl, bool ignore) {
    if (m_gameLayer != bgl || !bgl) return true;
    if (this != bgl->m_player1 && this != bgl->m_player2 && !ignore) return true;
    if (Utils::isSettingEnabled(MOD_DISABLED)) return true;
    if (m_isSecondPlayer && Utils::isSettingEnabled(P2_DISABLED)) return true;
    
    return false;
}

IconType ProPlayerObject::getIconType() {
    if (m_isShip) return m_isPlatformer ? IconType::Jetpack : IconType::Ship;
    if (m_isBird) return IconType::Ufo;
    if (m_isBall) return IconType::Ball;
    if (m_isDart) return IconType::Wave;
    if (m_isRobot) return IconType::Robot;
    if (m_isSpider) return IconType::Spider;
    if (m_isSwing) return IconType::Swing;
    return IconType::Cube;
}

void ProPlayerObject::updateCube(float) {
    auto f = m_fields.self();
    
    if (getIconType() == IconType::Cube) {
        f->m_previousType = static_cast<IconType>(-1);
        updateGradient();
    }
}

void ProPlayerObject::updateVisibility() {
    if (shouldReturn(GJBaseGameLayer::get())) return;
        
    auto f = m_fields.self();
    
    if (f->m_iconSprite)
        f->m_iconSprite->setOpacity(m_iconSprite->getOpacity());

    if (f->m_iconSpriteSecondary)
        f->m_iconSpriteSecondary->setOpacity(m_iconSpriteSecondary->getOpacity());

    if (f->m_iconGlow)
        f->m_iconGlow->setOpacity(m_iconGlow->getOpacity());

    if (f->m_iconSpriteWhitener)
        f->m_iconSpriteWhitener->setOpacity(m_iconSpriteWhitener->getOpacity());

    if (f->m_iconSpriteLine)
        f->m_iconSpriteLine->setOpacity(m_iconSprite->getOpacity());

    if (f->m_vehicleSprite)
        f->m_vehicleSprite->setOpacity(m_vehicleSprite->getOpacity());

    if (f->m_vehicleSpriteSecondary)
        f->m_vehicleSpriteSecondary->setOpacity(m_vehicleSpriteSecondary->getOpacity());

    if (f->m_vehicleGlow)
        f->m_vehicleGlow->setOpacity(m_vehicleGlow->getOpacity());

    if (f->m_vehicleSpriteWhitener)
        f->m_vehicleSpriteWhitener->setOpacity(m_vehicleSpriteWhitener->getOpacity());

    if (f->m_vehicleSpriteLine)
        f->m_vehicleSpriteLine->setOpacity(m_vehicleSprite->getOpacity());

    for (CCSprite* sprite : f->m_animSprites)
        if (f->m_animSpriteParents.contains(sprite))
            sprite->setOpacity(f->m_animSpriteParents.at(sprite)->getOpacity());
}

void ProPlayerObject::updateSprite(CCSprite* realSprite, CCSprite*& sprite, SpriteType type, ColorType color) {
    if (sprite) {
        Utils::hideSprite(realSprite);
        return sprite->setDisplayFrame(realSprite->displayFrame());
    }
    
    Utils::hideSprite(realSprite);
    
    sprite = CCSprite::createWithSpriteFrame(realSprite->displayFrame());
    sprite->setID(fmt::format("{}-gradient-{}"_spr, Utils::getTypeID(type), std::to_string(color)).c_str());
    
    if ((color == ColorType::Line || color == ColorType::White) && Loader::get()->isModLoaded("alphalaneous.fine_outline")) {
        Loader::get()->queueInMainThread([child = Ref(sprite), dad = Ref(realSprite)] {
            dad->addChild(child, 10);
        });
    } else
        realSprite->addChild(sprite);   
    
    sprite->setAnchorPoint({0, 0});
}

void ProPlayerObject::updateIconSprite(Gradient gradient, auto f) {
    if (!gradient.main.isEmpty(ColorType::Main, m_isSecondPlayer))
        updateSprite(m_iconSprite, f->m_iconSprite, SpriteType::Icon, ColorType::Main);

    if (!gradient.secondary.isEmpty(ColorType::Secondary, m_isSecondPlayer))
        updateSprite(m_iconSpriteSecondary, f->m_iconSpriteSecondary, SpriteType::Icon, ColorType::Secondary);

    if (!gradient.glow.isEmpty(ColorType::Glow, m_isSecondPlayer))
        updateSprite(m_iconGlow, f->m_iconGlow, SpriteType::Icon, ColorType::Glow);

    if (!gradient.white.isEmpty(ColorType::White, m_isSecondPlayer))
        updateSprite(m_iconSpriteWhitener, f->m_iconSpriteWhitener, SpriteType::Icon, ColorType::White);

    if (!gradient.line.isEmpty(ColorType::Line, m_isSecondPlayer)) {
        updateSprite(m_iconSprite, f->m_iconSpriteLine, SpriteType::Icon, ColorType::Line);
        updateSprite(m_iconSpriteSecondary, f->m_iconSpriteLineSecondary, SpriteType::Icon, ColorType::Line);
        updateSprite(m_iconSpriteWhitener, f->m_iconSpriteLineWhitener, SpriteType::Icon, ColorType::Line);
        
        if (!f->m_iconSprite || !f->m_iconSprite->isVisible())
            m_iconSprite->setShaderProgram(
                CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColor)
            );
        
        if (!f->m_iconSpriteSecondary || !f->m_iconSpriteSecondary->isVisible())
            m_iconSpriteSecondary->setShaderProgram(
                CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColor)
            );
        
        if (!f->m_iconSpriteWhitener || !f->m_iconSpriteWhitener->isVisible())
            m_iconSpriteWhitener->setShaderProgram(
                CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColor)
            );
    }

    if (f->m_iconSprite) {
        Utils::applyGradient(f->m_iconSprite, gradient.main, getIconType(), ColorType::Main, 105, false, m_isSecondPlayer, true, 2);
        f->m_iconSprite->setVisible(!gradient.main.isEmpty(ColorType::Main, m_isSecondPlayer));
        
        if (!f->m_iconSprite->isVisible())
            m_iconSprite->setShaderProgram(
                CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColor)
            );
    }

    if (f->m_iconSpriteSecondary) {
        Utils::applyGradient(f->m_iconSpriteSecondary, gradient.secondary, getIconType(), ColorType::Secondary, 205, false, m_isSecondPlayer, true, 2);
        f->m_iconSpriteSecondary->setVisible(!gradient.secondary.isEmpty(ColorType::Secondary, m_isSecondPlayer));
        
        if (!f->m_iconSpriteSecondary->isVisible())
            m_iconSpriteSecondary->setShaderProgram(
                CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColor)
            );
    }
    
    if (f->m_iconGlow) {
        Utils::applyGradient(f->m_iconGlow, gradient.glow, getIconType(), ColorType::Glow, 305, false, m_isSecondPlayer, true, 2);
        f->m_iconGlow->setVisible(!gradient.glow.isEmpty(ColorType::Glow, m_isSecondPlayer));
        
        if (!f->m_iconGlow->isVisible())
            m_iconGlow->setShaderProgram(
                CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColor)
            );
    }
    
    if (f->m_iconSpriteWhitener) {
        Utils::applyGradient(f->m_iconSpriteWhitener, gradient.white, getIconType(), ColorType::White, 405, false, m_isSecondPlayer, true, 2);
        f->m_iconSpriteWhitener->setVisible(!gradient.white.isEmpty(ColorType::White, m_isSecondPlayer));
        
        if (!f->m_iconSpriteWhitener->isVisible())
            m_iconSpriteWhitener->setShaderProgram(
                CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColor)
            );
    }
    
    if (f->m_iconSpriteLine) {
        Utils::applyGradient(f->m_iconSpriteLine, gradient.line, getIconType(), ColorType::Line, 505, false, m_isSecondPlayer, true, 2, true);
        f->m_iconSpriteLine->setVisible(!gradient.line.isEmpty(ColorType::Line, m_isSecondPlayer));
    }
    if (f->m_iconSpriteLineSecondary) {
        Utils::applyGradient(f->m_iconSpriteLineSecondary, gradient.line, getIconType(), ColorType::Line, 605, false, m_isSecondPlayer, true, 2, true);
        f->m_iconSpriteLineSecondary->setVisible(!gradient.line.isEmpty(ColorType::Line, m_isSecondPlayer));
    }
    if (f->m_iconSpriteLineWhitener) {
        Utils::applyGradient(f->m_iconSpriteLineWhitener, gradient.line, getIconType(), ColorType::Line, 705, false, m_isSecondPlayer, true, 2, true);
        f->m_iconSpriteLineWhitener->setVisible(!gradient.line.isEmpty(ColorType::Line, m_isSecondPlayer));
    }
}

void ProPlayerObject::updateVehicleSprite(Gradient gradient, auto f) {
    if (!gradient.main.isEmpty(ColorType::Main, m_isSecondPlayer))
        updateSprite(m_vehicleSprite, f->m_vehicleSprite, SpriteType::Vehicle, ColorType::Main);

    if (!gradient.secondary.isEmpty(ColorType::Secondary, m_isSecondPlayer))
        updateSprite(m_vehicleSpriteSecondary, f->m_vehicleSpriteSecondary, SpriteType::Vehicle, ColorType::Secondary);

    if (!gradient.glow.isEmpty(ColorType::Glow, m_isSecondPlayer))
        updateSprite(m_vehicleGlow, f->m_vehicleGlow, SpriteType::Vehicle, ColorType::Glow);

    if (!gradient.white.isEmpty(ColorType::White, m_isSecondPlayer))
        updateSprite(m_vehicleSpriteWhitener, f->m_vehicleSpriteWhitener, SpriteType::Vehicle, ColorType::White);

    if (!gradient.line.isEmpty(ColorType::Line, m_isSecondPlayer)) {
        updateSprite(m_vehicleSprite, f->m_vehicleSpriteLine, SpriteType::Vehicle, ColorType::Line);
        updateSprite(m_vehicleSpriteSecondary, f->m_vehicleSpriteLineSecondary, SpriteType::Vehicle, ColorType::Line);
        updateSprite(m_vehicleSpriteWhitener, f->m_vehicleSpriteLineWhitener, SpriteType::Vehicle, ColorType::Line);
    }

    if (f->m_vehicleSprite) {
        Utils::applyGradient(f->m_vehicleSprite, gradient.main, getIconType(), ColorType::Main, 104, false, m_isSecondPlayer, true, 44);
        f->m_vehicleSprite->setVisible(!gradient.main.isEmpty(ColorType::Main, m_isSecondPlayer));
    }

    if (f->m_vehicleSpriteSecondary) {
        Utils::applyGradient(f->m_vehicleSpriteSecondary, gradient.secondary, getIconType(), ColorType::Secondary, 204, false, m_isSecondPlayer, true, 44);
        f->m_vehicleSpriteSecondary->setVisible(!gradient.secondary.isEmpty(ColorType::Secondary, m_isSecondPlayer));
    }

    if (f->m_vehicleGlow) {
        Utils::applyGradient(f->m_vehicleGlow, gradient.glow, getIconType(), ColorType::Glow, 304, false, m_isSecondPlayer, true, 44);
        f->m_vehicleGlow->setVisible(!gradient.glow.isEmpty(ColorType::Glow, m_isSecondPlayer));
    }

    if (f->m_vehicleSpriteWhitener) {
        Utils::applyGradient(f->m_vehicleSpriteWhitener, gradient.white, getIconType(), ColorType::White, 404, false, m_isSecondPlayer, true, 44);
        f->m_vehicleSpriteWhitener->setVisible(!gradient.white.isEmpty(ColorType::White, m_isSecondPlayer));
    }

    if (f->m_vehicleSpriteLine) {
        Utils::applyGradient(f->m_vehicleSpriteLine, gradient.line, getIconType(), ColorType::Line, 504, false, m_isSecondPlayer, true, 44, true);
        f->m_vehicleSpriteLine->setVisible(!gradient.line.isEmpty(ColorType::Line, m_isSecondPlayer));
    }
    if (f->m_vehicleSpriteLineSecondary) {
        Utils::applyGradient(f->m_vehicleSpriteLineSecondary, gradient.line, getIconType(), ColorType::Line, 604, false, m_isSecondPlayer, true, 44, true);
        f->m_vehicleSpriteLineSecondary->setVisible(!gradient.line.isEmpty(ColorType::Line, m_isSecondPlayer));
    }
    if (f->m_vehicleSpriteLineWhitener) {
        Utils::applyGradient(f->m_vehicleSpriteLineWhitener, gradient.line, getIconType(), ColorType::Line, 704, false, m_isSecondPlayer, true, 44, true);
        f->m_vehicleSpriteLineWhitener->setVisible(!gradient.line.isEmpty(ColorType::Line, m_isSecondPlayer));
    }

}

void ProPlayerObject::updateAnimSprite(IconType type, Gradient gradient, auto f) {
    GJRobotSprite* sprite = type == IconType::Robot ? m_robotSprite : m_spiderSprite;
    log::debug("1");

    if (!sprite) return;
    if (!sprite->m_paSprite) return;
    
    log::debug("2");

    Utils::patchBatchNode(type == IconType::Robot ? m_robotBatchNode : m_spiderBatchNode);

    int count = 1;

    for (CCSpritePart* spr : CCArrayExt<CCSpritePart*>(sprite->m_paSprite->m_spriteParts)) {
        if (gradient.main.isEmpty(ColorType::Main, m_isSecondPlayer)) break;

        CCSprite* sprite = CCSprite::createWithSpriteFrame(spr->displayFrame());
        sprite->setID(fmt::format("{}-gradient-{}"_spr, Utils::getTypeID(SpriteType::Animation), count).c_str());
        sprite->setAnchorPoint({0, 0});

        spr->addChild(sprite);

        Utils::hideSprite(spr);
        
        f->m_animSprites.push_back(sprite);
        f->m_animSpriteParents[sprite] = spr;

        Utils::applyGradient(sprite, gradient.main, type, ColorType::Main, 100 + count, false, m_isSecondPlayer, true, 55);

        count++;
    }

    count = 1;

    for (CCSprite* spr : CCArrayExt<CCSprite*>(sprite->m_secondArray)) {
        if (gradient.secondary.isEmpty(ColorType::Secondary, m_isSecondPlayer)) break;

        CCSprite* sprite = CCSprite::createWithSpriteFrame(spr->displayFrame());
        sprite->setID(fmt::format("{}-gradient-{}"_spr, Utils::getTypeID(SpriteType::Animation), count).c_str());
        sprite->setAnchorPoint({0, 0});

        spr->addChild(sprite);

        Utils::hideSprite(spr);
        
        f->m_animSprites.push_back(sprite);
        f->m_animSpriteParents[sprite] = spr;

        Utils::applyGradient(sprite, gradient.secondary, type, ColorType::Secondary, 200 + count, false, m_isSecondPlayer, true, 55);

        count++;
    }

    count = 1;

    for (CCSprite* spr : CCArrayExt<CCSprite*>(sprite->m_glowSprite->getChildren())) {
        if (gradient.glow.isEmpty(ColorType::Glow, m_isSecondPlayer)) break;

        CCSprite* sprite = CCSprite::createWithSpriteFrame(spr->displayFrame());
        sprite->setID(fmt::format("{}-gradient-{}"_spr, Utils::getTypeID(SpriteType::Animation), count).c_str());
        sprite->setAnchorPoint({0, 0});

        spr->addChild(sprite);

        Utils::hideSprite(spr);
        
        f->m_animSprites.push_back(sprite);
        f->m_animSpriteParents[sprite] = spr;

        Utils::applyGradient(sprite, gradient.glow, type, ColorType::Glow, 300 + count, false, m_isSecondPlayer, true, 55);

        count++;
    }
    
    if (!gradient.white.isEmpty(ColorType::White, m_isSecondPlayer)) {
        CCSprite* spr = CCSprite::createWithSpriteFrame(sprite->m_extraSprite->displayFrame());
        spr->setID(fmt::format("{}-gradient"_spr, Utils::getTypeID(SpriteType::Animation)).c_str());
        spr->setAnchorPoint({0, 0});
        
        if (Loader::get()->isModLoaded("alphalaneous.fine_outline")) {
            Loader::get()->queueInMainThread([child = Ref(spr), dad = Ref(sprite->m_extraSprite)] {
                dad->addChild(child, 10);
            });
        } else
            sprite->m_extraSprite->addChild(spr);

        Utils::hideSprite(sprite->m_extraSprite);
        
        f->m_animSprites.push_back(spr);
        f->m_animSpriteParents[sprite] = spr;

        Utils::applyGradient(spr, gradient.white, type, ColorType::White, 400, false, m_isSecondPlayer, true, 55);
    }

    count = 1;

    for (CCSpritePart* spr : CCArrayExt<CCSpritePart*>(sprite->m_paSprite->m_spriteParts)) {
        if (gradient.line.isEmpty(ColorType::Line, m_isSecondPlayer)) break;

        CCSprite* sprite = CCSprite::createWithSpriteFrame(spr->displayFrame());
        sprite->setID(fmt::format("{}-gradient-line-{}"_spr, Utils::getTypeID(SpriteType::Animation), count).c_str());
        sprite->setAnchorPoint({0, 0});
        
        if (Loader::get()->isModLoaded("alphalaneous.fine_outline")) {
            Loader::get()->queueInMainThread([child = Ref(sprite), dad = Ref(spr)] {
                dad->addChild(child, 10);
            });
        } else
            spr->addChild(sprite);

        Utils::hideSprite(spr);

        f->m_animSprites.push_back(sprite);
        f->m_animSpriteParents[sprite] = spr;

        Utils::applyGradient(sprite, gradient.line, type, ColorType::Line, 500 + count, false, m_isSecondPlayer, true, 55, true);

        count++;
    }

    count = 1;

    for (CCSprite* spr : CCArrayExt<CCSprite*>(sprite->m_secondArray)) {
        if (gradient.line.isEmpty(ColorType::Line, m_isSecondPlayer)) break;

        CCSprite* sprite = CCSprite::createWithSpriteFrame(spr->displayFrame());
        sprite->setID(fmt::format("{}-gradient-line-{}"_spr, Utils::getTypeID(SpriteType::Animation), count).c_str());
        sprite->setAnchorPoint({0, 0});

        if (Loader::get()->isModLoaded("alphalaneous.fine_outline")) {
            Loader::get()->queueInMainThread([child = Ref(sprite), dad = Ref(spr)] {
                dad->addChild(child, 10);
            });
        } else
            spr->addChild(sprite);
        
        Utils::hideSprite(spr);

        f->m_animSprites.push_back(sprite);
        f->m_animSpriteParents[sprite] = spr;

        Utils::applyGradient(sprite, gradient.line, type, ColorType::Line, 600 + count, false, m_isSecondPlayer, true, 55, true);

        count++;
    }
    
    if (!gradient.line.isEmpty(ColorType::Line, m_isSecondPlayer)) {
        CCSprite* spr = CCSprite::createWithSpriteFrame(sprite->m_extraSprite->displayFrame());
        spr->setID(fmt::format("{}-gradient-line"_spr, Utils::getTypeID(SpriteType::Animation)).c_str());
        spr->setAnchorPoint({0, 0});
        
        if (Loader::get()->isModLoaded("alphalaneous.fine_outline")) {
            Loader::get()->queueInMainThread([child = Ref(spr), dad = Ref(sprite->m_extraSprite)] {
                dad->addChild(child, 10);
            });
        } else
            sprite->m_extraSprite->addChild(spr);

        Utils::hideSprite(sprite->m_extraSprite);

        f->m_animSprites.push_back(spr);
        f->m_animSpriteParents[sprite] = spr;

        Utils::applyGradient(spr, gradient.line, type, ColorType::Line, 700, false, m_isSecondPlayer, true, 55, true);
    }
}

void ProPlayerObject::updateGradient() {
    if (shouldReturn(GJBaseGameLayer::get())) return;

    auto f = m_fields.self();

    IconType type = getIconType();

    if (type == f->m_previousType) return;

    f->m_previousType = type;

    Gradient gradient = Utils::getGradient(type, m_isSecondPlayer);

    if (f->m_thatOneUfoShipAndCubeModIsLoaded) {
        if (f->m_iconSprite) f->m_iconSprite->setVisible(true);
        if (f->m_iconSpriteSecondary) f->m_iconSpriteSecondary->setVisible(true);
        if (f->m_iconGlow) f->m_iconGlow->setVisible(true);
        if (f->m_iconSpriteWhitener) f->m_iconSpriteWhitener->setVisible(true);
        if (f->m_iconSpriteLine) f->m_iconSpriteLine->setVisible(true);
    }

    if (type != IconType::Ship && type != IconType::Jetpack && type != IconType::Ufo)
        return updateIconSprite(gradient, f);

    updateVehicleSprite(gradient, f);
    updateIconSprite(Utils::getGradient(IconType::Cube, m_isSecondPlayer), f);

    if (f->m_thatOneUfoShipAndCubeModIsLoaded && !m_isSecondPlayer) {
        if (f->m_iconSprite) f->m_iconSprite->setVisible(false);
        if (f->m_iconSpriteSecondary) f->m_iconSpriteSecondary->setVisible(false);
        if (f->m_iconGlow) f->m_iconGlow->setVisible(false);
        if (f->m_iconSpriteWhitener) f->m_iconSpriteWhitener->setVisible(false);
        if (f->m_iconSpriteLine) f->m_iconSpriteLine->setVisible(false);

        m_iconSprite->setVisible(true);
        m_iconSpriteSecondary->setVisible(true);
        m_iconGlow->setVisible(m_hasGlow);
        m_iconSpriteWhitener->setVisible(true);

        m_iconSprite->setOpacity(255);
        m_iconSpriteSecondary->setOpacity(255);
        m_iconGlow->setOpacity(255);
        m_iconSpriteWhitener->setOpacity(255);
    }
}

void ProPlayerObject::togglePlayerScale(bool p0, bool p1) {
    PlayerObject::togglePlayerScale(p0, p1);

    if (GameManager::get()->getGameVariable("0060")) {
        m_fields->m_previousType = static_cast<IconType>(-9038);
        updateGradient();
    }
}

void ProPlayerObject::updatePlayerFrame(int p0) {
    PlayerObject::updatePlayerFrame(p0);
    updateGradient();
}

void ProPlayerObject::updatePlayerShipFrame(int p0) {
    PlayerObject::updatePlayerShipFrame(p0);
    updateGradient();
}

void ProPlayerObject::updatePlayerRollFrame(int p0) {
    PlayerObject::updatePlayerRollFrame(p0);
    updateGradient();
}

void ProPlayerObject::updatePlayerBirdFrame(int p0) {
    PlayerObject::updatePlayerBirdFrame(p0);
    updateGradient();
}

void ProPlayerObject::updatePlayerDartFrame(int p0) {
    PlayerObject::updatePlayerDartFrame(p0);
    updateGradient();
}

void ProPlayerObject::updatePlayerSwingFrame(int p0) {
    PlayerObject::updatePlayerSwingFrame(p0);
    updateGradient();
}

void ProPlayerObject::updatePlayerJetpackFrame(int p0) {
    PlayerObject::updatePlayerJetpackFrame(p0);
    updateGradient();
}

void ProPlayerObject::createRobot(int p0) {
    PlayerObject::createRobot(p0);

    if (getTag() == 0xCb04) return;

    Loader::get()->queueInMainThread([self = Ref(this)] {
        if (self->shouldReturn(GJBaseGameLayer::get())) return;

        self->updateAnimSprite(
            IconType::Robot,
            Utils::getGradient(IconType::Robot, self == self->m_gameLayer->m_player2),
            self->m_fields.self()
        );
    });
}

void ProPlayerObject::createSpider(int p0) {
    PlayerObject::createSpider(p0);

    if (getTag() == 0xCb04) return;

    Loader::get()->queueInMainThread([self = Ref(this)] {
        if (self->shouldReturn(GJBaseGameLayer::get())) return;

        self->updateAnimSprite(
            IconType::Spider,
            Utils::getGradient(IconType::Spider, self == self->m_gameLayer->m_player2),
            self->m_fields.self()
        );
    });
}

bool ProPlayerObject::init(int p0, int p1, GJBaseGameLayer* p2, CCLayer* p3, bool p4) {
    if (!PlayerObject::init(p0, p1, p2, p3, p4)) return false;

    m_fields->m_thatOneUfoShipAndCubeModIsLoaded = Loader::get()->isModLoaded("yellowcat98.custom_ufo_n_ship_cube");
    
    Loader::get()->queueInMainThread([self = Ref(this)] {
        if (
            LevelEditorLayer::get()
            && self->m_isSecondPlayer
            && Loader::get()->isModLoaded("weebify.separate_dual_icons")
        ) {
            self->schedule(schedule_selector(ProPlayerObject::updateCube));
        }
        
        self->updateGradient();
    });

    return true;
}