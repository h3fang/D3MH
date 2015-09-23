#include <windows.h>
#include <gdiplus.h>

#include <stdio.h>
#include <string.h>
#include <vector>

#include "engine/engine.h"
#include "process/pointer.h"
#include "process/helper.h"

using namespace Gdiplus;

float CANVAS_WIDTH = 1500.0f;
const float CANVAS_HEIGHT = 1200.0f;

HWND hWnd;
HWND d3Window = NULL;
Rect windowPos = {0,0,800,800/5*4};
int hotkey_id;
bool draw_minimap = true;

Engine *engine;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

bool registerHotKeys()
{
    hotkey_id = GlobalAddAtomA("D3 MapHack");
    return RegisterHotKey(hWnd, hotkey_id, 0, VK_OEM_3) == TRUE;
}

bool getD3ClientRect(Rect &r)
{
    if (!d3Window) {
        d3Window = FindMainWindow(MemoryReader::instance()->getProcessId());
    }

    if (!d3Window) {
        printf("Failed to find D3 window");
        return false;
    }

    RECT rect;
    if (!GetClientRect(d3Window, &rect)) {
        return false;
    }

    ClientToScreen(d3Window, reinterpret_cast<POINT*>(&rect.left));
    ClientToScreen(d3Window, reinterpret_cast<POINT*>(&rect.right));

    r.X = rect.left;
    r.Y = rect.top;
    r.Width = rect.right - rect.left;
    r.Height = rect.bottom - rect.top;

    return true;
}

void repositionWindow() {
    Rect r;
    if (getD3ClientRect(r)){
        if(r.X != windowPos.X ||
                r.Y != windowPos.Y ||
                r.Width != windowPos.Width ||
                r.Height != windowPos.Height) {
            SetWindowPos(hWnd, HWND_TOPMOST, r.X, r.Y, r.Width, r.Height, 0);

            CANVAS_WIDTH = float(r.Width)/(r.Height)*CANVAS_HEIGHT;
        }
    }
}

void drawInfo(Graphics *p)
{
//    static Pen greenPen(Color(0, 255, 0));
    static SolidBrush brush(Color(0, 255, 0));
    static Font font(L"Arial", 2);

    StringFormat stringformat;
    stringformat.SetAlignment(StringAlignmentCenter);
    stringformat.SetLineAlignment(StringAlignmentCenter);

    wchar_t str[256];
    swprintf(str, L"FrameCnt:%u\nAppLoopCnt:%u\nWorldSnoId:%d\nX:%.4f\nY:%.4f\nZ:%.4f\nSceneCnt:%u",
             Pointer<D3::DWORD>()(Addr_ObjectManager, offsetof(D3::ObjectManager,x038_Counter_CurrentFrame)),
             engine->ApplicationLoopCount,
             engine->localData.x0C_WorldSnoId,
             engine->localData.x24_WorldPosX,
             engine->localData.x28_WorldPosY,
             engine->localData.x2C_WorldPosZ,
             engine->navMesh->sceneData.size());

    p->DrawString(str, wcslen(str), &font,
                  RectF(0, 0, 0.2*CANVAS_WIDTH, 0.4*CANVAS_HEIGHT),
                  &stringformat, &brush);
}

void drawCoordinates(Graphics *p)
{
    Pen redPen(Color(255, 0, 0));
    redPen.SetWidth(3);
    p->DrawLine(&redPen, 0, 0, 200, 0);

    Pen bluePen(Color(0, 0, 255));
    bluePen.SetWidth(3);
    p->DrawLine(&bluePen, 0, 0, 0, 200);
}

