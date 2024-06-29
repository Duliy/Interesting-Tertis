// autotetris.cpp: ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "autotetris.h"
#include <windows.h>
#include <WinUser.h>
#define MAX_LOADSTRING 100

// ȫ�ֱ���: 
HINSTANCE hInst;                                // ��ǰʵ��
WCHAR szTitle[MAX_LOADSTRING];                  // �������ı�
WCHAR szWindowClass[MAX_LOADSTRING];            // ����������
HWND Mainhwnd;

// �˴���ģ���а����ĺ�����ǰ������: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: �ڴ˷��ô��롣

    // ��ʼ��ȫ���ַ���
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_AUTOTETRIS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ִ��Ӧ�ó����ʼ��: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_AUTOTETRIS));

    MSG msg;

    // ����Ϣѭ��: 
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

//
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_AUTOTETRIS));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_AUTOTETRIS); 
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��: 
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_BORDER | WS_SYSMENU,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}
VOID CALLBACK TimeProc(HWND hWnd, UINT message, UINT idTimer, DWORD dwTime)
{
	// ��ʱ��
	checkTetris(hWnd,VK_DOWN);
}
VOID ChangeTitle(HWND hWnd)
{
	const wchar_t* newTitle = L"Ȥζ����˹����  by��ө'sDOG��";

	SetWindowText(hWnd, newTitle);
}
// ������ ������Ϸ
VOID InitGame(HWND hWnd, INT i)
{
	// ��ʼ������
	btCurGrid.init(PAINT_YNODE,GAME_XNODE);
	iInterval = AI_SWITCH ? AI_DEFAULT_INTERVAL : DEFAULT_INTERVAL;
	iCurDif = SIMPLE;
	if( i == 1)
	{
		iCurScore = 0;
	}
	else
	{
		iCurScore = resurrection;
	}
	
	// �����׸�����ͼ��
	curTetris.random();
	// ������һ������ͼ��
	nextTetris.random();
	// ������ʱ��
	KillTimer(hWnd, TIMER_ID);

	SetTimer(hWnd, TIMER_ID, iInterval, TimeProc);

}

// ������ ��ʼ�������λ�úʹ�С
VOID InitWndPostionSize(HWND hWnd)
{
	RECT rect;
	int nWinX, nWinY, nClientX, nClientY;
	//��ȡ���ڴ�С  
	GetWindowRect(hWnd, &rect);
	nWinX = rect.right - rect.left;
	nWinY = rect.bottom - rect.top;
	//��ȡ�ͻ�����С  
	GetClientRect(hWnd, &rect);
	nClientX = rect.right - rect.left;
	nClientY = rect.bottom - rect.top;

	// ��ȡ��ǰ��Ļ�Ŀ�ߣ������ڷ�����������
	UINT iScreenX = GetSystemMetrics(SM_CXSCREEN);
	UINT iScreenY = GetSystemMetrics(SM_CYSCREEN);
	// ��������λ�����С
	MoveWindow(hWnd, (iScreenX - PAINT_WIDTH)/2, (iScreenY - PAINT_HEIGHT)/3, PAINT_WIDTH + nWinX - nClientX, PAINT_HEIGHT + nWinY - nClientY, true);
}
// ������ ��������
VOID playBGM()
{
	mciSendString(TEXT("open D:\\VS2022\\auto_tetris\\music\\d25.mp3 alias bgm"), NULL, 0, NULL);
	mciSendString(TEXT("open D:\\VS2022\\auto_tetris\\music\\clean.mp3 alias clean"), NULL, 0, NULL);
	mciSendString(TEXT("open D:\\VS2022\\auto_tetris\\music\\boom.mp3 alias boom"), NULL, 0, NULL);
	mciSendString(TEXT("open D:\\VS2022\\auto_tetris\\music\\clock.mp3 alias clock"), NULL, 0, NULL);
	mciSendString(TEXT("open D:\\VS2022\\auto_tetris\\music\\button.mp3 alias button"), NULL, 0, NULL);
	mciSendString(TEXT("play bgm repeat from 0"), NULL, 0, NULL);

}
	
