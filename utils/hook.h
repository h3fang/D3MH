#ifndef HOOK_H
#define HOOK_H

#include <vector>
#include <atomic>

#include <windows.h>

const unsigned long INJECTED_EVENT = 0xFFFFC0DE;

extern std::vector<MSLLHOOKSTRUCT> mouse_points;
extern std::atomic<bool> block_mouse_events;
extern std::atomic<bool> block_keyboard_events;

class Hook
{
public:
    Hook();
    ~Hook();

    void install();
};

#endif // HOOK_H
