#include "keyboardmouse.h"

#include <ctime>
#include <cmath>

#include <QDebug>

#include "hook.h"

template <typename T>
T clamp(const T& n, const T& lower, const T& upper) {
  return std::max(lower, std::min(n, upper));
}

KeyboardMouse::KeyboardMouse() :
    generator(std::default_random_engine(time(nullptr))),
    norm_dist(std::normal_distribution<float>(0,1)),
    uni_dist(std::uniform_real_distribution<float>(0,1))
{
    screen_width = GetSystemMetrics(SM_CXSCREEN);
    screen_height = GetSystemMetrics(SM_CYSCREEN);

    // polynomial fitting for
    // V_MAX = 25;
    // r = [0 0.15 0.5 0.85 1];
    // v = [0 0.9*V_MAX 1.1*V_MAX 0.9*V_MAX 0];
    // v(r) = p1*r^4 + p2*r^3 + p3*r^2 + p4*r + p5
    // Coefficients:
    const double p1 =      -542.617046818719;
    const double p2 =       1085.23409363744;
    const double p3 =      -788.271308523397;
    const double p4 =       245.654261704679;
    const double p5 =   1.83104695897732e-13;

    v.reserve(std::round(1.0f/dr) + 1);
    for (double r=0.0; r < 1.01; r+=dr) {
        v.push_back(p1*r*r*r*r + p2*r*r*r + p3*r*r + p4*r +p5);
    }
}

KeyboardMouse::~KeyboardMouse()
{

}

bool KeyboardMouse::keyClick(int vk)
{
    injecting_keyboard_events = true;

    if (!keyPress(vk)) {
        injecting_keyboard_events = false;
        return false;
    }

    Sleep(clamp(80 + norm_dist(generator)*5, 60.0f, 100.0f));

    if (!keyRelease(vk)) {
        injecting_keyboard_events = false;
        return false;
    }

    injecting_keyboard_events = false;
    return true;
}

bool KeyboardMouse::keyPress(int vk)
{
    // if already pressed, return false, to prevent further release
    if (GetAsyncKeyState(vk) & 0x8000) {
        return false;
    }

    return injectKeyboardEvent(vk, KEYEVENTF_SCANCODE);
}

bool KeyboardMouse::keyRelease(int vk)
{
    if (!(GetAsyncKeyState(vk) & 0x8000)) {
        return false;
    }

    return injectKeyboardEvent(vk, KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP);
}

bool KeyboardMouse::mouseMove(bool absolute)
{
    std::uniform_real_distribution<float> dist(0.4, 0.6);
    Point p(dist(generator)*screen_width, dist(generator)*screen_height);
    return mouseMove(p, absolute);
}

bool KeyboardMouse::mouseMove(Point target, bool absolute)
{
    POINT p;
    if (!GetCursorPos(&p)) {
        return false;
    }

    Point p0(p.x, p.y);

    target.x = target.x < 0 ? 0 : target.x;
    target.y = target.y < 0 ? 0 : target.y;
    target.x = target.x > (screen_width-1) ? screen_width-1 : target.x;
    target.y = target.y > (screen_height-1) ? screen_height-1 : target.y;

    if (distance(p0, target) <= 2) {return true;}

    Point normal(target.y-p0.y, p0.x-target.x);

    qDebug("p0: [%f, %f] , p3: [%f, %f]", p0.x, p0.y, target.x, target.y);

    float r1 = 0.7 + 0.1*norm_dist(generator), r2 = 0.3 + 0.1*norm_dist(generator);

    Point p1 = r1*p0 + (1-r1)*target + 0.1*norm_dist(generator)*normal;
    Point p2 = r2*p0 + (1-r2)*target + 0.1*norm_dist(generator)*normal;
    auto r = cubicBezierCurve(p0, p1, p2, target);

    injecting_mouse_events = true;

    for (size_t i=0; i<r.size(); ++i) {
        if (absolute) {
            if (!injectMouseEvent(r[i], MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE)) {
                qDebug("Failed to inject mouse event");
                return false;
            }
        }
        else {
            Point ds;

            if (i != 0) {
                ds = r[i] -r[i-1];
            }

            if (!injectMouseEvent(ds, MOUSEEVENTF_MOVE)) {
                qDebug("Failed to inject mouse event");
                return false;
            }
        }

        Sleep(MOUSE_SAMPLING_TIME + uni_dist(generator));
    }

    injecting_mouse_events = false;

    return true;
}

