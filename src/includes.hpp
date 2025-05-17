#pragma once

using namespace geode::prelude;

struct SimplePoint {

    cocos2d::CCPoint pos;
    cocos2d::ccColor3B color;

};

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