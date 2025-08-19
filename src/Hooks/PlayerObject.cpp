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
    
    if (f->m_iconSprite && f->m_iconSprite->getOpacity() != m_iconSprite->getOpacity())
        f->m_iconSprite->setOpacity(m_iconSprite->getOpacity());

    if (f->m_iconSpriteSecondary && f->m_iconSpriteSecondary->getOpacity() != m_iconSpriteSecondary->getOpacity())
        f->m_iconSpriteSecondary->setOpacity(m_iconSpriteSecondary->getOpacity());

    if (f->m_iconGlow && f->m_iconGlow->getOpacity() != m_iconGlow->getOpacity())
        f->m_iconGlow->setOpacity(m_iconGlow->getOpacity());

    if (f->m_vehicleSprite && f->m_vehicleSprite->getOpacity() != m_vehicleSprite->getOpacity())
        f->m_vehicleSprite->setOpacity(m_vehicleSprite->getOpacity());

    if (f->m_vehicleSpriteSecondary && f->m_vehicleSpriteSecondary->getOpacity() != m_vehicleSpriteSecondary->getOpacity())
        f->m_vehicleSpriteSecondary->setOpacity(m_vehicleSpriteSecondary->getOpacity());

    if (f->m_vehicleGlow && f->m_vehicleGlow->getOpacity() != m_vehicleGlow->getOpacity())
        f->m_vehicleGlow->setOpacity(m_vehicleGlow->getOpacity());

    for (CCSprite* sprite : f->m_animSprites)
        if (f->m_animSpriteParents.contains(sprite))
            sprite->setOpacity(f->m_animSpriteParents.at(sprite)->getOpacity());
}


void ProPlayerObject::updateSprite(CCSprite* realSprite, CCSprite*& sprite, SpriteType type, ColorType color) {
    if (!sprite) {
        sprite = CCSprite::createWithSpriteFrame(realSprite->displayFrame());
        sprite->setID(fmt::format("{}-gradient-{}"_spr, Utils::getTypeID(type), std::to_string(color)).c_str());

        realSprite->addChild(sprite);
    } else
        sprite->setDisplayFrame(realSprite->displayFrame());

    sprite->setAnchorPoint({0, 0});

    realSprite->setCascadeOpacityEnabled(true);
}

void ProPlayerObject::updateIconSprite(Gradient gradient, auto f) {
    if (!gradient.main.points.empty())
        updateSprite(m_iconSprite, f->m_iconSprite, SpriteType::Icon, ColorType::Main);

    if (!gradient.secondary.points.empty())
        updateSprite(m_iconSpriteSecondary, f->m_iconSpriteSecondary, SpriteType::Icon, ColorType::Secondary);

    if (!gradient.glow.points.empty())
        updateSprite(m_iconGlow, f->m_iconGlow, SpriteType::Icon, ColorType::Glow);

    if (!gradient.white.points.empty())
        updateSprite(m_iconSpriteWhitener, f->m_iconSpriteWhitener, SpriteType::Icon, ColorType::White);

    if (f->m_iconSprite) {
        Utils::applyGradient(f->m_iconSprite, gradient.main, getIconType(), 105, false, m_isSecondPlayer, true, 2);
        f->m_iconSprite->setVisible(!gradient.main.points.empty());
    }

    if (f->m_iconSpriteSecondary) {
        Utils::applyGradient(f->m_iconSpriteSecondary, gradient.secondary, getIconType(), 205, false, m_isSecondPlayer, true, 2);
        f->m_iconSpriteSecondary->setVisible(!gradient.secondary.points.empty());
    }
    
    if (f->m_iconGlow) {
        Utils::applyGradient(f->m_iconGlow, gradient.glow, getIconType(), 305, false, m_isSecondPlayer, true, 2);
        f->m_iconGlow->setVisible(!gradient.glow.points.empty());
    }
    
    if (f->m_iconSpriteWhitener) {
        Utils::applyGradient(f->m_iconSpriteWhitener, gradient.white, true);
        f->m_iconSpriteWhitener->setVisible(!gradient.white.points.empty());
    }
}

void ProPlayerObject::updateVehicleSprite(Gradient gradient, auto f) {
    if (!gradient.main.points.empty())
        updateSprite(m_vehicleSprite, f->m_vehicleSprite, SpriteType::Vehicle, ColorType::Main);

    if (!gradient.secondary.points.empty())
        updateSprite(m_vehicleSpriteSecondary, f->m_vehicleSpriteSecondary, SpriteType::Vehicle, ColorType::Secondary);

    if (!gradient.glow.points.empty())
        updateSprite(m_vehicleGlow, f->m_vehicleGlow, SpriteType::Vehicle, ColorType::Glow);

    if (!gradient.white.points.empty())
        updateSprite(m_vehicleSpriteWhitener, f->m_vehicleSpriteWhitener, SpriteType::Vehicle, ColorType::White);

    if (f->m_vehicleSprite) {
        Utils::applyGradient(f->m_vehicleSprite, gradient.main, getIconType(), 104, false, m_isSecondPlayer, true, 44);
        f->m_vehicleSprite->setVisible(!gradient.main.points.empty());
    }

    if (f->m_vehicleSpriteSecondary) {
        Utils::applyGradient(f->m_vehicleSpriteSecondary, gradient.secondary, getIconType(), 204, false, m_isSecondPlayer, true, 44);
        f->m_vehicleSpriteSecondary->setVisible(!gradient.secondary.points.empty());
    }

    if (f->m_vehicleGlow) {
        Utils::applyGradient(f->m_vehicleGlow, gradient.glow, getIconType(), 304, false, m_isSecondPlayer, true, 44);
        f->m_vehicleGlow->setVisible(!gradient.glow.points.empty());
    }

    if (f->m_vehicleSpriteWhitener) {
        Utils::applyGradient(f->m_vehicleSpriteWhitener, gradient.white, true);
        f->m_vehicleSpriteWhitener->setVisible(!gradient.white.points.empty());
    }

}

