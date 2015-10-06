#include "layeredwindow.h"

#include <stdio.h>
#include <wchar.h>
#include <string.h>

#include <glm/ext.hpp>

LayeredWindow::LayeredWindow() :
    hWnd(NULL),
    hDC(NULL),
    m_info(nullptr),
    m_bitmap(nullptr),
    initialized(false)
{

}

LayeredWindow::~LayeredWindow()
{
    if (m_info) {
        delete m_info;
    }

    if (m_bitmap) {
        delete m_bitmap;
    }

    if (hDC) {
        ReleaseDC(hWnd, hDC);
    }
}

HRESULT LayeredWindow::initialize()
{
    HRESULT hr;

    hr = initWindow();

    if (SUCCEEDED(hr))
    {
        m_info = new LayeredWindowInfo(windowPos.right - windowPos.left, windowPos.bottom - windowPos.top);
        m_bitmap = new LayeredBitmap(windowPos.right - windowPos.left, windowPos.bottom - windowPos.top);

        hDC = GetDC(hWnd);

        if (SUCCEEDED(initGL())) {
            initialized = true;
            onInit();
        }
    }

    return hr;
}

void LayeredWindow::runMessageLoop()
{
    MSG msg;

    while (GetMessage(&msg, NULL, 0, 0))
    {
        // handle global hotkey event
        if( msg.message == WM_HOTKEY) {
            onHotkey(msg.lParam);
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

HRESULT LayeredWindow::initWindow()
{
    HRESULT hr = S_OK;

    WNDCLASSEX wcex;
    ZeroMemory(&wcex, sizeof(WNDCLASSEX));
    wcex.cbSize         = sizeof(WNDCLASSEX);
    wcex.style         = CS_OWNDC;
    wcex.lpfnWndProc   = LayeredWindow::WndProc;
    wcex.cbClsExtra    = 0;
    wcex.cbWndExtra    = sizeof(LONG_PTR);
    wcex.hInstance     = HINST_THISCOMPONENT;
    wcex.hbrBackground = NULL;
    wcex.lpszMenuName  = NULL;
    wcex.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wcex.lpszClassName = L"DirectXWin32";

    if (!RegisterClassEx(&wcex)) {
        return E_FAIL;
    }

    windowPos.left = 0;
    windowPos.top = 0;
    windowPos.right = GetSystemMetrics(SM_CXSCREEN);
    windowPos.bottom = GetSystemMetrics(SM_CYSCREEN);

    hWnd = CreateWindowEx(WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TRANSPARENT,
                          wcex.lpszClassName, L"D3MH",
                          WS_POPUP,
                          windowPos.left, windowPos.top,
                          windowPos.right - windowPos.left,
                          windowPos.bottom - windowPos.top,
                          NULL, NULL, HINST_THISCOMPONENT, this);

    if (hWnd) {
        ShowWindow(hWnd, SW_SHOWNORMAL);
    }
    else {
        hr = E_FAIL;
    }

    return hr;
}

HRESULT LayeredWindow::initGL()
{
    HRESULT hr = S_OK;
    if (!createOpenGLContext()) {
        hr = E_FAIL;
    }
    else {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    return hr;
}

void LayeredWindow::resizeGL(UINT width, UINT height)
{
    glViewport(0, 0, width, height);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    projectionMatrix = glm::ortho(0u, width, 0u, height);

    modelViewMatrix = glm::mat4(1.0);
}

bool LayeredWindow::createOpenGLContext()
{
    // create a temporary context to load extensions

    PIXELFORMATDESCRIPTOR pfd =
    {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL,    //Flags
        PFD_TYPE_RGBA,            //The kind of framebuffer. RGBA or palette.
        32,                        //Colordepth of the framebuffer.
        0, 0, 0, 0, 0, 0,
        0,
        0,
        0,
        0, 0, 0, 0,
        24,                        //Number of bits for the depthbuffer
        8,                        //Number of bits for the stencilbuffer
        0,                        //Number of Aux buffers in the framebuffer.
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };

    int pf = ChoosePixelFormat(hDC, &pfd);

    if (!SetPixelFormat(hDC, pf, &pfd)) {
        return false;
    }

    HGLRC renderContext = wglCreateContext(hDC);

    if (!renderContext) {
        return false;
    }

    if (!wglMakeCurrent(hDC, renderContext)) {
        return false;
    }

    GLenum err=glewInit();
    if(err!=GLEW_OK)
    {
        //Problem: glewInit failed, something is seriously wrong.
        fprintf(stderr, "glewInit failed, aborting.\n");
    }

    fprintf(stderr, "OpenGL %s, GLSL %s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));

    return true;
}

void LayeredWindow::render() {
    onRender();

    // Copy the contents from the framebuffer to HBITMAP
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(0, 0, m_bitmap->width, m_bitmap->height, GL_BGRA_EXT, GL_UNSIGNED_BYTE, m_bitmap->bits);

    // Notice that we also need to invert the pbuffer's pixel data since OpenGL
    // by default orients the bitmap image bottom up. Our Windows DIB wrapper
    // expects images to be top down in orientation.

    // (this is done by invert the y axis)

    // pre-multiplied alpha values in the RGB channels

    // (too slow, use shader to do it)

    m_info->update(hWnd, m_bitmap->getDC());
}

LRESULT LayeredWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;

    if (message == WM_CREATE)
    {
        LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
        LayeredWindow *pApp = (LayeredWindow *)pcs->lpCreateParams;

        ::SetWindowLongPtrW(
                    hWnd,
                    GWLP_USERDATA,
                    PtrToUlong(pApp)
                    );
    }
    else
    {
        LayeredWindow *pApp = reinterpret_cast<LayeredWindow *>(static_cast<LONG_PTR>(
                                                                    ::GetWindowLongPtrW(
                                                                        hWnd,
                                                                        GWLP_USERDATA
                                                                        )));

        if (pApp && pApp->isInitialized())
        {
            switch (message)
            {
            case WM_ERASEBKGND:
            case WM_PAINT:
                break;
            case WM_TIMER:
                pApp->onTimer(wParam);
                break;
            case WM_MOVE:
                pApp->onMove(LOWORD(lParam), HIWORD(lParam));
                break;
            case WM_SIZE:
                pApp->onResize(LOWORD(lParam), HIWORD(lParam));
                break;
            case WM_DESTROY:
                pApp->onQuit();
                PostQuitMessage(0);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        else {
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }

    return result;
}

void LayeredWindow::onInit()
{

}

void LayeredWindow::onRender()
{

}

void LayeredWindow::onHotkey(DWORD /*lParam*/)
{

}

void LayeredWindow::onQuit()
{

}

void LayeredWindow::onResize(int width, int height)
{
    if (width <= 0 || height <= 0) {
        return;
    }

    if (m_info) {
        m_info->resize(width, height);
    }

    if (m_bitmap) {
        delete m_bitmap;
        m_bitmap = new LayeredBitmap(width, height);
    }

    resizeGL(width, height);
}

void LayeredWindow::onMove(int x, int y)
{
    if (m_info) {
        m_info->moveTo(x, y);
    }
}

void LayeredWindow::onTimer(UINT /*timerId*/)
{

}

bool LayeredWindow::isInitialized() const
{
    return initialized;
}