//�Ѷ�ѡ��ģ�飬��δ���
/*
VOID difchoice(HWND hWnd)
{
	initgraph(640, 480);
	IMAGE background1;
	loadimage(&background1, _T("C:\\Users\\21918\\Desktop\\3.png"));
	putimage(0, 0, &background1);
	setbkmode(TRANSPARENT);
	settextcolor(YELLOW);
	outtextxy(200, 50, _T("��Ϸ�Ѷ�"));
	// ѡ��
	settextcolor(WHITE);
	outtextxy(250, 150, _T("��"));
	outtextxy(250, 200, _T("��ͨ"));
	outtextxy(250, 250, _T("����"));

	// ѭ���ȴ��û�ѡ��˵�
	int x, y;
	while (true)
	{
		// �������������¼�
		if (MouseHit())
		{
			MOUSEMSG msg = GetMouseMsg();
			if (msg.uMsg == WM_LBUTTONDOWN)
			{
				x = msg.x;
				y = msg.y;
				if (x > 250 && x < 400 && y > 150 && y < 180)
				{
					iCurDif--;
					iInterval += 250;
					InitGame(hWnd);
					break;
				}
				if (x > 250 && x < 400 && y > 200 && y < 230)
				{
					InitGame(hWnd);
					break;
				}
				if (x > 250 && x < 400 && y > 250 && y < 280)
				{
					iCurDif++;
					iInterval -= 250;
					InitGame(hWnd);
					break;
				}
			}
		}
	}
}
*/
// ������ �������˵�
VOID MainBackGround(HWND hWnd)
{

	ChangeTitle(hWnd);

	//��������
	playBGM();

	// ��ʼ������
	Mainhwnd = initgraph(640, 480);

	ChangeTitle(Mainhwnd);

	// ���ر���ͼƬ
	IMAGE background;
	loadimage(&background, _T("D:\\VS2022\\auto_tetris\\base\\�����汳��.bmp"));

	// ����ͼƬ
	putimage(0, 0, &background);

	// ����������ʽ�ʹ�С
	settextstyle(50, 0, L"����");

	// ����ҳ��
	setbkmode(TRANSPARENT);
	settextcolor(RED);
	outtextxy(150, 50, _T("Ȥζ����˹����"));

	// ѡ��
	settextstyle(30, 0, L"΢���ź�");
	settextcolor(WHITE);
	outtextxy(250, 150, _T("��ʼ��Ϸ"));
	outtextxy(250, 200, _T("��������Ϣ"));
	outtextxy(250, 250, _T("��Ϸ����"));
	outtextxy(250, 300, _T("�˳���Ϸ"));


	// ѭ���ȴ��û�ѡ��˵�
	int x, y;
	HWND MainGround = GetForegroundWindow();
	while (true)
	{
		// �������������¼�
		if (MouseHit())
		{
			MOUSEMSG msg = GetMouseMsg();
			if (msg.uMsg == WM_LBUTTONDOWN)
			{
				x = msg.x;
				y = msg.y;
				if (x > 250 && x < 400 && y > 150 && y < 180)
				{
					// ������Ϸ
					ShowWindow(GetForegroundWindow(), SW_HIDE);
					InitGame(hWnd , 1);
					break;
				}
				if (x > 250 && x < 400 && y > 200 && y < 230)
				{
					initgraph(640, 480);
					IMAGE background1;
					loadimage(&background1, _T("D:\\VS2022\\auto_tetris\\base\\�����汳��.bmp"));
					putimage(0, 0, &background1);
					setbkmode(TRANSPARENT);
					settextcolor(YELLOW);
					outtextxy(200, 50, _T("��Ŀ���ƣ�Ȥζ����˹����"));
					settextcolor(WHITE);
					outtextxy(250, 150, _T("���������ң���ө��sDOG��"));
					outtextxy(250, 200, _T("�ӳ���������"));
					outtextxy(250, 250, _T("��Ա�������������"));
					outtextxy(250, 300, _T("����"));
					int x, y;
					while (true)
					{
						// �������������¼�
						if (MouseHit())
						{
							MOUSEMSG msg = GetMouseMsg();
							if (msg.uMsg == WM_LBUTTONDOWN)
							{
								x = msg.x;
								y = msg.y;
								if (x > 250 && x < 400 && y > 300 && y < 330)
								{
									closegraph();
									InitWndPostionSize(hWnd);
									MainBackGround(hWnd);
								}
							}
						}
					}
					break;
				}
				if (x > 250 && x < 400 && y > 250 && y < 280)
				{
					system("start https://tetris.huijiwiki.com/wiki/%E4%BF%84%E7%BD%97%E6%96%AF%E6%96%B9%E5%9D%97%E5%87%86%E5%88%99");
				}
				if (x > 250 && x < 400 && y > 300 && y < 330)
				{
					exit(0);
				}
			}
		}
	}
}

