#include "hook.h"

#include <QDebug>

std::vector<MSLLHOOKSTRUCT> mouse_points;
std::atomic<bool> block_mouse_events;
std::atomic<bool> block_keyboard_events;

HHOOK llmh, llkbh;

LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode < 0) {
        return CallNextHookEx(llmh, nCode, wParam, lParam);
    }

    MSLLHOOKSTRUCT *p = (MSLLHOOKSTRUCT*)lParam;

//    qDebug("MOUSE - type: 0x%X, x: %ld, y: %ld, flags: %ld, time: %ld", wParam, p->pt.x, p->pt.y, p->flags, p->time);

    // block real input while injecting
    if (!(p->flags & LLMHF_INJECTED) && block_mouse_events) {
        return 1;
    }

    if (p->dwExtraInfo == INJECTED_EVENT) {
        p->flags &= ~LLMHF_INJECTED;
        p->dwExtraInfo = 0;
    }

//    mouse_points.push_back(*p);

    return CallNextHookEx(llmh, nCode, wParam, lParam);
}

LRESULT CALLBACK LowLevelKeybdProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode < 0)
        return CallNextHookEx(llkbh, nCode, wParam, lParam);

    KBDLLHOOKSTRUCT *p = (KBDLLHOOKSTRUCT*)lParam;

//    qDebug("KEYBOARD - type: 0x%X, vk:%lu, sc:%lu, flags: %ld, time: %ld", wParam, p->vkCode, p->scanCode, p->flags, p->time);

    // block real input while injecting
    if (!(p->flags & LLKHF_INJECTED) && block_keyboard_events) {
        return 1;
    }

    if (p->dwExtraInfo == INJECTED_EVENT) {
        p->flags &= ~LLKHF_INJECTED;
        p->dwExtraInfo = 0;
    }

    return CallNextHookEx(llkbh, nCode, wParam, lParam);
}

Hook::Hook()
{
    block_mouse_events = false;
    block_keyboard_events = false;

    mouse_points.reserve(200);

    install();
}

Hook::~Hook()
{
    if (!UnhookWindowsHookEx(llmh)) {
        qDebug("Unable to unhook WH_MOUSE_LL");
    }

    if (!UnhookWindowsHookEx(llkbh)) {
        qDebug("Unable to unhook WH_KEYBOARD_LL");
    }
}

void Hook::install()
{
    HINSTANCE hInstance = (HINSTANCE)::GetModuleHandle(NULL);

    llmh = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, hInstance, 0);

    if (!llmh) {
        qDebug("Unable to hook WH_MOUSE_LL");
    }

    llkbh = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeybdProc, hInstance, 0);

    if (!llkbh) {
        qDebug("Unable to hook WH_KEYBOARD_LL");
    }
}
