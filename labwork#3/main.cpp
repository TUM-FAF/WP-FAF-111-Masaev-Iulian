#include<windows.h>
#include<math.h>
#include<stdio.h>
#define size 200


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
                        0, 0, 1200, 720, NULL, NULL, hInstance, NULL);

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
    for(i=1; i<4; i++)
    {
        if(dist(pt,apt1[i])<iDist)
        {
            pt=apt1[i];
            iDist=dist(pt,point);
        }
    }
    for(i=0; i<15; i++)
    {
        if(dist(pt,apt2[i])<iDist)
        {
            pt=apt2[i];
            iDist=dist(pt,point);
        }
    }
    return pt;
}
POINT* Beziergongen(int w,int h)
{
    POINT* bzg;
    int i,off1,off2;
    off1=w/15;
    off2=h/20;
    bzg=(POINT*)malloc(15*sizeof(POINT));
    bzg[0].x=w/2;
    bzg[0].y=3*h/7;
    bzg[1].x=bzg[0].x-off1;
    bzg[1].y=bzg[0].y;
    bzg[3].x=w/4;
    bzg[3].y=h/2;
    bzg[2].x=bzg[3].x+off2;
    bzg[2].y=bzg[3].y-off1;
    bzg[4].x=bzg[3].x-off2;
    bzg[4].y=bzg[3].y+off1;
    bzg[6].x=5*w/12;
    bzg[6].y=3*h/4;
    bzg[5].x=bzg[6].x-off1;
    bzg[5].y=bzg[6].y-off2;
    bzg[7].x=bzg[6].x+off1;
    bzg[7].y=bzg[6].y+off2;
    for(i=8; i<15; i++)
    {
        bzg[i].x=w-bzg[15-i].x;
        bzg[i].y=bzg[15-i].y;
    }
    return bzg;
}
void Beziergon(HDC hdc,POINT* bzg)
{
    HPEN hpen;
    POINT *apt;
    int i,j;
    apt=(POINT*)malloc(4*sizeof(POINT));
    for(i=0; i<5; i++)
    {
        for(j=0; j<4; j++)
        {
            if((i+j)==15)
            {
                apt[j]=bzg[0];
            }
            else
            {
                apt[j]=bzg[3*i+j];
            }
        }
    }

}
POINT* OwnBezier(POINT* pt,int width)
{
    POINT *bzr;
    int i;
    float t=0,mt,xcoord,ycoord;
    POINT *pm;
    pm=(POINT*)malloc(4*sizeof(POINT));
    bzr=(POINT*)malloc(size*sizeof(POINT));
    for(i=0; i<4; i++)
    {
        pm[i].x=pt[i].x+width/2;
        pm[i].y=pt[i].y;
    }
    bzr[0]=pm[0];
    bzr[199]=pm[3];
    for(i=1; i<200; i++)
    {
        t=t+0.005;
        mt=1-t;
        xcoord=mt*mt*mt*pm[0].x + 3*t*mt*mt*pm[1].x + 3*t*t*mt*pm[2].x + t*t*t*pm[3].x;
        ycoord=mt*mt*mt*pm[0].y + 3*t*mt*mt*pm[1].y + 3*t*t*mt*pm[2].y + t*t*t*pm[3].y;
        bzr[i].x=(int)xcoord;
        bzr[i].y=(int)ycoord;
    }

    return bzr;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC             hdc;
    PAINTSTRUCT     ps;
    static POINT    *bzr,pt[4],*bzg,point,*apt; //for own bezier, built in bezier, and beizergon with 5 points
    static int      cxClient, cyClient;
    RECT            rect;
    HPEN            hwide,hvar,hbez;
    int             i,j;
    switch (message)
    {
    case WM_CREATE:

        return 0;
    case WM_SIZE:
        cxClient=LOWORD(lParam);
        cyClient=HIWORD(lParam);
        pt[0].x = cxClient/12;
        pt[0].y = cyClient/8;
        pt[1].x = cxClient/12;
        pt[1].y = cyClient/4;
        pt[2].x = cxClient/3;
        pt[2].y = cyClient/4;
        pt[3].x = cxClient/3;
        pt[3].y = cyClient/8;
        bzr=OwnBezier(pt,cxClient);
        bzg=Beziergongen(cxClient,cyClient);
        return 0;
    case WM_PAINT:
        InvalidateRect (hwnd, NULL, TRUE);
        hdc=BeginPaint(hwnd,&ps);
        hwide=CreatePen(PS_SOLID,3,RGB(0,0,0));
        SelectObject(hdc,hwide);
        MoveToEx(hdc,0,cyClient/3,&point);
        LineTo(hdc,cxClient,cyClient/3);  //draw a horizontal line
        MoveToEx(hdc,cxClient/2,0,&point);
        LineTo(hdc,cxClient/2,cyClient/3); //draw a vertical line
        hbez=CreatePen(PS_SOLID,2,RGB(0,200,32));
        SelectObject(hdc,hbez);

        PolyBezier(hdc,pt,4); //draw bezier curve using implemented function in first rectangle
        hbez=CreatePen(PS_SOLID,2,RGB(87,91,18));
        SelectObject(hdc,hbez);
        Polyline(hdc,bzr,size); //draw bezier curve using own function(for computing set of points)
        //Beziergon(hdc,bzg);
        apt=(POINT*)malloc(4*sizeof(POINT));
        for(i=0; i<5; i++)
        {
            for(j=0; j<4; j++)
            {
                if((3*i+j)==15)
                {
                    apt[j]=bzg[0];
                }
                else
                {
                    apt[j]=bzg[3*i+j];
                }
            }
            hvar=CreatePen(PS_SOLID,4,RGB(100*i,255-34*i,11*i));
            SelectObject(hdc,hvar);
            PolyBezier(hdc,apt,4);
            DeleteObject(hvar);
        }
        DeleteObject(hwide);
        DeleteObject(hbez);
        EndPaint(hwnd,&ps);
        return 0 ;

    case WM_LBUTTONDOWN:
        point.x=LOWORD(lParam);
        point.y=HIWORD(lParam);
        find_near(pt,bzg,point);
    case WM_DESTROY:

        PostQuitMessage (0) ;
        return 0 ;


    }
    return DefWindowProc (hwnd, message, wParam, lParam) ;
}