// ������ ���Ʊ���
VOID DrawBackGround(HDC hdc)
{
	// ������Ϸ���򱳾�ͼƬ
	HBITMAP hGameBitmap = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP3));
	if (hGameBitmap == NULL)
	{
		PostQuitMessage(0);
		// �������λͼʧ�ܣ��������
		return;
	}

	// ������ָ���豸�����ļ��ݵ��ڴ��豸������
	HDC hMemDC = CreateCompatibleDC(hdc);
	if (hMemDC == NULL)
	{
		DeleteObject(hGameBitmap);
		return;
	}

	// ѡ��λͼ���ڴ��豸��������
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hGameBitmap);

	// ��ȡλͼ�ĳߴ�
	BITMAP gameBitmap;
	GetObject(hGameBitmap, sizeof(BITMAP), &gameBitmap);

	// ����λͼ��Ŀ���豸�����ģ���Ϸ����
	BitBlt(hdc, GAME_POSITION.iLeftX, GAME_POSITION.iLeftY ,
		GAME_POSITION.iRightX - GAME_POSITION.iLeftX,
		GAME_POSITION.iRightY - GAME_POSITION.iLeftY,
		hMemDC, 0, 0, SRCCOPY);

	// �ָ��ڴ��豸�������еľ�λͼ
	SelectObject(hMemDC, hOldBitmap);

	// ɾ����Ϸ���򱳾�λͼ����
	DeleteObject(hGameBitmap);

	// ������Ϣ������ͼƬ
	HBITMAP hInfoBitmap = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP2));
	if (hInfoBitmap == NULL)
	{
		PostQuitMessage(0);
		// �������λͼʧ�ܣ��������
		DeleteDC(hMemDC);
		return;
	}

	// ѡ����Ϣ��λͼ���ڴ��豸��������
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hInfoBitmap);

	// ��ȡ��Ϣ��λͼ�ĳߴ�
	BITMAP infoBitmap;
	GetObject(hInfoBitmap, sizeof(BITMAP), &infoBitmap);

	// ����λͼ��Ŀ���豸�����ģ���Ϣ����
	BitBlt(hdc, INFO_POSITION.iLeftX, INFO_POSITION.iLeftY,
		INFO_POSITION.iRightX - INFO_POSITION.iLeftX,
		INFO_POSITION.iRightY - INFO_POSITION.iLeftY,
		hMemDC, 0, 0, SRCCOPY);

	// �ָ��ڴ��豸�������еľ�λͼ
	SelectObject(hMemDC, hOldBitmap);

	// ɾ���ڴ��豸�����ĺ���Ϣ��λͼ����
	DeleteDC(hMemDC);
	DeleteObject(hInfoBitmap);
}

