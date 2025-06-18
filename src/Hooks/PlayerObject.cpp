#include "../Utils/Utils.hpp"
#include "../Utils/Cache.hpp"

#include "PlayerObject.hpp"

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

void ProPlayerObject::updateVisibility() {
    auto f = m_fields.self();

    if (f->m_iconSprite && f->m_iconSprite->getOpacity() != m_iconSprite->getOpacity())
        f->m_iconSprite->setOpacity(m_iconSprite->getOpacity());

    if (f->m_iconSpriteSecondary && f->m_iconSpriteSecondary->getOpacity() != m_iconSpriteSecondary->getOpacity())
        f->m_iconSpriteSecondary->setOpacity(m_iconSpriteSecondary->getOpacity());

    if (f->m_vehicleSprite && f->m_vehicleSprite->getOpacity() != m_vehicleSprite->getOpacity())
        f->m_vehicleSprite->setOpacity(m_vehicleSprite->getOpacity());

    if (f->m_vehicleSpriteSecondary && f->m_vehicleSpriteSecondary->getOpacity() != m_vehicleSpriteSecondary->getOpacity())
        f->m_vehicleSpriteSecondary->setOpacity(m_vehicleSpriteSecondary->getOpacity());

    for (CCSprite* sprite : f->m_animSprites) {
        CCSprite* parent = static_cast<CCSprite*>(sprite->getParent());
        GLubyte parentOpacity = parent->getOpacity();

        if (sprite->getOpacity() != parentOpacity) {
            sprite->setOpacity(parentOpacity);
        }
    }
}


void ProPlayerObject::updateSprite(CCSprite* realSprite, CCSprite*& sprite, SpriteType type, bool secondary) {
    if (!sprite) {
        sprite = CCSprite::createWithSpriteFrame(realSprite->displayFrame());
        sprite->setID(fmt::format("{}-gradient-{}"_spr, Utils::getTypeID(type), secondary ? "2" : "1").c_str());

        realSprite->addChild(sprite);
    } else
        sprite->setDisplayFrame(realSprite->displayFrame());

    sprite->setAnchorPoint({0, 0});

    realSprite->setCascadeOpacityEnabled(true);
}

void ProPlayerObject::updateIconSprite(Gradient gradient, auto f) {
    if (!gradient.main.points.empty())
        updateSprite(m_iconSprite, f->m_iconSprite, SpriteType::Icon, false);

    if (!gradient.secondary.points.empty())
        updateSprite(m_iconSpriteSecondary, f->m_iconSpriteSecondary, SpriteType::Icon, true);

    if (f->m_iconSprite) {
        Utils::applyGradient(f->m_iconSprite, gradient.main, true);
        f->m_iconSprite->setVisible(!gradient.main.points.empty());
    }

    if (f->m_iconSpriteSecondary) {
        Utils::applyGradient(f->m_iconSpriteSecondary, gradient.secondary, true);
        f->m_iconSpriteSecondary->setVisible(!gradient.secondary.points.empty());
    }
}

void ProPlayerObject::updateVehicleSprite(Gradient gradient, auto f) {
    if (!gradient.main.points.empty())
        updateSprite(m_vehicleSprite, f->m_vehicleSprite, SpriteType::Vehicle, false);

    if (!gradient.secondary.points.empty())
        updateSprite(m_vehicleSpriteSecondary, f->m_vehicleSpriteSecondary, SpriteType::Vehicle, true);

    if (f->m_vehicleSprite) {
        Utils::applyGradient(f->m_vehicleSprite, gradient.main, true);
        f->m_vehicleSprite->setVisible(!gradient.main.points.empty());
    }

    if (f->m_vehicleSpriteSecondary) {
        Utils::applyGradient(f->m_vehicleSpriteSecondary, gradient.secondary, true);
        f->m_vehicleSpriteSecondary->setVisible(!gradient.secondary.points.empty());
    }

}

void ProPlayerObject::updateAnimSprite(IconType type, Gradient gradient, auto f) {
    GJBaseGameLayer* bgl = GJBaseGameLayer::get();

    if (m_gameLayer != bgl || !bgl) return;
    if (this != bgl->m_player1 && this != bgl->m_player2) return;
    if (Utils::isSettingEnabled(MOD_DISABLED)) return;
    if (m_isSecondPlayer && Utils::isSettingEnabled(P2_DISABLED)) return;

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

        Utils::applyGradient(sprite, gradient.main, true);

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

        Utils::applyGradient(sprite, gradient.secondary, true);

        count++;
    }
}

void ProPlayerObject::updateGradient() {
    GJBaseGameLayer* bgl = GJBaseGameLayer::get();

    if (!bgl) return;
    if (this != bgl->m_player1 && this != bgl->m_player2) return;
    if (Utils::isSettingEnabled(MOD_DISABLED)) return;
    if (m_isSecondPlayer && Utils::isSettingEnabled(P2_DISABLED)) return;

    auto f = m_fields.self();

    IconType type = getIconType();
    bool shouldFlip = m_isSecondPlayer && Utils::isSettingEnabled(P2_FLIP);

    if (type == f->m_previousType) return;

    f->m_previousType = type;

    Gradient gradient = Utils::getGradient(type, shouldFlip);

    if (f->m_thatOneUfoShipAndCubeModIsLoaded) {
        if (f->m_iconSprite) f->m_iconSprite->setVisible(true);
        if (f->m_iconSpriteSecondary) f->m_iconSpriteSecondary->setVisible(true);
    }

    if (type != IconType::Ship && type != IconType::Jetpack && type != IconType::Ufo)
        return updateIconSprite(gradient, f);

    updateVehicleSprite(gradient, f);

    updateIconSprite(
        Utils::getGradient(IconType::Cube, shouldFlip),
        f
    );

    if (f->m_thatOneUfoShipAndCubeModIsLoaded && !m_isSecondPlayer) {
        if (f->m_iconSprite) f->m_iconSprite->setVisible(false);
        if (f->m_iconSpriteSecondary) f->m_iconSpriteSecondary->setVisible(false);

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

    if (!m_gameLayer) return;

    Loader::get()->queueInMainThread([this] {
        bool shouldFlip = this == m_gameLayer->m_player2 && Utils::isSettingEnabled(P2_FLIP);

        updateAnimSprite(
            IconType::Robot,
            Utils::getGradient(IconType::Robot, shouldFlip),
            m_fields.self()
        );
    });
}

void ProPlayerObject::createSpider(int p0) {
    PlayerObject::createSpider(p0);

    if (!m_gameLayer) return;

    Loader::get()->queueInMainThread([this] {
        bool shouldFlip = this == m_gameLayer->m_player2 && Utils::isSettingEnabled(P2_FLIP);

        updateAnimSprite(
            IconType::Spider,
            Utils::getGradient(IconType::Spider, shouldFlip),
            m_fields.self()
        );
    });
}

bool ProPlayerObject::init(int p0, int p1, GJBaseGameLayer* p2, cocos2d::CCLayer* p3, bool p4) {
    if (!PlayerObject::init(p0, p1, p2, p3, p4)) return false;

    m_fields->m_thatOneUfoShipAndCubeModIsLoaded = Loader::get()->isModLoaded("yellowcat98.custom_ufo_n_ship_cube");

    Loader::get()->queueInMainThread([this] {
        updateGradient();
    });

    return true;
}