void ProPlayerObject::updateAnimSprite(IconType type, Gradient gradient, auto f) {
    if (shouldReturn(GJBaseGameLayer::get())) return;

    GJRobotSprite* sprite = type == IconType::Robot ? m_robotSprite : m_spiderSprite;

    if (!sprite) return;
    if (!sprite->m_paSprite) return;

    Utils::patchBatchNode(type == IconType::Robot ? m_robotBatchNode : m_spiderBatchNode);

    int count = 1;

    for (CCSpritePart* spr : CCArrayExt<CCSpritePart*>(sprite->m_paSprite->m_spriteParts)) {
        if (gradient.main.points.empty()) break;

        CCSprite* sprite = CCSprite::createWithSpriteFrame(spr->displayFrame());
        sprite->setID(fmt::format("{}-gradient-{}"_spr, Utils::getTypeID(SpriteType::Animation), count).c_str());
        sprite->setAnchorPoint({0, 0});

        spr->addChild(sprite);

        f->m_animSprites.push_back(sprite);
        f->m_animSpriteParents[sprite] = spr;

        Utils::applyGradient(sprite, gradient.main, type, 100 + count, false, m_isSecondPlayer, true, 55);

        count++;
    }

    count = 1;

    for (CCSprite* spr : CCArrayExt<CCSprite*>(sprite->m_secondArray)) {
        if (gradient.secondary.points.empty()) break;

        CCSprite* sprite = CCSprite::createWithSpriteFrame(spr->displayFrame());
        sprite->setID(fmt::format("{}-gradient-{}"_spr, Utils::getTypeID(SpriteType::Animation), count).c_str());
        sprite->setAnchorPoint({0, 0});

        spr->addChild(sprite);

        f->m_animSprites.push_back(sprite);
        f->m_animSpriteParents[sprite] = spr;

        Utils::applyGradient(sprite, gradient.secondary, type, 200 + count, false, m_isSecondPlayer, true, 55);

        count++;
    }

    count = 1;

    for (CCSprite* spr : CCArrayExt<CCSprite*>(sprite->m_glowSprite->getChildren())) {
        if (gradient.glow.points.empty()) break;

        CCSprite* sprite = CCSprite::createWithSpriteFrame(spr->displayFrame());
        sprite->setID(fmt::format("{}-gradient-{}"_spr, Utils::getTypeID(SpriteType::Animation), count).c_str());
        sprite->setAnchorPoint({0, 0});

        spr->addChild(sprite);

        f->m_animSprites.push_back(sprite);
        f->m_animSpriteParents[sprite] = spr;

        Utils::applyGradient(sprite, gradient.glow, type, 300 + count, false, m_isSecondPlayer, true, 55);

        count++;
    }
    if (!gradient.white.points.empty()) {
        CCSprite* spr = CCSprite::createWithSpriteFrame(sprite->m_extraSprite->displayFrame());
        spr->setID(fmt::format("{}-gradient-{}"_spr, Utils::getTypeID(SpriteType::Animation), count).c_str());
        spr->setAnchorPoint({0, 0});

        sprite->m_extraSprite->addChild(spr);

        f->m_animSprites.push_back(spr);

        Utils::applyGradient(spr, gradient.white, true);

        count++;
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
    }

    if (type != IconType::Ship && type != IconType::Jetpack && type != IconType::Ufo)
        return updateIconSprite(gradient, f);

    updateVehicleSprite(gradient, f);
    updateIconSprite(Utils::getGradient(IconType::Cube, m_isSecondPlayer), f);

    if (f->m_thatOneUfoShipAndCubeModIsLoaded && !m_isSecondPlayer) {
        if (f->m_iconSprite) f->m_iconSprite->setVisible(false);
        if (f->m_iconSpriteSecondary) f->m_iconSpriteSecondary->setVisible(false);
        if (f->m_iconGlow) f->m_iconGlow->setVisible(false);

        m_iconSprite->setVisible(true);
        m_iconSpriteSecondary->setVisible(true);

        m_iconSprite->setOpacity(255);
        m_iconSpriteSecondary->setOpacity(255);
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