bool KeyboardMouse::mouseClick(MouseKey key)
{
    unsigned long flags_down = 0, flags_up = 0;
    bool key_down = false;
    switch(key) {
    case MouseKey::LEFT_BUTTON: {
        flags_down |= MOUSEEVENTF_LEFTDOWN;
        flags_up |= MOUSEEVENTF_LEFTUP;
        key_down = GetAsyncKeyState(VK_LBUTTON) & 0x8000;
        break;
    }
    case MouseKey::RIGHT_BUTTON: {
        flags_down |= MOUSEEVENTF_RIGHTDOWN;
        flags_up |= MOUSEEVENTF_RIGHTUP;
        key_down = GetAsyncKeyState(VK_RBUTTON) & 0x8000;
        break;
    }
    case MouseKey::MIDDLE_BUTTON: {
        flags_down |= MOUSEEVENTF_MIDDLEDOWN;
        flags_up |= MOUSEEVENTF_MIDDLEUP;
        key_down = GetAsyncKeyState(VK_MBUTTON) & 0x8000;
        break;
    }
    }

    if (key_down) {
        return false;
    }

    Point p;

    injecting_mouse_events = true;

    if (!injectMouseEvent(p, flags_down)) {
        return false;
    }

    Sleep(clamp(60 + 5*norm_dist(generator), 40.0f, 80.0f));

    auto r = injectMouseEvent(p, flags_up);

    injecting_mouse_events = false;
    return r;
}

bool KeyboardMouse::injectMouseEvent(Point pos, unsigned long flags)
{
    INPUT in;
    in.type = INPUT_MOUSE;

    if (flags & MOUSEEVENTF_ABSOLUTE) {
        pos.x = pos.x < 0 ? 0 : pos.x;
        pos.y = pos.y < 0 ? 0 : pos.y;
        pos.x = pos.x > (screen_width-1) ? screen_width-1 : pos.x;
        pos.y = pos.y > (screen_height-1) ? screen_height-1 : pos.y;

        in.mi.dx = pos.x/screen_width * 65536;
        in.mi.dy = pos.y/screen_height * 65536;
    }
    else {
        in.mi.dx = pos.x;
        in.mi.dy = pos.y;
    }

    in.mi.mouseData = 0;
    in.mi.time = timeGetTime();
    in.mi.dwFlags = flags;
    in.mi.dwExtraInfo = INJECTED_EVENT;

    return SendInput(1, &in, sizeof(INPUT)) == 1;
}


bool KeyboardMouse::injectKeyboardEvent(int vk, unsigned long flags)
{
    INPUT input;

    input.type = INPUT_KEYBOARD;
    input.ki.wVk = vk;
    input.ki.wScan = MapVirtualKey(vk, MAPVK_VK_TO_VSC);
    input.ki.dwFlags = flags;
    input.ki.time = timeGetTime();
    input.ki.dwExtraInfo = INJECTED_EVENT;

    return SendInput(1, &input, sizeof (INPUT)) == 1;
}

std::vector<Point> KeyboardMouse::cubicBezierCurve(const Point &p0, const Point &p1, const Point &p2, const Point &p3)
{
    std::vector<Point> result;
    result.reserve(50);
    result.push_back(p0);

    float r = 0.0f, u = 1 - r, dt = 0.0f;

    Point last_p = p0;

    while (true) {
        r += dr;

        if (r > 1.0f) {
            break;
        }

        u = 1 - r;

        // cubic bezier curve
        Point p = u*u*u * p0 + 3 * u*u * r * p1 + 3 * u * r*r * p2 + r*r*r * p3;

        dt += distance(p, last_p) / ((v[std::round(r/dr)]+v[std::round(r/dr) - 1]) / 2.0f);
        dt *= 25.0/MOUSE_AVERAGE_SPEED;

        if (dt >= MOUSE_SAMPLING_TIME) {
            result.push_back(p);
            dt = 0.0f;
        }

        last_p = p;
    }

    if (distance(p3, result.back()) >= 3) {
        result.push_back(p3);
    }

    return result;
}
