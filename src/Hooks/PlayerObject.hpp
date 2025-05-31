#pragma once

#include "../Includes.hpp"

#include <Geode/modify/PlayerObject.hpp>

class $modify(ProPlayerObject, PlayerObject) {

	static void onModify(auto& self) {
        (void)self.setHookPriorityPre("PlayerObject::playDeathEffect", Priority::Last + 0x500000); // eclipses big ass priority didnt test other mod menus
        (void)self.setHookPriorityPost("PlayerObject::playSpawnEffect", Priority::Last);
        (void)self.setHookPriorityPost("PlayerObject::playCompleteEffect", Priority::Last);
	}

	struct Fields {
		IconType m_previousType = static_cast<IconType>(-9038);

		CCSprite* m_iconSprite = nullptr;
		CCSprite* m_iconSpriteSecondary = nullptr;
		CCSprite* m_vehicleSprite = nullptr;
		CCSprite* m_vehicleSpriteSecondary = nullptr;

		std::vector<CCSprite*> m_animSprites;

		bool m_thatOneUfoShipAndCubeModIsLoaded = false;
	};

	IconType getIconType();

	void updateVisibility(bool = false);

	void updateSprite(CCSprite*, CCSprite*&, SpriteType, bool);

	void updateIconSprite(Gradient, auto);

	void updateVehicleSprite(Gradient, auto);

	void updateAnimSprite(IconType, Gradient, auto);

	void updateGradient();

	void togglePlayerScale(bool, bool);

	void updatePlayerFrame(int);

    void updatePlayerShipFrame(int);

    void updatePlayerRollFrame(int);

    void updatePlayerBirdFrame(int);

    void updatePlayerDartFrame(int);

    void updatePlayerSwingFrame(int);

    void updatePlayerJetpackFrame(int);

	void createRobot(int);

	void createSpider(int);

	bool init(int, int, GJBaseGameLayer*, cocos2d::CCLayer*, bool);

	void playCompleteEffect(bool, bool);

	void playDeathEffect();

	void playSpawnEffect();

};