// ������ ������Ϣ��
VOID DrawInfo(HDC hdc)
{
	//��ʼ����߷�
	ifstream file(TEXT("D:\\VS2022\\auto_tetris\\record.txt"));
	if (!file.is_open())
	{
		exit(0);
	}
	else
	{
		file >> Highestsocre;
	}
	file.close();

	// ��һ�����䷽��
	SetTextColor(hdc, RGB(255, 255, 255));
	RECT rect;
	HFONT hFont = CreateFont(25, 0, 0, 0, FW_HEAVY, 0, 0, 0, UNICODE, 0, 0, 0, 0, L"΢���ź�");
	SelectObject(hdc, hFont);
	SetBkMode(hdc, TRANSPARENT);
	SetBkColor(hdc, RGB(255,255,0));
	
	NEXT_FONT_POSITION.setRect(rect);
	DrawText(hdc, TEXT("��һ��"), _tcslen(TEXT("��һ��")), &rect , 0);

	Writer_POSITION.setRect(rect);
	DrawText(hdc, TEXT("���ϴ�ѧ"), _tcslen(TEXT("���ϴ�ѧ")), &rect, 0);

	TEAM_POSITION.setRect(rect);
	DrawText(hdc, TEXT("��ө'sDOG��"), _tcslen(TEXT("��ө'sDOG��")), &rect, 0);

	LEVEL_FONT_POSITION.setRect(rect);
	TCHAR tcDif[10];
	LPCTSTR lpctDif = TEXT("�Ѷ�: %d");
	wsprintf(tcDif, lpctDif , iCurDif);
	DrawText(hdc, tcDif, _tcslen(tcDif), &rect , 0);

	SCORE_FONT_POSITION.setRect(rect);
	TCHAR tcScore[10];
	LPCTSTR lpctScore = TEXT("�÷�: %d");
	wsprintf(tcScore, lpctScore, iCurScore);
	DrawText(hdc, tcScore, _tcslen(tcScore), &rect , 0);

	BOOM_FONT_POSITION.setRect(rect);
	TCHAR tcBoom[10];
	LPCTSTR lpctBoom = TEXT("ը��: %d");
	wsprintf(tcBoom, lpctBoom, iCurBoom);
	DrawText(hdc, tcBoom, _tcslen(tcBoom), &rect, 0);

	CLOCK_FONT_POSITION.setRect(rect);
	TCHAR tcClock[10];
	LPCTSTR lpctClock = TEXT("ʱͣ����: %d");
	wsprintf(tcClock, lpctClock, iCurClock);
	DrawText(hdc, tcClock, _tcslen(tcClock), &rect, 0);

	HIGHEST_FONT_POSITION.setRect(rect);
	TCHAR tcHigh[10];
	LPCTSTR lpctHigh = TEXT("��߷�: %d");
	wsprintf(tcHigh, lpctHigh, Highestsocre);
	DrawText(hdc, tcHigh, _tcslen(tcHigh), &rect, 0);

	DeleteObject(hFont);
}
// ���������ȡ��ǰ�ڵ�λ��
RectPosition getRectPosition(BYTE x, BYTE y)
{
	RectPosition rectPs;
	rectPs.iLeftX = BLOCK_SIZE + y*NODE_SIZE;
	rectPs.iLeftY = BLOCK_SIZE + x*NODE_SIZE;
	rectPs.iRightX = rectPs.iLeftX + NODE_SIZE;
	rectPs.iRightY = rectPs.iLeftY + NODE_SIZE;
	return rectPs;
}
// ���������Ʒ���
VOID DrawTetris(HDC hdc)
{
	RectPosition ps;
	HPEN hPen = (HPEN)GetStockObject(NULL_PEN);
	SelectObject(hdc, hPen);
	// ����Game����
	for (int i =0; i< PAINT_YNODE; i++)
	{
		for (int j =0; j< GAME_XNODE; j++)
		{
			if (btCurGrid[i][j] > 0)
			{
				// ���Ƶ�ǰ�ڵ�
				SelectObject(hdc, getHbrush(btCurGrid[i][j]));
				ps = getRectPosition(i, j);
				Rectangle(hdc, ps.iLeftX, ps.iLeftY, ps.iRightX, ps.iRightY);
			}
		}
	}
	// ������������ͼ��
	SelectObject(hdc, getHbrush(curTetris.color));
	for (int i =0;i < 4; i++)
	{
		ps = getRectPosition(curTetris.positions[i].x , curTetris.positions[i].y);
		Rectangle(hdc, ps.iLeftX, ps.iLeftY, ps.iRightX, ps.iRightY);
	}
	// ������һ������ͼ��
	int move_x = 8 * NODE_SIZE + BLOCK_SIZE;
	int move_y = 2 * NODE_SIZE + BLOCK_SIZE;
	SelectObject(hdc, getHbrush(nextTetris.color));
	for (int i =0;i < 4; i++)
	{
		ps = getRectPosition(nextTetris.positions[i].x , nextTetris.positions[i].y);
		Rectangle(hdc, ps.iLeftX + move_x, ps.iLeftY + move_y, ps.iRightX+ move_x, ps.iRightY +move_y);
	}
}
// ������Ϸ�Ѷ�
VOID setLevel(HWND hWnd)
{
	BOOL SimpleToGeneral = iCurDif < GENERAL && iCurScore / 10 >= GENERAL_SCORE;
	BOOL GeneralToDifficult = iCurDif < DIFFICULT && iCurScore / 10 >= DIFFICULT_SCORE;
	BOOL DifficultToEst = iCurDif < DIFFICULTEST && iCurScore / 10 >= DIFFICULTEST_SCORE;
	
	// �����Ѷ�
	if (SimpleToGeneral || GeneralToDifficult || DifficultToEst)
	{
		iCurDif ++;
		iInterval -= 250;
		KillTimer(hWnd , TIMER_ID);
		SetTimer(hWnd , TIMER_ID, iInterval , TimeProc);
	}
}
// �������
VOID finishDown(HWND hWnd)
{
	// ����btCurGrid��ǰ��������
	btCurGrid.addCurTerisToGrid(curTetris);
	// �������䷽�飬��������nextTetris
	curTetris = nextTetris;
	nextTetris.random();
	// ����
	iCurScore = iCurScore + ( btCurGrid.clearLine() * 10 );
	//���µ���״̬
	if ((iCurScore / 10 - totalBoom) > 0)
	{
		totalBoom++;
		iCurBoom++;
	}
	if ((iCurScore / 10 - totalClock) > 0)
	{
		totalClock++;
		iCurClock++;
	}
	//�����߷ֱ䶯
	if (iCurScore > Highestsocre)
	{
		Highestsocre = iCurScore;
		string file_name = "record.txt";
		ofstream file_writer(file_name, ios_base::out);
		file_writer << iCurScore << "\n";
		file_writer.close();

	}
	// �ж�����λ���Ƿ�ռ��
	if (btCurGrid.isExist(curTetris))
	{
		resurrection = iCurScore;
		// TODO ��Ϸ����
		KillTimer(hWnd, TIMER_ID);
		int res = MessageBox(hWnd, TEXT("��,�Ƿ�ۿ���渴�"),TEXT("��Ϸ����"),MB_YESNO);
		if(res == IDYES)
		{ 
			system("start http://rcxq.swu.edu.cn/");
			std::this_thread::sleep_for(std::chrono::seconds(5));
			int res1 = MessageBox(hWnd, TEXT("����һ�ΰɣ�"), TEXT("лл֧��"), MB_OK);
			if (res1 == IDOK)
			{
				InitGame(hWnd, 0);
			}
			return;
		}
		if (res == IDNO)
		{
			return;
		}
		
	}
	// ������Ϸ�Ѷ�(aiģʽ����Ҫ)
	if (!AI_SWITCH)
	{
		setLevel(hWnd);
		InvalidateRect(hWnd, NULL, TRUE);
	}
	else
	{
		InvalidateRect(hWnd, NULL, TRUE);
		// ���ΪAIģʽ����������
		AIHandle(hWnd);
	}
}

