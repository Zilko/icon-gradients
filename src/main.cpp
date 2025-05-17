#include "includes.hpp"

#include <Geode/modify/PlayLayer.hpp>

class $modify(PlayLayer) {
	void setupHasCompleted() {
		PlayLayer::setupHasCompleted();

		CCSprite* spr;

		// spr = CCSprite::createWithSpriteFrame(m_player1->m_iconSpriteSecondary->displayFrame());
		// spr->setColor(ccc3(90, 90, 255));
		// spr->setPosition(m_player1->m_iconSpriteSecondary->getPosition());

		// m_player1->m_iconSprite->addChild(spr);

		// spr = CCSprite::create("C:\\Users\\zilk\\Pictures\\image.png");
		// spr = CCSprite::createWithSpriteFrame("player_231_001.png");
		spr = CCSprite::createWithSpriteFrame(m_player1->m_iconSprite->displayFrame());
		spr->setAnchorPoint({0, 0});

		// log::debug("{}", m_player1->m_iconSprite->displayFrame()->m_bRotated);

		m_player1->m_iconSprite->addChild(spr);

		
	}
};