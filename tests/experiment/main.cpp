#include <windows.h>

enum Status
{
    Ok = 0,
    GenericError = 1,
    InvalidParameter = 2,
    OutOfMemory = 3,
    ObjectBusy = 4,
    InsufficientBuffer = 5,
    NotImplemented = 6,
    Win32Error = 7,
    WrongState = 8,
    Aborted = 9,
    FileNotFound = 10,
    ValueOverflow = 11,
    AccessDenied = 12,
    UnknownImageFormat = 13,
    FontFamilyNotFound = 14,
    FontStyleNotFound = 15,
    NotTrueTypeFont = 16,
    UnsupportedGdiplusVersion = 17,
    GdiplusNotInitialized = 18,
    PropertyNotFound = 19,
    PropertyNotSupported = 20
};

struct GdiplusStartupInput
{
    UINT32 GdiplusVersion;
    void* DebugEventCallback;
    BOOL SuppressBackgroundThread;
    BOOL SuppressExternalCodecs;
};

extern "C"
{
    Status __stdcall GdiplusStartup(ULONG_PTR*, const GdiplusStartupInput*, void*);
    void __stdcall GdiplusShutdown(ULONG_PTR);
    Status __stdcall GdipCreateHBITMAPFromBitmap(void*, HBITMAP*, DWORD);
    Status __stdcall GdipLoadImageFromFile(const WCHAR*, void**);
    Status __stdcall GdipGetImageWidth(void*, UINT*);
    Status __stdcall GdipGetImageHeight(void*, UINT*);
    Status __stdcall GdipDisposeImage(void*);
}

int main()
{
    ULONG_PTR token;
    GdiplusStartupInput input = {1, 0, FALSE, FALSE};
    GdiplusStartup(&token, &input, 0);
    void* bitmap;
    GdipLoadImageFromFile(L"file.jpg", &bitmap);
    UINT w, h;
    GdipGetImageWidth(bitmap, &w);
    GdipGetImageHeight(bitmap, &h);
    HBITMAP hBmp;
    GdipCreateHBITMAPFromBitmap(bitmap, &hBmp, 0);

    HDC hDc = GetDC(0);
    HDC hDcBitmap = CreateCompatibleDC(hDc);
    HBITMAP hPrevBmp = (HBITMAP)SelectObject(hDcBitmap, hBmp);
    BitBlt(hDc, 0, 0, w, h, hDcBitmap, 0, 0, SRCCOPY);
    SelectObject(hDcBitmap, hPrevBmp);
    DeleteObject(hBmp);
    DeleteDC(hDcBitmap);
    ReleaseDC(0, hDc);

    GdipDisposeImage(bitmap);
    GdiplusShutdown(token);
}
