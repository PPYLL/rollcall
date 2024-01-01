#include <Windows.h>
#include<time.h>
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void CALLBACK CALLBACK_ID_TIMER_1(HWND hwnd, UINT message, UINT_PTR iTimerID, DWORD dwTime);
typedef int(*DLLCALLBACK)(int *x,int *y,char *s,int *ms);
DLLCALL dllcallback=NULL;
HINSTANCE hInst;
HWND hwnd;         //窗口句柄
int x,y;
int ms=30,flag=0;
char text[20];

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	PSTR szCmdLine,
	int iCmdShow
) {
	static TCHAR szClassName[] = TEXT("DMWin");   //窗口类名
	MSG msg;           //消息
	WNDCLASS wndclass; // 窗口类
	hInst = hInstance;

    dllcallback=(DLLCALLBACK)GetProcAddress(LoadLibrary(TEXT("call.dll")),MAKEINTRESOURCE(1));
    
	 /**********第①步：注册窗口类**********/
	//为窗口类的各个字段赋值
	wndclass.style = CS_HREDRAW | CS_VREDRAW;	//窗口风格
	wndclass.lpfnWndProc = WndProc;		//窗口过程
	wndclass.cbClsExtra = 0;			//暂时不需要理解
	wndclass.cbWndExtra = 0;			//暂时不需要理解
	wndclass.hInstance = hInstance;			//当前窗口句柄
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);	//窗口图标
	wndclass.hCursor = LoadIcon(NULL, IDC_ARROW);		//鼠标样式
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	//窗口背景画刷
	wndclass.lpszMenuName = NULL;		//窗口菜单
	wndclass.lpszClassName = szClassName; // 窗口类名

	//注册窗口
	RegisterClass(&wndclass);


	/*****第②步：创建窗口(并让窗口显示出来)*****/
	hwnd = CreateWindow(
		szClassName,		//窗口类的名字
		TEXT("点名器V1.0"),		//窗口标题（出现在标题栏）
		WS_OVERLAPPEDWINDOW,	//窗口风格
		800,			//初始化时x轴的位置
		1000,			//初始化时y轴的位置
		500,			//窗口宽度
		300,			//窗口高度
		NULL,			//父窗口句柄
		NULL,			//窗口菜单句柄
		hInstance,		//当前窗口的句柄
		NULL			//不使用该值
	);

	//显示窗口
	ShowWindow(hwnd, iCmdShow);
	//更新（绘制）窗口
	UpdateWindow(hwnd);


	/**********第③步：消息循环**********/
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

/**********第④步：窗口过程**********/
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;		  //设备环境句柄
	PAINTSTRUCT ps;   //存储绘图环境的相关信息
	int wmId, wmEvent;
	static HFONT hFont;
	static HWND hBtn;
	RECT rect={};
	switch (message)
	{
	case WM_CREATE:		
		srand((unsigned)time(NULL));
		
		GetClientRect(hWnd,&rect);
		//创建逻辑字体
		hFont = CreateFont(
			-15/*高度*/, -7.5/*宽度*/, 0/*不用管*/, 0/*不用管*/, 700 /*一般这个值设为400*/,
			FALSE/*不带斜体*/, FALSE/*不带下划线*/, FALSE/*不带删除线*/,
			DEFAULT_CHARSET,  //这里使用默认字符集，还有其他以 _CHARSET 结尾的常量可用
			OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,  //这行参数不用管
			DEFAULT_QUALITY,  //默认输出质量
			FF_DONTCARE,  //不指定字体族*/
			TEXT("微软雅黑" ) //字体名
		);

		//创建按钮控件
		hBtn = CreateWindow(
			TEXT("button"), //按钮控件的类名
			TEXT("开始"),
			WS_CHILD | WS_VISIBLE | WS_BORDER | BS_FLAT/*扁平样式*/,
			rect.right/2-70 /*X坐标*/, rect.bottom/2*3 /*Y坐标*/, 140 /*宽度*/, 50/*高度*/,
			hWnd, (HMENU)1 /*控件唯一标识符*/, hInst, NULL
		);

		SendMessage(hBtn, WM_SETFONT, (LPARAM)hFont, NULL); //设置按钮字体
		break;

	case WM_PAINT:{
		GetClientRect(hWnd,&rect);
		hdc = BeginPaint(hWnd, &ps);    //开始绘图并返回环境句柄
		
		LOGFONT logfont； //改变输出字体
        ZeroMemory(&logfont, sizeof(LOGFONT));
        logfont.lfCharSet=DEFAULT_CHARSET;
		logfont.lfWeight=1000;
		logfont.lfHeight = -MulDiv(26, GetDeviceCaps(hDC, LOGPIXELSY), 72);//改26->27字体增大
		SelectObject(hdc, CreateFontIndirect(&logfont));
		
		SetTextColor(hdc,RGB(0,0,255));
        SetBkMode(hdc, TRANSPARENT);
        if(1==flag){
            char s[20];
            sprintf(s, "(%u，%u)",x,y);
		    TextOut(hdc,
		    (rect.right-strlen(s))/2,   //cxChar = LOWORD (GetDialogBaseUnits ());
		    (rect.bottom-HIWORD (GetDialogBaseUnits ()))/2-rect.bottom/3
		    ,s,strlen(s));
        }
		if(2==flag){
		    TextOut(hdc,
		    (rect.right-strlen(text))/2,   //cxChar = LOWORD (GetDialogBaseUnits ());
		    (rect.bottom-HIWORD (GetDialogBaseUnits ()))/2-rect.bottom/3
		    ,text,strlen(text));
		}
		
		MoveWindow(hBtn,rect.right/2-70 /*X坐标*/, rect.bottom/2*3 /*Y坐标*/, 140 /*宽度*/, 50/*高度*/,0);
		
		EndPaint(hWnd, &ps);       //结束绘图并释放环境句柄
		break;
	}
		
		


	case WM_DESTROY:		//窗口销毁消息
		DeleteObject(hFont);	//删除创建的字体
		PostQuitMessage(0);
		break;
	case WM_COMMAND:
       
        if (LOWORD(wParam) == 1) {
            // 按钮2被点击，执行操作
            
            LPWSTR s[20];
            GetWindowText(hBtn,s,sizeof(s));
            
            if(!strcmp(s,"开始")){
                dllcallback(&x,&y,text,&ms);
                SetTimer(hwnd, 1, ms, CALLBACK_ID_TIMER_1);
				SetWindowText(hBtn,(LPCWSTR)"停止");
			}
			//if(CompareString(GetThreadLocale(),NULL,s,strlen(s),"停止",strlen("停止"))){
			if(!strcmp(s,"停止")){
			    KillTimer(hwnd, 1);
            	SetWindowText(hBtn,(LPCWSTR)"开始");
			}
            
        }
        
    case WM_TIMER:
        switch (wParam){
            case 1:
                break;
            default:
                break;
    }
    
    //case WM_SYSCOMMAND: break;  //用于改变窗口大小
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

/*timer回调*/
void CALLBACK CALLBACK_ID_TIMER_1(HWND hwnd, UINT message, UINT_PTR iTimerID, DWORD dwTime)
{
    flag=dllcallback(&x,&y,text,&ms);
	InvalidateRect(hwnd, NULL, TRUE);
	return;
}

