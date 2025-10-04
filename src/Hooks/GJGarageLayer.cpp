#include "../Utils/Utils.hpp"
#include "../Utils/Cache.hpp"
#include "../UI/GradientLayer.hpp"

#include "GJGarageLayer.hpp"

void ProGJGarageLayer::onGradient(CCObject*) {
	GradientLayer::create()->show();
}

void ProGJGarageLayer::onSwap(CCObject* sender) {
	if (!m_fields->m_originalCallback) return;

	(this->*m_fields->m_originalCallback)(sender);

	updateGradient();
}

IconType ProGJGarageLayer::getType() {
	auto f = m_fields.self();

	if (f->m_allIcons.empty()) return IconType::Cube;

	return Utils::getIconType(f->m_allIcons.front());
}

void ProGJGarageLayer::updateGradient(bool colorful) {	
	if (Utils::isSettingEnabled(MOD_DISABLED)) {
		if (!m_playerObject) return;

		Utils::applyGradient(m_playerObject, {{}, true}, ColorType::Main, true);
		Utils::applyGradient(m_playerObject, {{}, true}, ColorType::Secondary, true);
		Utils::applyGradient(m_playerObject, {{}, true}, ColorType::Glow, true);

		if (Loader::get()->isModLoaded("weebify.separate_dual_icons"))
			if (SimplePlayer* icon = typeinfo_cast<SimplePlayer*>(getChildByID("player2-icon"))) {
				Utils::applyGradient(icon, {{}, true}, ColorType::Main, true);
				Utils::applyGradient(icon, {{}, true}, ColorType::Secondary, true);
				Utils::applyGradient(icon, {{}, true}, ColorType::Glow, true);
			}
		
		return;
	}

	Loader::get()->queueInMainThread([this] {
		if (!m_playerObject) return;
		
		IconType type = Utils::getIconType(m_playerObject);

		Gradient gradient = Utils::getGradient(type, false);

		Utils::applyGradient(m_playerObject, gradient.main, ColorType::Main, true);
		Utils::applyGradient(m_playerObject, gradient.secondary, ColorType::Secondary, true);
		Utils::applyGradient(m_playerObject, gradient.glow, ColorType::Glow, true);

		if (Loader::get()->isModLoaded("weebify.separate_dual_icons"))
			if (SimplePlayer* icon = typeinfo_cast<SimplePlayer*>(getChildByID("player2-icon"))) {
				Gradient gradient = Utils::getGradient(
					Utils::getIconType(icon),
					Utils::isSettingEnabled(P2_FLIP)
				);

				Utils::applyGradient(icon, gradient.main, ColorType::Main, true);
				Utils::applyGradient(icon, gradient.secondary, ColorType::Secondary, true);
				Utils::applyGradient(icon, gradient.glow, ColorType::Glow, true);
			}
	});

	if (!Loader::get()->isModLoaded("ninkaz.colorful-icons") || !colorful) return;

	auto f = m_fields.self();
	bool loadedGradient = false;
	Gradient gradient;

	for (SimplePlayer* icon : f->m_allIcons) {
		IconType type = Utils::getIconType(icon);

		if (!loadedGradient) {
			gradient = Utils::getGradient(type, false);
			loadedGradient = true;
		}

		Utils::applyGradient(icon, gradient.main, ColorType::Main, true);
		Utils::applyGradient(icon, gradient.secondary, ColorType::Secondary, true);
		Utils::applyGradient(icon, gradient.glow, ColorType::Glow, true);
	}
}

void ProGJGarageLayer::updateQuickGradient() {
	// still lags randomly idk

	// if (!Loader::get()->isModLoaded("ninkaz.colorful-icons")) return;

	// auto f = m_fields.self();
	// bool loadedGradient = false;
	// Gradient gradient;

	// // log::debug("{}", f->m_visibleIcons.size());

	// for (SimplePlayer* icon : f->m_visibleIcons) {
	// 	IconType type = Utils::getIconType(icon);

	// 	if (!loadedGradient) {
	// 		gradient = Utils::getGradient(type, false);
	// 		loadedGradient = true;
	// 	}

	// 	Utils::applyGradient(icon, gradient.main, false, true);
	// 	Utils::applyGradient(icon, gradient.secondary, true, true);
	// }
}

bool ProGJGarageLayer::init() {
	if (!GJGarageLayer::init()) return false;

	updateGradient(false);

	Loader::get()->queueInMainThread([this] {
		if (CCNode* menu = getChildByID("shards-menu")) {
			CircleButtonSprite* spr = CircleButtonSprite::createWithSprite("buton.png"_spr, 0.87f, CircleBaseColor::Gray, CircleBaseSize::Small);
			spr->getTopNode()->setRotation(8);
			
			CCMenuItemSpriteExtra* btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(ProGJGarageLayer::onGradient));
			btn->setID("gradient-button"_spr);

			menu->addChild(btn);
			menu->updateLayout();

			if (Loader::get()->isModLoaded("weebify.separate_dual_icons"))
				if (CCNode* buttonNode = menu->getChildByID("swap-2p-button")) {
					CCMenuItemSpriteExtra* button = static_cast<CCMenuItemSpriteExtra*>(buttonNode);

					m_fields->m_originalCallback = button->m_pfnSelector;
					button->m_pfnSelector = menu_selector(ProGJGarageLayer::onSwap);
				}
		}
	}); 

	return true;
}

void ProGJGarageLayer::onSelect(CCObject* sender) {
	GJGarageLayer::onSelect(sender);
	updateGradient();
}

void ProGJGarageLayer::setupPage(int p0, IconType p1) {
	GJGarageLayer::setupPage(p0, p1);

	if (!Loader::get()->isModLoaded("ninkaz.colorful-icons")) return;

	auto f = m_fields.self();

	f->m_visibleIcons.clear();
	f->m_allIcons.clear();

	if (!m_iconSelection) return;
	if (!m_iconSelection->m_pages) return;
	if (m_iconSelection->m_pages->count() <= 0) return;
	if (!m_iconSelection->m_pages->firstObject()) return;

	std::vector<SimplePlayer*> icons;

	if (CCMenu* menu = static_cast<CCNode*>(m_iconSelection->m_pages->firstObject())->getChildByType<CCMenu>(0))
		for (CCNode* node : CCArrayExt<CCNode*>(menu->getChildren()))
			if (GJItemIcon* item = node->getChildByType<GJItemIcon>(0))
				if (SimplePlayer* icon = item->getChildByType<SimplePlayer>(0))
					icons.push_back(icon);

	f->m_allIcons = icons;
	
	if (icons.empty()) return;

	f->m_visibleIcons.push_back(icons[0]);

	if (icons.size() >= 12)
		f->m_visibleIcons.push_back(icons[11]);

	if (icons.size() >= 13)
		f->m_visibleIcons.push_back(icons[12]);

	if (icons.size() >= 24)
		f->m_visibleIcons.push_back(icons[23]);

	if (icons.size() >= 25)
		f->m_visibleIcons.push_back(icons[24]);

	if (icons.size() >= 36)
		f->m_visibleIcons.push_back(icons[35]);
}