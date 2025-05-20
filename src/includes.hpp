#pragma once

using namespace geode::prelude;

struct SimplePoint {
    cocos2d::CCPoint pos;
    cocos2d::ccColor3B color;

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
    bool isLinear;

};

struct Gradient {

    GradientConfig mainColor;
    GradientConfig secondaryColor;

};

class PointsLayerDelegate {

public:

    virtual void pointMoved() {}

    virtual void pointSelected(CCNode*) {}

};