// ���ͼ�������Ƿ�Խ��/��� / ����
VOID checkTetris(HWND hWnd, INT type)
{
	int maxX , minX , maxY , minY;
	Tetris temp = curTetris;
	switch (type)
	{
		case VK_LEFT: // ��
			temp.left();
			mciSendString(TEXT("play button from 0"), NULL, 0, NULL);
			break;
		case VK_RIGHT: // ��
			temp.right();
			mciSendString(TEXT("play button from 0"), NULL, 0, NULL);
			break;
		case VK_UP: // ��
			temp.rotate();
			mciSendString(TEXT("play button from 0"), NULL, 0, NULL);
			break;
		case VK_DOWN: // ��(���������½�Ч��)
			temp.down(); 
			break;
		case VK_TAB:
			if (iCurScore > 0)
			{
				temp.check();
				iCurScore--;
				break;
			}
	}
	temp.getMaxMin(minX , maxX , minY , maxY);

	// �ж��Ƿ�Խ��
	if (maxX > 19 )
	{
		// �����ײ����������
		finishDown(hWnd);
		return;
	}
	if (minX < 0 || maxY > 9 || minY < 0)
	{
		// �������ұ߽�
		return;
	}
	// �ж�λ���Ƿ�ռ��
	if (btCurGrid.isExist(temp))
	{
		if (type == VK_DOWN )
		{
			// ���䵽�Ѵ��ڽڵ��Ϸ����������
			finishDown(hWnd);
		}
		return;
	}
	// ��������
	curTetris = temp;
	InvalidateRect(hWnd, NULL, TRUE);
}
//
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
//
//  WM_COMMAND  - ����Ӧ�ó���˵�
//  WM_PAINT    - ����������
//  WM_DESTROY  - �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int res;
	int res1;
    switch (message)
    {
	case WM_CREATE:
		{
			// ��ʼ�������λ�úʹ�С
			InitWndPostionSize(hWnd);
			MainBackGround(hWnd);
		}
		break;
	case WM_KEYDOWN:
		{
			if (!AI_SWITCH) // ���Զ���Ϸģʽ�� ���̲�����
			{
				checkTetris(hWnd,wParam);
			}
		}
		break;
	case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // �����˵�ѡ��: 
            switch (wmId)
            {
			case IDM_COMMON:
				AI_SWITCH = false;
				InitGame(hWnd , 1 );
				break;
			case IDM_AI:
				res = MessageBox(hWnd, TEXT("�ۿ���漤��ϵͳ��"), TEXT("�޵�ģʽ"), MB_YESNO);
				if(res == IDYES)
				{
					system("start http://rcxq.swu.edu.cn/");
					std::this_thread::sleep_for(std::chrono::seconds(5));
					res1 = MessageBox(hWnd, TEXT("������׼���Ѳ���"), TEXT("лл֧��"), MB_OK);
					if (res1 == IDOK)
					{
						AI_SWITCH = true;
						InitGame(hWnd , 1 );
						AIHandle(hWnd);
						break;
					}
				}
				if (res == IDNO)
				{
					break;
				}
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
			case IDM_BOOM:
				//ը��ģ��
				if (iCurBoom >= 1)
				{
					iCurBoom--;
					mciSendString(TEXT("play boom from 0"), NULL, 0, NULL);
					nextTetris.random();
				}
				break;
				
			case IDM_CLOCK:
				//ʱ����ͣģ��
				if (iCurClock >= 1)
				{
					iCurClock--;
					mciSendString(TEXT("play clock from 0"), NULL, 0, NULL);
					Sleep(5000);
				}
				break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
			// ���Ʊ���
			DrawBackGround(hdc);
			// ������Ϣ��
			DrawInfo(hdc);
			// ����ͼ��
			DrawTetris(hdc);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// �����ڡ������Ϣ�������
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

// ����������AIģʽ
VOID AIHandle(HWND hWnd)
{
	// Pierre Dellacherie ��ȡ�����ŵ�
	getBestPosition();
	// �Զ��ƶ������λ�ò���������
	aiMoveTetris(hWnd);
}
// ��ȡ������ּ���λ��
VOID getBestPosition()
{
	bestScore = 0x80000000;
	int maxX , minX , maxY , minY;
	algoTetris = curTetris;	
	Tetris temp;
	//��ȡ����ˮƽ��
	getHorizonIndex(); 
	// ͼ��������״̬
	for (int i = 0 ; i < 4 ; i ++)
	{
		algoTetris.getMaxMin(minX , maxX , minY , maxY);
		int num = 10 - (maxY - minY); // ÿ�α任����num�����
		for (int j = 0 ; j < num ; j++)
		{
			// ��ԭ����
			algoGrid = btCurGrid; 
			temp = algoTetris;
			// ��base_tetris�ŵ�algoGrid��ָ��λ����, ������Ҫ�����ƶ�����
			int moveLeft = putTetrisToGrid(j);
			// ����
			int score = Dellacherie();
			if (score > bestScore)
			{
				bestScore = score;
				bestTetris = algoTetris;
				needRotate = i;
				needMoveLeft = moveLeft;
			}
			// ��ԭ algoTetri
			algoTetris = temp;
		}
		algoTetris.rotate();
	}
}
VOID CALLBACK AITimeProc(HWND hWnd, UINT message, UINT idTimer, DWORD dwTime)
{
	if (needRotate)
	{
		// �ȱ���
		needRotate--;
		curTetris.rotate();
	} else if (needMoveLeft > 0)
	{
		// ���ƶ�
		curTetris.left();
		needMoveLeft--;
	}
	else if(needMoveLeft < 0)
	{
		// ���ƶ�
		curTetris.right();
		needMoveLeft++;
	}
	// ���»��ƽ���
	InvalidateRect(hWnd, NULL, TRUE);
	// ������ʱ��
	if (needRotate == 0 && needMoveLeft == 0)
	{
		KillTimer(hWnd, AI_TIMER_ID);
	}
}
// ���ٲ����ö�ʱ��
VOID CALLBACK TESTTimeProc(HWND hWnd, UINT message, UINT idTimer, DWORD dwTime)
{
	// ����btCurGrid��ǰ��������
	btCurGrid.addCurTerisToGrid(bestTetris);
	// �������䷽�飬��������nextTetris
	curTetris = nextTetris;
	nextTetris.random();
	// ����
	iCurScore = iCurScore + btCurGrid.clearLine();
	// �ж�����λ���Ƿ�ռ��
	if (btCurGrid.isExist(curTetris))
	{
		// TODO ��Ϸ����
		KillTimer(hWnd, 3);
		MessageBox(hWnd, TEXT("��"),TEXT("��Ϸ����"),MB_OK);
		return;
	}
	// ���ΪAIģʽ����������
	AIHandle(hWnd);
}
// �������ƶ������λ��
VOID aiMoveTetris(HWND hWnd)
{
	SetTimer(hWnd, AI_TIMER_ID, iInterval / 5 ,AITimeProc);
	// SetTimer(hWnd, 3, 0 ,TESTTimeProc); // ���ٲ���ʹ��
}
// ��ȡ����ˮƽ��
VOID getHorizonIndex()
{
	for (int i = 0 ; i < GAME_XNODE; i ++)
	{
		for(int j = 0 ; j < PAINT_YNODE -1;j++)
		{
			if (btCurGrid[j+1][i])
			{
				horizonIndex[i] = j;
				break;
			}
			if (j == (PAINT_YNODE -2))
			{
				horizonIndex[i] = j + 1;
			}
		}
	}

}
INT putTetrisToGrid(int col)
{
	int left_bottom_index = algoTetris.getLeftBottom(); // ��ȡ�����½ǵ�Ԫ���±�
	
	int maxDistanceRow = horizonIndex[col] - algoTetris.positions[left_bottom_index].x; // ��Ҫ�����ƶ��ľ���
	int maxDistanceCol = algoTetris.positions[left_bottom_index].y - col; // ��Ҫ�����ƶ��ľ���
	// ���������ƶ�����
	for (int j = 0 ; j < 4 ; j ++)
	{
		if (j != left_bottom_index)
		{
			int colDistance = algoTetris.positions[j].y - algoTetris.positions[left_bottom_index].y;
			int rowDistance = horizonIndex[col + colDistance] - algoTetris.positions[j].x;
			if (rowDistance < maxDistanceRow)
			{
				maxDistanceRow = rowDistance;
			}
		}
	}
	// ��algoTetris�ƶ���ָ��λλ��
	for(int j = 0 ; j < 4 ; j ++)
	{
		algoTetris.positions[j].x += maxDistanceRow;
		algoTetris.positions[j].y -= maxDistanceCol;
		algoGrid[algoTetris.positions[j].x][algoTetris.positions[j].y] = algoTetris.color;
	}
	return maxDistanceCol;
}
// AI�����㷨
INT Dellacherie()
{
	INT lh_score, re_score, rt_score, ct_score, nh_score, ws_score;
	lh_score = getLH();
	re_score = getRE();
	rt_score = getRT();
	ct_score = getCT();
	nh_score = getNH();
	ws_score = getWS();
	return lh_score * LH_WEIGHT + re_score * RE_WEIGHT + rt_score * RT_WEIGHT + ct_score * CT_WEIGHT + nh_score * NH_WEIGHT + ws_score * WS_WEIGHT;
}
// �߶�����
INT getLH() 
{
	int maxX, minX, maxY, minY;
	algoTetris.getMaxMin(minX, maxX, minY, maxY);
	return PAINT_YNODE - maxX;
} 
// ������*���׷���������
INT getRE() 
{	
	int  status , clearline = 0, cells = 0;
	
	for (int i = PAINT_YNODE -1; i >= 0 ; i--)
	{
		status = algoGrid.lineStatus(i);
		if (status == 0)
		{
			continue;
		}
		if (status == 1)
		{
			// ��
			clearline++; // ��+1
			// ���й��׷�����
			for (int j = 0 ; j < 4 ; j++)
			{
				if (algoTetris.positions[j].x == i)
				{
					cells++;
				}
			}
		}
		else 
		{
			// ��
			break;
		}
	}
	return clearline * cells;
} 
// �б仯������
INT getRT() 
{
	// TODO �����Ż����������У�������ֱ��ÿ��+2
	int lineChane = 0, lastStatus;
	for (int i = PAINT_YNODE - 1; i >= 0; i--)
	{
		lastStatus = 1; // ��ǽ��״̬Ϊ��Ч����������ͼ��
		for (int j = 0;j < GAME_XNODE; j++)
		{
			// ����һ�����Ӳ�ͬ
			if (!(lastStatus * algoGrid[i][j]) && lastStatus != algoGrid[i][j])
			{
				lineChane++;
				lastStatus = algoGrid[i][j];
			}
			// ��ǰ�����һ���ڵ�Ϊ��
			if (j == (GAME_XNODE - 1) && algoGrid[i][j] == 0)
			{
				lineChane++;
			}
		}
	}
	return lineChane;
}  
// �б仯������
INT getCT() 
{
	int colChane = 0, lastStatus;
	for (int i = 0; i < GAME_XNODE; i++)
	{
		lastStatus = 1; // ��ǽ��״̬Ϊ��Ч����������ͼ��
		for (int j = 0;j < PAINT_YNODE; j++)
		{
			// ����һ�����Ӳ�ͬ
			if (!(lastStatus * algoGrid[j][i]) && lastStatus != algoGrid[j][i])
			{
				colChane++;
				lastStatus = algoGrid[j][i];
			}
			// ��ǰ�����һ���ڵ�Ϊ��
			if (j == (PAINT_YNODE - 1) && algoGrid[j][i] == 0)
			{
				colChane++;
			}
		}
	}
	return colChane;
}  
// �ն�������
INT getNH() 
{
	int holes = 0;
	bool holeFlag = false; // �Ƿ���Կ�ʼ����ն�
	for (int i = 0; i < GAME_XNODE; i++)
	{
		holeFlag = false;
		for (int j = 0;j < PAINT_YNODE; j++)
		{
			if (!holeFlag)
			{
				if (algoGrid[j][i])
				{
					holeFlag = true;
				}
			}
			else 
			{
				if (algoGrid[j][i] == 0)
				{
					holes++;
				}
			}
		}
	}
	return holes;
}  
// ��������
INT getWS() 
{
	// TODO: ������ⲻ��ȷ
	int wells = 0;
	int center, left, right;
	for (int i = 0; i < PAINT_YNODE; i++)
	{
		for (int j = 0;j < GAME_XNODE; j++)
		{
			
			center = algoGrid[i][j];
			if (j > 0)
			{
				left = algoGrid[i][j - 1];
			}
			else
			{
				left = 1;
			}
			if (j < GAME_XNODE - 1)
			{
				right = algoGrid[i][j+1];
			}
			else
			{
				right = 1;
			}
			// �Ƿ�Ϊ��
			if (left && right && !center)
			{
				for (int k = i; k < PAINT_YNODE;k ++)
				{
					if (algoGrid[k][j] == 0)
					{
						wells++;
					}
					else 
					{
						break;
					}
				}
			}
		}
	}
	return wells;
} 
