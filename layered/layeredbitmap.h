#ifndef LAYERED_BITMAP_H
#define LAYERED_BITMAP_H

#include <windows.h>

#include <stdio.h>

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

        if (!hdcMem) {
            fprintf(stderr, "Failed to call CreateCompatibleDC(NULL)\n");
        }

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

        if (!bmpMem) {
            fprintf(stderr, "Failed to call CreateDIBSection()\n");
        }

        bmpOld = SelectObject(hdcMem, bmpMem);
    }

    ~LayeredBitmap() {
        SelectObject(hdcMem, bmpOld);
        DeleteObject(bmpMem);
        DeleteDC(hdcMem);
    }

    HDC getDC() const { return hdcMem; }

private:
    HGDIOBJ bmpOld;
    HBITMAP bmpMem;
    HDC hdcMem;
};

#endif // LAYERED_BITMAP_H
