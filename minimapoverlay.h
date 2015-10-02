#ifndef D3MH_MINIMAPOVERLAY_H
#define D3MH_MINIMAPOVERLAY_H

#include "layered/layeredwindow.h"

class Engine;

class MinimapOverlay : public LayeredWindow
{
public:
    MinimapOverlay();
    ~MinimapOverlay();

protected:
    void onInit() override;
    void onQuit() override;
    void onRender() override;
    void onTimer(UINT timerId) override;

private:
    bool getD3ClientRect(RECT &rect);
    void repositionWindow();

    void drawInfo();
    void drawCoordinates();
    void drawMinimap();

private:
    const float CANVAS_HEIGHT = 1200.0f;
    float CANVAS_WIDTH;

    HWND d3Window;
    int minimapHotkeyId;
    bool draw_minimap;

    Engine *engine;
};

#endif // D3MH_MINIMAPOVERLAY_H
