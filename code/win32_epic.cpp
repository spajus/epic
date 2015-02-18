#include <windows.h>

#define internal static
#define local_persist static
#define global_variable static

global_variable BITMAPINFO BitmapInfo;
global_variable void *BitmapMemory;
global_variable HBITMAP BitmapHandle;
global_variable HDC BitmapDeviceContext;

internal void
Win32ResizeDIBSection(int Width, int Height)
{

  if (BitmapHandle)
  {
    DeleteObject(BitmapHandle);
  }

  if (!BitmapDeviceContext)
  {
    BitmapDeviceContext = CreateCompatibleDC(0);
  }
  BitmapInfo.bmiHeader.biSize = sizeof(BitmapInfo.bmiHeader);
  BitmapInfo.bmiHeader.biWidth = Width;
  BitmapInfo.bmiHeader.biHeight = Height;
  BitmapInfo.bmiHeader.biPlanes = 1;
  // 24 would be enough, but we want it to be DWORD alligned
  BitmapInfo.bmiHeader.biBitCount = 32;
  BitmapInfo.bmiHeader.biCompression = BI_RGB;

  BitmapHandle = CreateDIBSection(
    BitmapDeviceContext,
    &BitmapInfo,
    DIB_RGB_COLORS,
    &BitmapMemory,
    0, 0);
}

internal void
Win32UpdateWindow(HDC DeviceContext, int X, int Y, int Width, int Height)
{

StretchDIBits(DeviceContext,
              X, Y, Width, Height,
              X, Y, Width, Height,
  BitmapMemory,
  &BitmapInfo,
  DIB_RGB_COLORS, SRCCOPY);
}

global_variable bool Running;

LRESULT CALLBACK
Win32MainWindowCallback(HWND Window,
                   UINT Message,
                   WPARAM WParam,
                   LPARAM LParam)
{
  LRESULT Result = 0;
  switch(Message)
  {
    case WM_SIZE:
    {
      RECT ClientRect;
      GetClientRect(Window, &ClientRect);
      int Width = ClientRect.right - ClientRect.left;
      int Height = ClientRect.bottom - ClientRect.top;
      Win32ResizeDIBSection(Width, Height);
      OutputDebugString("WM_SIZE\n");
    } break;

    case WM_DESTROY:
    {
      Running = false;
      OutputDebugString("WM_DESTROY\n");
    } break;

    case WM_CLOSE:
    {
      Running = false;
      OutputDebugString("WM_CLOSE\n");
    } break;
    case WM_PAINT:
    {
      PAINTSTRUCT Paint;
      HDC DeviceContext = BeginPaint(Window, &Paint);
      int X = Paint.rcPaint.left;
      int Y = Paint.rcPaint.right;
      int Height = Paint.rcPaint.bottom - Paint.rcPaint.top;
      int Width = Paint.rcPaint.right - Paint.rcPaint.left;
      Win32UpdateWindow(DeviceContext, X, Y, Width, Height);
      EndPaint(Window, &Paint);

    } break;

    case WM_ACTIVATEAPP:
    {
      OutputDebugString("WM_ACTIVATEAPP\n");
    } break;

    default:
    {
      //OutputDebugString("default\n");
      Result = DefWindowProc(Window, Message, WParam, LParam);
    } break;
  }
  return(Result);
}


int CALLBACK
WinMain(HINSTANCE Instance,
        HINSTANCE PrevInstance,
        LPSTR CommandLine,
        int ShowCode)
{
  //MessageBox(0, "Hi", "Caption", MB_OK|MB_ICONINFORMATION);
  WNDCLASS WindowClass = {};
  //WindowClass.hIcon;
  WindowClass.style = CS_OWNDC|CS_HREDRAW|CS_VREDRAW;
  WindowClass.hInstance = Instance;
  WindowClass.lpszClassName = "EpicWindowClass";
  WindowClass.lpfnWndProc = Win32MainWindowCallback;

  if (RegisterClass(&WindowClass))
  {
    HWND WindowHandle =
      CreateWindowEx(
        0,
        WindowClass.lpszClassName,
        "Epic",
        WS_OVERLAPPEDWINDOW|WS_VISIBLE,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        0,
        0,
        Instance,
        0);
    if (WindowHandle)
    {
      Running = true;
      while(Running)
      {
        MSG Message;
        BOOL MessageResult = GetMessage(&Message, 0, 0, 0);
        if (MessageResult > 0)
        {
           TranslateMessage(&Message);
           DispatchMessage(&Message);

        }
        else
        {
          break;
        }
      }
    }
  }
  else
  {
  }
  return 0;
}
