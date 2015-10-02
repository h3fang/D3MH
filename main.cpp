#include "minimapoverlay.h"

int WINAPI WinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int /*nCmdShow*/)
{
    MinimapOverlay m;

    if (SUCCEEDED(m.initialize())) {
        m.runMessageLoop();
    }
    else {
        return 1;
    }

    return 0;
}
