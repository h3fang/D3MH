#ifndef ENIGMA_KEYBOARDMOUSE_H
#define ENIGMA_KEYBOARDMOUSE_H

#include <random>
#include <vector>

#include <windows.h>

#include "point.h"

const float MOUSE_SAMPLING_TIME = 15.0f; // ms
const float MOUSE_AVERAGE_SPEED = 20.0f;
const float dr = 0.01;

enum class MouseKey {
    LEFT_BUTTON,
    RIGHT_BUTTON,
    MIDDLE_BUTTON
};

class KeyboardMouse {
public:
    float screen_width;
    float screen_height;

public:
    KeyboardMouse();
    ~KeyboardMouse();

    bool keyClick(int vk);
    bool keyPress(int vk);
    bool keyRelease(int vk);

    bool mouseMove(bool absolute = true);
    bool mouseMove(Point target, bool absolute = true);
    bool mouseClick(MouseKey key);

private:
    bool injectMouseEvent(Point pos, unsigned long flags);
    bool injectKeyboardEvent(int vk, unsigned long flags);

    std::vector<Point> cubicBezierCurve(const Point& p0, const Point& p1, const Point& p2, const Point& p3);

private:
    std::default_random_engine generator;
    std::normal_distribution<float> norm_dist;
    std::uniform_real_distribution<float> uni_dist;

    // table for velocity
    std::vector<double> v;
};


#endif // ENIGMA_KEYBOARDMOUSE_H
