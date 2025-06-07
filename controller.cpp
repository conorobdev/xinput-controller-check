// Add these defines before any includes
#define UNICODE
#define _UNICODE

#include <windows.h>
#include <Xinput.h>
#include <string>
#include <wingdi.h>

// ... existing code ...

// Window dimensions and control positions
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int STICK_SIZE = 100;
const int TRIGGER_HEIGHT = 20;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void DrawControllerState(HWND hwnd, HDC hdc, DWORD dwUserIndex);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Register window class
    const wchar_t CLASS_NAME[] = L"Controller Viewer";
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    RegisterClass(&wc);

    // Create window
    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Controller State",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwnd == NULL) return 0;
    ShowWindow(hwnd, nCmdShow);

    // Message loop
    MSG msg = {};
    while (true) {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) return 0;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        InvalidateRect(hwnd, NULL, TRUE);
        Sleep(16); // ~60 FPS
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        
        // Draw state for each controller
        for (DWORD i = 0; i < XUSER_MAX_COUNT; i++) {
            DrawControllerState(hwnd, hdc, i);
        }
        
        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void DrawControllerState(HWND hwnd, HDC hdc, DWORD dwUserIndex) {
    XINPUT_STATE state;
    ZeroMemory(&state, sizeof(XINPUT_STATE));
    DWORD result = XInputGetState(dwUserIndex, &state);

    int yOffset = dwUserIndex * 150; // Space between controller displays

    if (result == ERROR_SUCCESS) {
        // Draw buttons
        WORD buttons = state.Gamepad.wButtons;
        const WCHAR* buttonLabels[] = { L"A", L"B", L"X", L"Y" };
        WORD buttonMasks[] = { XINPUT_GAMEPAD_A, XINPUT_GAMEPAD_B, XINPUT_GAMEPAD_X, XINPUT_GAMEPAD_Y };
        
        for (int i = 0; i < 4; i++) {
            RECT buttonRect = { 20 + (i * 60), 20 + yOffset, 60 + (i * 60), 50 + yOffset };
            bool pressed = (buttons & buttonMasks[i]) != 0;
            FillRect(hdc, &buttonRect, (HBRUSH)GetStockObject(pressed ? BLACK_BRUSH : WHITE_BRUSH));
            DrawText(hdc, buttonLabels[i], -1, &buttonRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        }

        // Draw triggers
        int leftTrigger = (int)state.Gamepad.bLeftTrigger;
        int rightTrigger = (int)state.Gamepad.bRightTrigger;
        
        RECT leftTriggerRect = { 300, 20 + yOffset, 300 + (leftTrigger / 2), 40 + yOffset };
        RECT rightTriggerRect = { 300, 50 + yOffset, 300 + (rightTrigger / 2), 70 + yOffset };
        
        // Add label rectangles
        RECT leftTriggerLabel = { 240, 20 + yOffset, 295, 40 + yOffset };
        RECT rightTriggerLabel = { 240, 50 + yOffset, 295, 70 + yOffset };
        
        DrawText(hdc, L"LT:", -1, &leftTriggerLabel, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
        DrawText(hdc, L"RT:", -1, &rightTriggerLabel, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
        
        FillRect(hdc, &leftTriggerRect, (HBRUSH)GetStockObject(GRAY_BRUSH));
        FillRect(hdc, &rightTriggerRect, (HBRUSH)GetStockObject(GRAY_BRUSH));

        // Draw thumbsticks
        int centerX = 500;
        int centerY = 50 + yOffset;
        
        // Left stick
        Ellipse(hdc, 
            centerX + (state.Gamepad.sThumbLX / 2000) - 5,
            centerY - (state.Gamepad.sThumbLY / 2000) - 5,
            centerX + (state.Gamepad.sThumbLX / 2000) + 5,
            centerY - (state.Gamepad.sThumbLY / 2000) + 5);

        // Right stick
        Ellipse(hdc,
            centerX + 100 + (state.Gamepad.sThumbRX / 2000) - 5,
            centerY - (state.Gamepad.sThumbRY / 2000) - 5,
            centerX + 100 + (state.Gamepad.sThumbRX / 2000) + 5,
            centerY - (state.Gamepad.sThumbRY / 2000) + 5);
    } else {
        std::wstring text = L"Controller " + std::to_wstring(dwUserIndex) + L" not connected";
        TextOut(hdc, 20, 20 + yOffset, text.c_str(), text.length());
    }
}