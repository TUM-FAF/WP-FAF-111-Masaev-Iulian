#include<windows.h>
#include<math.h>


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
    static TCHAR szAppName[] = TEXT("bezier curves");
    HWND     hwnd;
    MSG      msg;
    WNDCLASS wndclass;


    wndclass.style = CS_HREDRAW | CS_VREDRAW ;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szAppName;

    RegisterClass (&wndclass);

    hwnd = CreateWindow(szAppName, TEXT ("Bezier Curves"), WS_OVERLAPPEDWINDOW,
                        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);

    while(GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam ;
}
int dist(POINT pt1,POINT pt2)
{
    float d=sqrt((pt2.y-pt1.y)*(pt2.y-pt1.y)+(pt2.x-pt1.x)*(pt2.x-pt1.x));
    return (int)d;
}
POINT find_near(POINT* apt1,POINT* apt2,POINT point)
{
    POINT pt;
    int i,iDist;
    pt=apt1[0];
    iDist=dist(pt,point);
    for(i=1;i<4;i++)
    {
        if(dist(pt,apt1[i])<iDist) {pt=apt1[i]; iDist=dist(pt,point); }
    }
    for(i=0;i<15;i++)
    {
        if(dist(pt,apt2[i])<iDist) {pt=apt2[i]; iDist=dist(pt,point); }
    }
    return pt;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC         hdc ;
    PAINTSTRUCT ps ;
    static POINT bzr[200],pt[4],bzg[15]; //for own bezier, built in bezier, and beizergon with 5 points
    int          cxClient, cyClient ;
    RECT        rect ;
    switch (message)
    {
    case WM_SIZE:
        cxClient=LOWORD(lParam);
        cyClient=HIWORD(lParam);

    case WM_PAINT:

        return 0 ;

    case WM_DESTROY:
        PostQuitMessage (0) ;
        return 0 ;


    }
    return DefWindowProc (hwnd, message, wParam, lParam) ;
}
