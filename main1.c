#include <Windows.h>
#include<time.h>
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HANDLE ht;
HINSTANCE hInst;
HWND hwnd;         //���ھ��
int x,y;
void T(){
	while(1){
		x=rand()%8+1;
		if(1==x) y=rand()%4+1;
		if(2==x||3==x||7==x||8==x||9==x) y=rand()%5+1;
		if(4==x||5==x||6==x) y=rand()%6+1;
		InvalidateRect(hwnd, NULL, TRUE);
		Sleep(30);
	}
	return;
}


int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	PSTR szCmdLine,
	int iCmdShow
) {
	ht=NULL;
	static TCHAR szClassName[] = TEXT("DMWin");   //��������
	
	MSG msg;           //��Ϣ
	WNDCLASS wndclass; // ������

	hInst = hInstance;

	 /**********�ڢٲ���ע�ᴰ����**********/
	//Ϊ������ĸ����ֶθ�ֵ
	wndclass.style = CS_HREDRAW | CS_VREDRAW;	//���ڷ��
	wndclass.lpfnWndProc = WndProc;		//���ڹ���
	wndclass.cbClsExtra = 0;			//��ʱ����Ҫ���
	wndclass.cbWndExtra = 0;			//��ʱ����Ҫ���
	wndclass.hInstance = hInstance;			//��ǰ���ھ��
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);	//����ͼ��
	wndclass.hCursor = LoadIcon(NULL, IDC_ARROW);		//�����ʽ
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	//���ڱ�����ˢ
	wndclass.lpszMenuName = NULL;		//���ڲ˵�
	wndclass.lpszClassName = szClassName; // ��������

	//ע�ᴰ��
	RegisterClass(&wndclass);


	/*****�ڢڲ�����������(���ô�����ʾ����)*****/
	hwnd = CreateWindow(
		szClassName,		//�����������
		TEXT("������V1.0"),		//���ڱ��⣨�����ڱ�������
		WS_OVERLAPPEDWINDOW,	//���ڷ��
		800,			//��ʼ��ʱx���λ��
		1000,			//��ʼ��ʱy���λ��
		500,			//���ڿ��
		300,			//���ڸ߶�
		NULL,			//�����ھ��
		NULL,			//���ڲ˵����
		hInstance,		//��ǰ���ڵľ��
		NULL			//��ʹ�ø�ֵ
	);

	//��ʾ����
	ShowWindow(hwnd, iCmdShow);
	//���£����ƣ�����
	UpdateWindow(hwnd);


	/**********�ڢ۲�����Ϣѭ��**********/
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

/**********�ڢܲ������ڹ���**********/
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;		  //�豸�������
	PAINTSTRUCT ps;   //�洢��ͼ�����������Ϣ
	int wmId, wmEvent;
	static HFONT hFont;
	static HWND hBtn;
	RECT rect={};
	switch (message)
	{
	case WM_CREATE:		
		srand((unsigned)time(NULL));
		
		GetClientRect(hWnd,&rect);
		//�����߼�����
		hFont = CreateFont(
			-15/*�߶�*/, -7.5/*���*/, 0/*���ù�*/, 0/*���ù�*/, 700 /*һ�����ֵ��Ϊ400*/,
			FALSE/*����б��*/, FALSE/*�����»���*/, FALSE/*����ɾ����*/,
			DEFAULT_CHARSET,  //����ʹ��Ĭ���ַ��������������� _CHARSET ��β�ĳ�������
			OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,  //���в������ù�
			DEFAULT_QUALITY,  //Ĭ���������
			FF_DONTCARE,  //��ָ��������*/
			TEXT("΢���ź�" ) //������
		);

		//������ť�ؼ�
		hBtn = CreateWindow(
			TEXT("button"), //��ť�ؼ�������
			TEXT("��ʼ"),
			WS_CHILD | WS_VISIBLE | WS_BORDER | BS_FLAT/*��ƽ��ʽ*/,
			rect.right/2-70 /*X����*/, rect.bottom/2*3 /*Y����*/, 140 /*���*/, 50/*�߶�*/,
			hWnd, (HMENU)1 /*�ؼ�Ψһ��ʶ��*/, hInst, NULL
		);

		SendMessage(hBtn, WM_SETFONT, (LPARAM)hFont, NULL); //���ð�ť����
		break;

	case WM_PAINT:{
		char s[20];
		GetClientRect(hWnd,&rect);
		hdc = BeginPaint(hWnd, &ps);    //��ʼ��ͼ�����ػ������
		
		LOGFONT logfont�� //�ı��������
        ZeroMemory(&logfont, sizeof(LOGFONT));
        logfont.lfCharSet=DEFAULT_CHARSET;
		logfont.lfWeight=1000;
		logfont.lfHeight = -MulDiv(26, GetDeviceCaps(hDC, LOGPIXELSY), 72);//��26->27��������
		SelectObject(hdc, CreateFontIndirect(&logfont));
		
		SetTextColor(hdc,RGB(0,0,255));
        SetBkMode(hdc, TRANSPARENT);
		sprintf(s, "(%u��%u)",x,y);
		TextOut(hdc,
		(rect.right-strlen(s))/2,   //cxChar = LOWORD (GetDialogBaseUnits ());
		(rect.bottom-HIWORD (GetDialogBaseUnits ()))/2-rect.bottom/3
		,s,strlen(s));
		
		MoveWindow(hBtn,rect.right/2-70 /*X����*/, rect.bottom/2*3 /*Y����*/, 140 /*���*/, 50/*�߶�*/,0);
		
		EndPaint(hWnd, &ps);       //������ͼ���ͷŻ������
		break;
	}
		
		


	case WM_DESTROY:		//����������Ϣ
		DeleteObject(hFont);	//ɾ������������
		PostQuitMessage(0);
		break;
	case WM_COMMAND:
       
        if (LOWORD(wParam) == 1) {
            // ��ť2�������ִ�в���
            
            LPWSTR s[20];
            GetWindowText(hBtn,s,sizeof(s));
            
            if(!strcmp(s,"��ʼ")){
            //	MessageBox(NULL,s,"��Ϣ1", MB_OK);
            	if(!ht){
            		ht=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)&T,NULL,0,NULL);
				}
            	else{
            		ResumeThread(ht);
				}
				SetWindowText(hBtn,(LPCWSTR)"ֹͣ");
			}
			//if(CompareString(GetThreadLocale(),NULL,s,strlen(s),"ֹͣ",strlen("ֹͣ"))){
			if(!strcmp(s,"ֹͣ")){
			//	MessageBox(NULL,s,"��Ϣ2", MB_OK);
            	SuspendThread(ht);
            	SetWindowText(hBtn,(LPCWSTR)"��ʼ");
			}
            
        }
       
    //case WM_SYSCOMMAND: break;  //���ڸı䴰�ڴ�С
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


