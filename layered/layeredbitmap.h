#ifndef LAYERED_BITMAP_H
#define LAYERED_BITMAP_H

#include <windows.h>

class LayeredBitmap
{
public:
    UINT width;
    UINT height;
    UCHAR* bits;

public:
    LayeredBitmap(int width, int height):
        width(width),
        height(height),
        bits(0),
        bmpOld(0),
        bmpMem(0)
    {
        hdcMem = CreateCompatibleDC(NULL);

        BITMAPINFO bitmapInfo;
        ZeroMemory(&bitmapInfo, sizeof(bitmapInfo));
        bitmapInfo.bmiHeader.biSize = sizeof(bitmapInfo.bmiHeader);
        bitmapInfo.bmiHeader.biWidth = width;
        bitmapInfo.bmiHeader.biHeight = 0 - height;
        bitmapInfo.bmiHeader.biPlanes = 1;
        bitmapInfo.bmiHeader.biBitCount = 32;
        bitmapInfo.bmiHeader.biCompression = BI_RGB;

        bmpMem = CreateDIBSection(
                    0, // device context
                    &bitmapInfo,
                    DIB_RGB_COLORS,
                    (void**)&bits,
                    0, // file mapping object
                    0);

        SelectObject(hdcMem, bmpMem);
    }

    ~LayeredBitmap() {
        SelectObject(hdcMem, bmpOld);
        DeleteObject(bmpMem);
        DeleteDC(hdcMem);
    }

    HDC getDC() const { return hdcMem; }

private:
    HBITMAP bmpOld;
    HBITMAP bmpMem;
    HDC hdcMem;
};

#endif // LAYERED_BITMAP_H
