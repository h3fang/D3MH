#include "layeredwindow.h"

#include <stdio.h>
#include <wchar.h>
#include <string.h>

void *GetAnyGLFuncAddress(const char *name)
{
    void *p = (void *)wglGetProcAddress(name);
    if(p == 0 ||
            (p == (void*)0x1) || (p == (void*)0x2) || (p == (void*)0x3) ||
            (p == (void*)-1) )
    {
        HMODULE module = LoadLibraryA("opengl32.dll");
        p = (void *)GetProcAddress(module, name);
    }

    return p;
}

LayeredWindow::LayeredWindow() :
    hWnd(NULL),
    hDC(NULL),
    g_hPBufferDC(0),
    g_hPBufferRC(0),
    g_hPBuffer(0),
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

    releasePBuffer();
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
    wcex.style         = 0;
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
        glClearColor(0, 0, 0, 0);
    }

    return hr;
}

void LayeredWindow::resizeGL(UINT width, UINT height)
{
    glViewport(0, 0, width, height);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, width, 0.0, height);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

bool LayeredWindow::createOpenGLContext()
{
    // create a temporary context to load extensions

    PIXELFORMATDESCRIPTOR pfd;
    memset(&pfd,0, sizeof(PIXELFORMATDESCRIPTOR));
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 0;
    pfd.iLayerType = PFD_MAIN_PLANE;

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

    if (!initOpenGLExtensions()) {
        return false;
    }

    if (!initPBuffer()) {
        return false;
    }

    wglMakeCurrent(hDC, 0);
    wglMakeCurrent(g_hPBufferDC, g_hPBufferRC);

    fprintf(stderr, "OpenGL version: %s\n", glGetString(GL_VERSION));

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
    UCHAR *pPixel = m_bitmap->bits;
    int totalPixels = m_bitmap->width * m_bitmap->height;

    for (int i = 0; i < totalPixels; ++i, pPixel += 4)
    {
        pPixel[0] = (UCHAR)(pPixel[0] * (float)pPixel[3] / 255.0f);
        pPixel[1] = (UCHAR)(pPixel[1] * (float)pPixel[3] / 255.0f);
        pPixel[2] = (UCHAR)(pPixel[2] * (float)pPixel[3] / 255.0f);
    }

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
            WINDOWPOS *pos = NULL;
            switch (message)
            {
            case WM_ERASEBKGND:
            case WM_PAINT:
                break;
            case WM_TIMER:
                pApp->onTimer(wParam);
                break;
            case WM_WINDOWPOSCHANGING:
                pos = (WINDOWPOS*)lParam;
                pApp->onResize(pos->x, pos->y, pos->cx, pos->cy);
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

void LayeredWindow::onResize(UINT x, UINT y, UINT width, UINT height)
{
    m_info->updateSize(x, y, width, height);
    if (m_bitmap) {
        delete m_bitmap;
        m_bitmap = new LayeredBitmap(width, height);
        releasePBuffer();
        updatePBuffer(width, height);
    }

    resizeGL(width, height);
}

void LayeredWindow::onTimer(UINT /*timerId*/)
{

}

bool LayeredWindow::initOpenGLExtensions()
{
    // WGL_ARB_pbuffer.
    wglDestroyPbufferARB   = (PFNWGLDESTROYPBUFFERARBPROC)wglGetProcAddress("wglDestroyPbufferARB");
    wglQueryPbufferARB     = (PFNWGLQUERYPBUFFERARBPROC)wglGetProcAddress("wglQueryPbufferARB");
    wglGetPbufferDCARB     = (PFNWGLGETPBUFFERDCARBPROC)wglGetProcAddress("wglGetPbufferDCARB");
    wglCreatePbufferARB    = (PFNWGLCREATEPBUFFERARBPROC)wglGetProcAddress("wglCreatePbufferARB");
    wglReleasePbufferDCARB = (PFNWGLRELEASEPBUFFERDCARBPROC)wglGetProcAddress("wglReleasePbufferDCARB");

    // WGL_ARB_pixel_format.
    wglChoosePixelFormatARB      = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
    wglGetPixelFormatAttribfvARB = (PFNWGLGETPIXELFORMATATTRIBFVARBPROC)wglGetProcAddress("wglGetPixelFormatAttribfvARB");
    wglGetPixelFormatAttribivARB = (PFNWGLGETPIXELFORMATATTRIBIVARBPROC)wglGetProcAddress("wglGetPixelFormatAttribivARB");

    if (!wglDestroyPbufferARB || !wglQueryPbufferARB || !wglGetPbufferDCARB || !wglCreatePbufferARB || !wglReleasePbufferDCARB)
    {
        fprintf(stderr, "Required extension WGL_ARB_pbuffer not supported\n");
        return false;
    }

    if (!wglChoosePixelFormatARB || !wglGetPixelFormatAttribfvARB || !wglGetPixelFormatAttribivARB)
    {
        fprintf(stderr, "Required extension WGL_ARB_pixel_format not supported\n");
        return false;
    }

    return true;
}

bool LayeredWindow::initPBuffer()
{
    int attribList[] =
    {
        WGL_DRAW_TO_PBUFFER_ARB, TRUE,      // allow rendering to the pbuffer
        WGL_SUPPORT_OPENGL_ARB,  TRUE,      // associate with OpenGL
        WGL_DOUBLE_BUFFER_ARB,   FALSE,     // single buffered
        WGL_RED_BITS_ARB,   8,              // minimum 8-bits for red channel
        WGL_GREEN_BITS_ARB, 8,              // minimum 8-bits for green channel
        WGL_BLUE_BITS_ARB, 8,              // minimum 8-bits for blue channel
        WGL_ALPHA_BITS_ARB, 8,              // minimum 8-bits for alpha channel
        WGL_DEPTH_BITS_ARB, 16,             // minimum 16-bits for depth buffer
        0
    };

    pixelformat = 0;
    UINT matchingFormats = 0;

    if (!wglChoosePixelFormatARB(hDC, attribList, 0, 1, &pixelformat, &matchingFormats))
    {
        fprintf(stderr, "wglChoosePixelFormatARB() failed\n");
        return false;
    }

    return updatePBuffer(windowPos.right-windowPos.left, windowPos.bottom-windowPos.top);
}

bool LayeredWindow::updatePBuffer(UINT width, UINT height)
{
    if (!(g_hPBuffer = wglCreatePbufferARB(hDC, pixelformat, width, height, 0)))
    {
        fprintf(stderr, "wglCreatePbufferARB() failed\n");
        return false;
    }

    if (!(g_hPBufferDC = wglGetPbufferDCARB(g_hPBuffer)))
    {
        fprintf(stderr, "wglGetPbufferDCARB() failed\n");
        return false;
    }

    if (!(g_hPBufferRC = wglCreateContext(g_hPBufferDC)))
    {
        fprintf(stderr, "wglCreateContext() failed for PBuffer\n");
        return false;
    }

    return true;
}

void LayeredWindow::releasePBuffer()
{
    if (g_hPBuffer)
    {
        wglDeleteContext(g_hPBufferRC);
        wglReleasePbufferDCARB(g_hPBuffer, g_hPBufferDC);
        wglDestroyPbufferARB(g_hPBuffer);
        g_hPBufferRC = 0;
        g_hPBufferDC = 0;
        g_hPBuffer = 0;
    }
}

bool LayeredWindow::isInitialized() const
{
    return initialized;
}

