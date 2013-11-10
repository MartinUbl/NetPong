#include "game_inc.h"
#include "drawing.h"
#include "interface.h"
#include "controller.h"
#include "network.h"
#include <SimplyFlat.h>
#include <locale>

LRESULT CALLBACK MyWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    return sSimplyFlat->SFWndProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(    HINSTANCE    hInstance,
                    HINSTANCE    hPrevInstance,
                    LPSTR        lpCmdLine,
                    int            nCmdShow)
{
    MSG msg;
    bool done = false;

    srand ( (unsigned int)time(NULL) );

    sSimplyFlat->CreateMainWindow("NetPong", 640, 480, 32, false, 60, &MyWndProc);

    Controller.RetainCmdLine(lpCmdLine);
    Network.Init();
    Controller.Init();

    ShowCursor(true);

    while (1)
    {
        if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
        {
            if (msg.message != WM_QUIT)
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            else
                break;
        }
        else
        {
            Network.Listen();
            Controller.Update();

            sSimplyFlat->BeforeDraw();

            MyDraw.Draw();

            sSimplyFlat->AfterDraw();
        }
    }

    sSimplyFlat->DestroyMainWindow();
    return (msg.wParam);
}
