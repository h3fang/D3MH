#ifndef LAYERED_LAYEREDWINDOWINFO_H
#define LAYERED_LAYEREDWINDOWINFO_H

#include <windows.h>

#include <stdio.h>

class LayeredWindowInfo
{
public:
    LayeredWindowInfo( UINT width, UINT height)
    {
        ZeroMemory(&m_sourcePosition, sizeof(m_sourcePosition));
        ZeroMemory(&m_windowPosition, sizeof(m_windowPosition));
        ZeroMemory(&m_info, sizeof(m_info));

        m_size.cx = width;
        m_size.cy = height;

        m_blend.SourceConstantAlpha = 255;
        m_blend.AlphaFormat = AC_SRC_ALPHA;
        m_blend.BlendFlags = 0;
        m_blend.BlendOp = AC_SRC_OVER;

        m_info.cbSize = sizeof(UPDATELAYEREDWINDOWINFO);
        m_info.pptSrc = &m_sourcePosition;
        m_info.pptDst = &m_windowPosition;
        m_info.psize = &m_size;
        m_info.pblend = &m_blend;
        m_info.dwFlags = ULW_ALPHA;
    }

    void update( HWND window, HDC source) {

        m_info.hdcSrc = source;

        if (!UpdateLayeredWindowIndirect(window, &m_info)) {
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

            fwprintf(stderr, L"Failed to call UpdateLayeredWindowIndirect(): %ls\n",  lpMsgBuf);

            LocalFree( lpMsgBuf );
        }
    }

    void resize(int width, int height) {
        m_size.cx = width;
        m_size.cy = height;
    }

    void moveTo(int x, int y) {
        m_windowPosition.x = x;
        m_windowPosition.y = y;
    }

    UINT getWidth() const { return m_size.cx; }

    UINT getHeight() const { return m_size.cy; }

private:
    POINT m_sourcePosition;
    POINT m_windowPosition;
    SIZE m_size;
    BLENDFUNCTION m_blend;
    UPDATELAYEREDWINDOWINFO m_info;
};

#endif // LAYERED_LAYEREDWINDOWINFO_H

