#pragma once

#include "../Includes.hpp"

#include <Geode/modify/PlayerObject.hpp>

class $modify(ProPlayerObject, PlayerObject) {

	struct Fields {
		IconType m_previousType = static_cast<IconType>(-9038);

		CCSprite* m_iconSprite = nullptr;
		CCSprite* m_iconSpriteSecondary = nullptr;
		CCSprite* m_vehicleSprite = nullptr;
		CCSprite* m_vehicleSpriteSecondary = nullptr;

		bool m_thatOneUfoShipAndCubeModIsLoaded = false;
	};

	IconType getIconType();

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

};