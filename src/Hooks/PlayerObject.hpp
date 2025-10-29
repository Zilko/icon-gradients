#pragma once

#include "../Includes.hpp"

#include <Geode/modify/PlayerObject.hpp>

class $modify(ProPlayerObject, PlayerObject) {

	static void onModify(auto& self) {
		(void)self.setHookPriorityPost("PlayerObject::updatePlayerFrame", Priority::Last + 21903809);
		(void)self.setHookPriorityPost("PlayerObject::updatePlayerShipFrame", Priority::Last + 21903809);
		(void)self.setHookPriorityPost("PlayerObject::updatePlayerRollFrame", Priority::Last + 21903809);
		(void)self.setHookPriorityPost("PlayerObject::updatePlayerBirdFrame", Priority::Last + 21903809);
		(void)self.setHookPriorityPost("PlayerObject::updatePlayerDartFrame", Priority::Last + 21903809);
		(void)self.setHookPriorityPost("PlayerObject::createRobot", Priority::Last + 21903809);
		(void)self.setHookPriorityPost("PlayerObject::createSpider", Priority::Last + 21903809);
		(void)self.setHookPriorityPost("PlayerObject::updatePlayerSwingFrame", Priority::Last + 21903809);
		(void)self.setHookPriorityPost("PlayerObject::updatePlayerJetpackFrame", Priority::Last + 21903809);
	}

	struct Fields {
		IconType m_previousType = static_cast<IconType>(-9038);

		CCSprite* m_iconSprite = nullptr;
		CCSprite* m_iconSpriteSecondary = nullptr;
		CCSprite* m_iconGlow = nullptr;
		CCSprite* m_iconSpriteWhitener = nullptr;
		CCSprite* m_iconSpriteLine = nullptr;
		CCSprite* m_iconSpriteLineSecondary = nullptr;
		CCSprite* m_iconSpriteLineWhitener = nullptr;
		CCSprite* m_vehicleSprite = nullptr;
		CCSprite* m_vehicleSpriteSecondary = nullptr;
		CCSprite* m_vehicleGlow = nullptr;
		CCSprite* m_vehicleSpriteWhitener = nullptr;
		CCSprite* m_vehicleSpriteLine = nullptr;
		CCSprite* m_vehicleSpriteLineSecondary = nullptr;
		CCSprite* m_vehicleSpriteLineWhitener = nullptr;

		std::vector<Ref<CCSprite>> m_animSprites;
		std::unordered_map<CCSprite*, Ref<CCSprite>> m_animSpriteParents;

		bool m_thatOneUfoShipAndCubeModIsLoaded = false;
		bool m_separateDualIconsIsLoaded = false;
	};
	
	bool shouldReturn(GJBaseGameLayer*, bool = false);

	IconType getIconType();
	
	void updateCube(float);

	void updateFlip(float);
	
	void updateVisibility();

	void updateSprite(CCSprite*, CCSprite*&, SpriteType, ColorType);

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

	bool init(int, int, GJBaseGameLayer*, CCLayer*, bool);

};