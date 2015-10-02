#include "minimapoverlay.h"

#include <stdio.h>
#include <string.h>
#include <vector>

#include <GL/gl.h>
#include <GL/glu.h>

#include "engine/engine.h"
#include "process/pointer.h"
#include "process/helper.h"

MinimapOverlay::MinimapOverlay() :
    LayeredWindow(),
    d3Window(NULL),
    minimapHotkeyId(0),
    draw_minimap(true),
    engine(NULL)
{
}

MinimapOverlay::~MinimapOverlay()
{
}

void MinimapOverlay::onInit()
{
    setbuf(stderr, NULL);
    engine = Engine::getInstance();

    minimapHotkeyId = GlobalAddAtomW(L"D3 MapHack");
    if (!RegisterHotKey(hWnd, minimapHotkeyId, 0, VK_OEM_3)) {

        LPVOID lpMsgBuf;
        FormatMessage(
                    FORMAT_MESSAGE_ALLOCATE_BUFFER |
                    FORMAT_MESSAGE_FROM_SYSTEM |
                    FORMAT_MESSAGE_IGNORE_INSERTS,
                    NULL,
                    GetLastError(),
                    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                    (LPTSTR) &lpMsgBuf,
                    0,
                    NULL
                    );

        fwprintf(stderr, L"Failed to register hotkey: %s\n",  lpMsgBuf);

        LocalFree( lpMsgBuf );
        return;
    }

    CANVAS_WIDTH = float(windowPos.right-windowPos.top)/(windowPos.bottom-windowPos.top)*CANVAS_HEIGHT;

    SetTimer(hWnd, 1, 100, NULL);

    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
}

void MinimapOverlay::onQuit()
{
    KillTimer(hWnd, 1);

    if (d3Window) {
        CloseHandle(d3Window);
    }
    UnregisterHotKey(hWnd, minimapHotkeyId);
}

void MinimapOverlay::onRender()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glTranslatef((windowPos.right-windowPos.left)/CANVAS_WIDTH, (windowPos.bottom-windowPos.top)/CANVAS_HEIGHT, 0.0f);

    drawInfo();
    drawMinimap();
}

void MinimapOverlay::onTimer(UINT timerId)
{
    switch (timerId) {
    case 1:
        repositionWindow();

        if (d3Window && d3Window == GetForegroundWindow()) {
            engine->update();
        }

        onRender();
        break;
    default:
        break;
    }
}

bool MinimapOverlay::getD3ClientRect(RECT &rect)
{
    if (!d3Window) {
        d3Window = FindMainWindow(MemoryReader::instance()->getProcessId());
    }

    if (!d3Window) {
        fprintf(stderr, "Failed to find D3 window\n");
        return false;
    }

    if (!GetClientRect(d3Window, &rect)) {
        return false;
    }

    ClientToScreen(d3Window, reinterpret_cast<POINT*>(&rect.left));
    ClientToScreen(d3Window, reinterpret_cast<POINT*>(&rect.right));

    return true;
}

void MinimapOverlay::repositionWindow() {
    RECT r;
    if (getD3ClientRect(r)){
        if(r.left != windowPos.left || r.top != windowPos.top ||
                r.right != windowPos.right || r.bottom != windowPos.bottom) {
            SetWindowPos(hWnd, HWND_TOPMOST, r.left, r.top, r.right-r.left, r.bottom-r.top, 0);
            windowPos = r;
            CANVAS_WIDTH = float(r.right-r.left)/(r.bottom-r.top)*CANVAS_HEIGHT;
        }
    }
}

void MinimapOverlay::drawInfo()
{
    if (!d3Window || d3Window != GetForegroundWindow()) {
        return;
    }

//    SolidBrush brush(Color(255, 0, 255, 0));
//    Font font(L"Consolas", 16);

    char str[256];
    sprintf(str, "FrameCnt:%u\nAppLoopCnt:%u\nWorldSnoId:%d\nX:%.4f\nY:%.4f\nZ:%.4f\nSceneCnt:%u",
             Pointer<D3::DWORD>()(Addr_ObjectManager, offsetof(D3::ObjectManager,x038_Counter_CurrentFrame)),
             engine->ApplicationLoopCount,
             engine->localData.x0C_WorldSnoId,
             engine->localData.x24_WorldPosX,
             engine->localData.x28_WorldPosY,
             engine->localData.x2C_WorldPosZ,
             engine->navMesh->sceneData.size());

//    p->DrawString(str, wcslen(str), &font,
//                  RectF(0, 0, 0.2*CANVAS_WIDTH, 0.4*CANVAS_HEIGHT),
//                  &stringformat, &brush);
}

void MinimapOverlay::drawCoordinates()
{
    glColor3f(255, 0, 0);
    glLineWidth(3);

    glBegin(GL_LINE);
        glVertex2f(0, 0);
        glVertex2f(200, 0);
    glEnd();

    glColor3f(0, 0, 255);
    glLineWidth(3);

    glBegin(GL_LINE);
        glVertex2f(0, 0);
        glVertex2f(0, 200);
    glEnd();
}

void MinimapOverlay::drawMinimap()
{
    if (!draw_minimap || !d3Window || d3Window != GetForegroundWindow()) {
        return;
    }

    glPushMatrix();

    glTranslatef(CANVAS_WIDTH/2,CANVAS_HEIGHT/2, 0.0f);
    glRotatef(-45.0f, 0.0f, 0.0f, 0.0f);
    glScalef(-1.0f, 1.0f, 0.0f);

    drawCoordinates();

    glTranslatef(-engine->localData.x24_WorldPosX, -engine->localData.x28_WorldPosY, 0.0f);

//    std::vector<RectF> scene_grids, scene_cells;
//    scene_grids.reserve(engine->navMesh->sceneData.size());
//    scene_cells.reserve(100*engine->navMesh->sceneData.size());


//    for (auto &pair : engine->navMesh->sceneData) {
//        D3::SceneData *s = pair.second;

//        scene_grids.push_back(RectF(s->min.x, s->min.y, s->max.x-s->min.x, s->max.y-s->min.y));

//        for (D3::NavCell &c: s->cells) {
//            scene_cells.push_back(RectF(c.min.x+s->min.x, c.min.y+s->min.y, c.max.x-c.min.x, c.max.y-c.min.y));
//        }
//    }

////    Pen greenPen(Color(0, 255, 0));
////    SolidBrush greenBrush(Color(8, 0, 64, 0));
////    p->DrawRectangles(&greenPen, scene_grids.data(), scene_grids.size());
////    p->FillRectangles(&greenBrush, scene_grids.data(), scene_grids.size());

//    glColor3f(0, 0, 255);
//    SolidBrush blueBrush(Color(64, 0, 0, 255));
//    p->DrawRectangles(&bluePen, scene_cells.data(), scene_cells.size());
//    p->FillRectangles(&blueBrush, scene_cells.data(), scene_cells.size());

    glPopMatrix();
}
