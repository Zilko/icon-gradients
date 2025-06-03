#include "ColorSelectLayer.hpp"
#include "GradientLayer.hpp"

ColorSelectLayer* ColorSelectLayer::create(GradientLayer* layer) {
    ColorSelectLayer* ret = new ColorSelectLayer();

    ret->m_layer = layer;

    if (ret->initAnchored(450, 245, "GJ_square05.png")) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

void ColorSelectLayer::onColor(CCObject* sender) {
    if (m_layer)
        m_layer->colorSelected(
            static_cast<CCSprite*>(static_cast<CCMenuItemSpriteExtra*>(sender)->getNormalImage())->getColor()
        );

    onClose(nullptr);
}

void ColorSelectLayer::createButton(int color, const cocos2d::CCPoint& pos) {
    CCSprite* spr = CCSprite::createWithSpriteFrameName("GJ_colorBtn_001.png");
    spr->setColor(GameManager::get()->colorForIdx(color));
    spr->setScale(0.65f);

    CCMenuItemSpriteExtra* btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(ColorSelectLayer::onColor));
    btn->setPosition(pos + ccp(10, -23));
    m_buttonMenu->addChild(btn);
}

bool ColorSelectLayer::setup() {
    setTitle("Select Color");

    createButton(51, ccp(17, 217));
    createButton(19, ccp(41, 217));
    createButton(48, ccp(65, 217));
    createButton(9, ccp(89, 217));
    createButton(62, ccp(125, 217));
    createButton(63, ccp(149, 217));
    createButton(10, ccp(173, 217));
    createButton(29, ccp(197, 217));
    createButton(70, ccp(233, 217));
    createButton(42, ccp(257, 217));
    createButton(11, ccp(281, 217));
    createButton(27, ccp(305, 217));
    createButton(72, ccp(341, 217));
    createButton(73, ccp(365, 217));
    createButton(0, ccp(389, 217));
    createButton(1, ccp(413, 217));
    createButton(37, ccp(17, 193));
    createButton(53, ccp(41, 193));
    createButton(54, ccp(65, 193));
    createButton(55, ccp(89, 193));
    createButton(26, ccp(125, 193));
    createButton(59, ccp(149, 193));
    createButton(60, ccp(173, 193));
    createButton(61, ccp(197, 193));
    createButton(71, ccp(233, 193));
    createButton(14, ccp(257, 193));
    createButton(31, ccp(281, 193));
    createButton(45, ccp(305, 193));
    createButton(105, ccp(341, 193));
    createButton(28, ccp(365, 193));
    createButton(32, ccp(389, 193));
    createButton(20, ccp(413, 193));
    createButton(25, ccp(17, 169));
    createButton(56, ccp(41, 169));
    createButton(57, ccp(65, 169));
    createButton(58, ccp(89, 169));
    createButton(30, ccp(125, 169));
    createButton(64, ccp(149, 169));
    createButton(65, ccp(173, 169));
    createButton(66, ccp(197, 169));
    createButton(46, ccp(233, 169));
    createButton(67, ccp(257, 169));
    createButton(68, ccp(281, 169));
    createButton(69, ccp(305, 169));
    createButton(2, ccp(341, 169));
    createButton(38, ccp(365, 169));
    createButton(79, ccp(389, 169));
    createButton(80, ccp(413, 169));
    createButton(74, ccp(17, 135.4));
    createButton(75, ccp(41, 135.4));
    createButton(44, ccp(65, 135.4));
    createButton(3, ccp(89, 135.4));
    createButton(83, ccp(125, 135.4));
    createButton(16, ccp(149, 135.4));
    createButton(4, ccp(173, 135.4));
    createButton(5, ccp(197, 135.4));
    createButton(52, ccp(233, 135.4));
    createButton(41, ccp(257, 135.4));
    createButton(6, ccp(281, 135.4));
    createButton(35, ccp(305, 135.4));
    createButton(98, ccp(341, 135.4));
    createButton(8, ccp(365, 135.4));
    createButton(36, ccp(389, 135.4));
    createButton(103, ccp(413, 135.4));
    createButton(40, ccp(17, 111.399994));
    createButton(76, ccp(41, 111.399994));
    createButton(77, ccp(65, 111.399994));
    createButton(78, ccp(89, 111.399994));
    createButton(22, ccp(125, 111.399994));
    createButton(39, ccp(149, 111.399994));
    createButton(84, ccp(173, 111.399994));
    createButton(50, ccp(197, 111.399994));
    createButton(47, ccp(233, 111.399994));
    createButton(23, ccp(257, 111.399994));
    createButton(92, ccp(281, 111.399994));
    createButton(93, ccp(305, 111.399994));
    createButton(7, ccp(341, 111.399994));
    createButton(13, ccp(365, 111.399994));
    createButton(24, ccp(389, 111.399994));
    createButton(104, ccp(413, 111.399994));
    createButton(33, ccp(17, 87.399994));
    createButton(21, ccp(41, 87.399994));
    createButton(81, ccp(65, 87.399994));
    createButton(82, ccp(89, 87.399994));
    createButton(34, ccp(125, 87.399994));
    createButton(85, ccp(149, 87.399994));
    createButton(86, ccp(173, 87.399994));
    createButton(87, ccp(197, 87.399994));
    createButton(49, ccp(233, 87.399994));
    createButton(95, ccp(257, 87.399994));
    createButton(96, ccp(281, 87.399994));
    createButton(97, ccp(305, 87.399994));
    createButton(43, ccp(341, 87.399994));
    createButton(99, ccp(365, 87.399994));
    createButton(100, ccp(389, 87.399994));
    createButton(101, ccp(413, 87.399994));
    createButton(106, ccp(125, 63.399994));
    createButton(88, ccp(149, 63.399994));
    createButton(89, ccp(173, 63.399994));
    createButton(90, ccp(197, 63.399994));
    createButton(12, ccp(269, 53.799988));
    createButton(91, ccp(293, 53.799988));
    createButton(17, ccp(317, 53.799988));
    createButton(102, ccp(341, 53.799988));
    createButton(18, ccp(365, 53.799988));
    createButton(94, ccp(389, 53.799988));
    createButton(15, ccp(413, 53.799988));  

    return true;
}