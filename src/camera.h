#ifndef CAMERA_H
#define CAMERA_H

#include "vector_util.h"
#include <SFML/System.hpp>

class Camera {
public:
    Camera(const sf::Vector2f& windowSize, const sf::Vector2f& viewCenterWorld,
           float worldUnitsPerPixel = 1.f)
        : mWindowSize{windowSize},
          mViewCenterWorld{viewCenterWorld},
          mWorldUnitsPerPixel{worldUnitsPerPixel} {}

    // TODO: all of these:

    sf::Vector2f windowToWorld(const sf::Vector2f& pWindow) {
        sf::Vector2f offsetPixels = pWindow - (mWindowSize / 2.f);
        sf::Vector2f offsetWorld = offsetPixels * mWorldUnitsPerPixel;
        auto pWorld = mViewCenterWorld + offsetWorld;
        return pWorld;
    };

    sf::Vector2f worldToWindow(const sf::Vector2f& pWorld) {
        sf::Vector2f offsetWorld = pWorld - mViewCenterWorld;
        sf::Vector2f offsetPixels = offsetWorld / mWorldUnitsPerPixel;
        return offsetPixels + (mWindowSize / 2.f);
    };

    sf::Vector2f windowPercentToWorld(const sf::Vector2f& pWindowPercent) {
        sf::Vector2f pWindow = mWindowSize * pWindowPercent;
        return windowToWorld(pWindow);
    };

    sf::Vector2f worldToWindowPercent(const sf::Vector2f& pWorld) {
        auto pixel = worldToWindow(pWorld);
        return pixel / mWindowSize;
    };

    void translateWorld(const sf::Vector2f& deltaWorld) { mViewCenterWorld += deltaWorld; };

    void translateWindow(const sf::Vector2f& deltaScreen) {
        auto deltaWorld = deltaScreen * mWorldUnitsPerPixel;
        translateWorld(deltaWorld);
    };

    void translateWindowPercent(const sf::Vector2f& deltaScreenPercent) {
        auto deltaPixels = deltaScreenPercent * mWindowSize;
        translateWindow(deltaPixels);
    };

    void setCamera(const sf::Vector2f& pWorld, const sf::Vector2f& pWindow) {
        auto current = windowToWorld(pWindow);
        auto difference = pWorld - current;
        translateWorld(difference);
    };

    void setCameraPercent(const sf::Vector2f& pWorld, const sf::Vector2f& pWindowPercent) {
        auto current = windowPercentToWorld(pWindowPercent);
        auto difference = pWorld - current;
        translateWorld(difference);
    };

    void setCameraCenter(const sf::Vector2f& pWorld) { mViewCenterWorld = pWorld; };

    sf::Vector2f getCameraCenter() const { return mViewCenterWorld; };

private:
    sf::Vector2f mWindowSize;       // size of the window in screen pixels (i.e. window coords)
    sf::Vector2f mViewCenterWorld;  // world coords point corresponding to the center of our window.
    float mWorldUnitsPerPixel;      // distance in world coords covered by one pixel
};

#endif  // CAMERA_H
