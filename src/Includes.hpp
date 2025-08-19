#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

const int MOD_DISABLED = 1;
const int P2_DISABLED = 2;
const int P2_FLIP = 3;
const int MENU_GRADIENTS = 4;
const int P2_SEPARATE = 5;

enum SpriteType {
    Icon = 1,
    Vehicle = 2,
    Animation = 3
};

enum ColorType {
    Main = 1,
    Secondary = 2,
    Glow = 3,
    White = 4,
    // Line = 5,
};

struct SimplePoint {
    CCPoint pos;
    ccColor3B color;

    bool operator==(const SimplePoint& other) const {
        return pos.equals(other.pos) && color.r == other.color.r &&
               color.g == other.color.g && color.b == other.color.b;
    }
};

namespace std {
    template <>
    struct hash<SimplePoint> {
        size_t operator()(const SimplePoint& point) const {
            return hash<float>()(point.pos.x) ^ hash<float>()(point.pos.y) ^
                   hash<int>()(point.color.r) ^ hash<int>()(point.color.g) ^
                   hash<int>()(point.color.b);
        }
    };
}

struct GradientConfig {

    std::vector<SimplePoint> points;
    bool isLinear = true;
    // bool isSecondary = false;

    bool operator==(const GradientConfig& other) const {
        return isLinear == other.isLinear && points == other.points/* && isSecondary == other.isSecondary*/;
    }

};

struct Gradient {

    GradientConfig main;
    GradientConfig secondary;
    GradientConfig glow;
    GradientConfig white;

};

class FakeSpriteBatchNode : public CCSpriteBatchNode {

public:

    void draw() override {
        CCNode::draw();
    }

    void visit() override {
        CCNode::visit();
    }
    
};
