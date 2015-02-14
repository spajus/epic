#include <windows.h>

/*
struct tagWNDCLASS {
  UINT      style;
  WNDPROC   lpfnWndProc;
  int       cbClsExtra;
  int       cbWndExtra;
  HINSTANCE hInstance;
  HICON     hIcon;
  HCURSOR   hCursor;
  HBRUSH    hbrBackground;
  LPCTSTR   lpszMenuName;
  LPCTSTR   lpszClassName;
} WNDCLASS, *PWNDCLASS;
*/

LRESULT CALLBACK
MainWindowCallback(HWND Window,
                   UINT Message,
                   WPARAM WParam,
                   LPARAM LParam)
{
  LRESULT Result = 0;
  switch(Message)
  {
    case WM_SIZE:
    {
      OutputDebugString("WM_SIZE\n");
    } break;

    case WM_DESTROY:
    {
      OutputDebugString("WM_DESTROY\n");
    } break;

    case WM_CLOSE:
    {
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
      static DWORD Operation = WHITENESS;
      PatBlt(DeviceContext, X, Y, Width, Height, Operation);
      if (Operation == WHITENESS)
      {
        Operation = BLACKNESS;
      }
      else
      {
        Operation = WHITENESS;
      }
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
  WindowClass.lpfnWndProc = MainWindowCallback;

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
      MSG Message;
      for (;;)
      {
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
