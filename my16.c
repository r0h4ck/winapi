#include <windows.h>
#define MAXIMIZE_MINDOW 1
#define MINIMIZE_WINDOW 2
#define CLOSE_WINDOW 3

void AddControls(HWND);
void registerDialogClass(HINSTANCE);
void displayDialog(HWND);
void registerDialogClass(HINSTANCE);
void displayDialog(HWND);
HWND hstats;


LRESULT CALLBACK WindowProcedure(HWND hWnd,UINT msg,WPARAM wp,LPARAM lp);

int WINAPI WinMain(HINSTANCE hInst,HINSTANCE hPrevInst,LPSTR args,int ncmdshow) {

    WNDCLASSW wc = {};
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL,IDC_ARROW);
    wc.hInstance = hInst;
    wc.lpszClassName = L"myWindowClass";
    wc.lpfnWndProc = WindowProcedure;


    if(!RegisterClassW(&wc)) return -1;
    registerDialogClass(hInst);

    CreateWindowW(L"myWindowClass",L"My Window",WS_OVERLAPPEDWINDOW | WS_VISIBLE,100,100,500,500,NULL,NULL,NULL,NULL);


    MSG msg = {0};

    while(GetMessage(&msg,NULL,NULL,NULL)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

LRESULT CALLBACK WindowProcedure(HWND hWnd,UINT msg,WPARAM wp,LPARAM lp) {
    wchar_t out[50];
    switch(msg) {

    case WM_COMMAND:
        switch(wp) {
            case CLOSE_WINDOW:
                DestroyWindow(hWnd);
                break;
            case MAXIMIZE_MINDOW:
                ShowWindow(hWnd,SW_MAXIMIZE);
                SetWindowText(hstats,"Maximized");
                break;

            case MINIMIZE_WINDOW:
                ShowWindow(hWnd,SW_MINIMIZE);
                SetWindowText(hstats,"Minimized");
                break;


        }
        break;
    case WM_CREATE:
        displayDialog(hWnd);
        AddControls(hWnd);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd,msg,wp,lp);
    }
}


void AddControls(HWND hWnd) {
    CreateWindowW(L"Button",L"Minimize",WS_VISIBLE | WS_CHILD,0,140,98,38,hWnd,(HMENU)MINIMIZE_WINDOW,NULL,NULL);
    CreateWindowW(L"Button",L"Maximize",WS_VISIBLE | WS_CHILD,150,140,98,38,hWnd,(HMENU)MAXIMIZE_MINDOW,NULL,NULL);
    CreateWindowW(L"Button",L"Close",WS_VISIBLE | WS_CHILD,300,140,98,38,hWnd,(HMENU)CLOSE_WINDOW,NULL,NULL);
}



LRESULT CALLBACK DialogProcedure(HWND hWnd,UINT msg,WPARAM wp,LPARAM lp) {
    switch(msg) {


    case WM_CLOSE:
        DestroyWindow(hWnd);
        break;
    default:
        return DefWindowProcW(hWnd,msg,wp,lp);


    }
}





void registerDialogClass(HINSTANCE hInst) {
    WNDCLASSW dialog = {};
    dialog.hbrBackground = (HBRUSH)COLOR_WINDOW;
    dialog.hCursor = LoadCursor(NULL,IDC_CROSS);
    dialog.hInstance = hInst;
    dialog.lpszClassName = L"myDialogClass";
    dialog.lpfnWndProc = DialogProcedure;


    RegisterClassW(&dialog);

}

void displayDialog(HWND hWnd) {
   HWND child =  CreateWindowW(L"myDialogClass",L"Dialog",WS_VISIBLE | WS_OVERLAPPEDWINDOW,400,400,400,400,hWnd,NULL,NULL,NULL);
   hstats = CreateWindowW(L"Static",NULL,WS_VISIBLE | WS_CHILD,100,50,98,38,child,NULL,NULL,NULL);



}