void drawMinimap(Graphics *p){
    if (!draw_minimap) {
        return;
    }

    auto state = p->Save();

    p->ScaleTransform((windowPos.Width)/CANVAS_WIDTH, (windowPos.Height)/CANVAS_HEIGHT);
    p->TranslateTransform(CANVAS_WIDTH/2,CANVAS_HEIGHT/2);
    p->RotateTransform(-45.0);
    p->ScaleTransform(-1.0, 1.0);

    drawCoordinates(p);

    p->TranslateTransform(-engine->localData.x24_WorldPosX, -engine->localData.x28_WorldPosY);


    std::vector<RectF> scene_grids, scene_cells;
    scene_grids.reserve(engine->navMesh->sceneData.size());
    scene_cells.reserve(100*engine->navMesh->sceneData.size());


    for (auto &pair : engine->navMesh->sceneData) {
        D3::SceneData *s = pair.second;

        scene_grids.push_back(RectF(s->min.x, s->min.y, s->max.x-s->min.x, s->max.y-s->min.y));

        for (D3::NavCell &c: s->cells) {
            scene_cells.push_back(RectF(c.min.x+s->min.x, c.min.y+s->min.y, c.max.x-c.min.x, c.max.y-c.min.y));
        }
    }

    Pen greenPen(Color(0, 255, 0));
    SolidBrush greenBrush(Color(64, 0, 255, 0));
    p->DrawRectangles(&greenPen, scene_grids.data(), scene_grids.size());
    p->FillRectangles(&greenBrush, scene_grids.data(), scene_grids.size());

    Pen bluePen(Color(0, 0, 255));
    SolidBrush blueBrush(Color(64, 0, 0, 255));
    p->DrawRectangles(&bluePen, scene_cells.data(), scene_cells.size());
    p->FillRectangles(&blueBrush, scene_cells.data(), scene_cells.size());

    p->Restore(state);
}

void OnPaint(HDC hdc)
{
    if (!d3Window || d3Window != GetForegroundWindow()) {
        return;
    }

    Graphics g(hdc);
    drawInfo(&g);
    drawMinimap(&g);
}

void CALLBACK OnTimer(HWND /*hwnd*/, UINT /*message*/, UINT /*idTimer*/, DWORD /*dwTime*/)
{
    repositionWindow();

    if (!d3Window || d3Window != GetForegroundWindow()) {
        return;
    }
    engine->update();
    InvalidateRect(hWnd,NULL,TRUE);
}

void OnQuit() {
    if (d3Window) {
        CloseHandle(d3Window);
    }
    UnregisterHotKey(hWnd, hotkey_id);
}

void init() {
    setbuf(stderr, NULL);
    engine = Engine::getInstance();
    registerHotKeys();
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
    init();

    WNDCLASSEX wcex;
    ZeroMemory(&wcex, sizeof(wcex));

    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;

    // Initialize GDI+.
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    wcex.cbSize         = sizeof(WNDCLASSEX);
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.hInstance		= hInstance;
    wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground	= (HBRUSH)GetStockObject(NULL_BRUSH);
    wcex.lpszMenuName	= NULL;
    wcex.lpszClassName	= L"minwindowsapp";

    if( !RegisterClassEx(&wcex) )
        return 1;

    hWnd = CreateWindowEx(WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TRANSPARENT,
                          wcex.lpszClassName, L"D3MH",
                          WS_POPUPWINDOW,
                          windowPos.X, windowPos.Y,
                          windowPos.Width, windowPos.Height,
                          NULL, NULL, hInstance, NULL);
    if( !hWnd) {
        return 2;
    }

    SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
    SetLayeredWindowAttributes(hWnd, RGB(255,255,255), (255 * 0) / 100, LWA_COLORKEY);

    SetWindowPos(hWnd, HWND_TOPMOST,
                 windowPos.X, windowPos.Y,
                 windowPos.Width,
                 windowPos.Height, 0);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    unsigned int timerId = 1;

    SetTimer(hWnd, timerId, 100, OnTimer);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    KillTimer(hWnd, timerId);
    OnQuit();
    GdiplusShutdown(gdiplusToken);

    return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;

    switch(message)
    {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        OnPaint(hdc);
        EndPaint(hWnd, &ps);
        break;
    case WM_HOTKEY:
        draw_minimap = !draw_minimap;
        break;
    case WM_DESTROY:
    case WM_CLOSE:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
