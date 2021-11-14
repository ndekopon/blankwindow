#define WINVER 0x0A00
#define _WIN32_WINNT 0x0A00

#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

const WCHAR window_title[] = L"blankwindow";
const WCHAR window_class[] = L"blankwindowclass";
const LONG window_width = 320;
const LONG window_height = 160;

LRESULT CALLBACK window_proc(HWND _window, UINT _message, WPARAM _wparam, LPARAM _lparam)
{
    switch (_message)
    {
    case WM_GETMINMAXINFO:
    {
        MINMAXINFO *mminfo = (MINMAXINFO *)_lparam;
        mminfo->ptMaxSize.x = window_width;
        mminfo->ptMaxSize.y = window_height;
        mminfo->ptMaxPosition.x = 0;
        mminfo->ptMaxPosition.y = 0;
        mminfo->ptMinTrackSize.x = window_width;
        mminfo->ptMinTrackSize.y = window_height;
        mminfo->ptMaxTrackSize.x = window_width;
        mminfo->ptMaxTrackSize.y = window_height;
        return 0;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(_window, _message, _wparam, _lparam);
    }
    return 0;
}

inline ATOM _register_class(HINSTANCE _instance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = window_proc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = _instance;
    wcex.hIcon = LoadIconW(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursorW(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = window_class;
    wcex.hIconSm = LoadIconW(NULL, IDI_APPLICATION);

    return RegisterClassExW(&wcex);
}

inline BOOL _init_instance(HINSTANCE _instance)
{
    HWND window = CreateWindowW(window_class, window_title, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, _instance, NULL);

    if (!window)
    {
        return FALSE;
    }

    ShowWindow(window, SW_NORMAL);
    UpdateWindow(window);

    return TRUE;
}

int WinMainCRTStartup(void)
{
    HINSTANCE instance = GetModuleHandleW(NULL);

    _register_class(instance);

    if (!_init_instance(instance))
    {
        return 1;
    }

    MSG msg;

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}
