// Level editor.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Level editor.h"
#include <shlobj.h>  
#include "FileReader.h"
#include "Commctrl.h"
#include "TextureManager.h"
#include "EntityManager.h"
#include "LevelManager.h"
#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
TCHAR * shapistPath;
LPSTR shapistPathFilePath = "data/configuration/shapistpath.txt";
DWORD gamestate = 0;							//
CLevelManager levelManager;


// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	Configure(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	ImportTexture(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	ManageTextures(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	CreateLevel(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	CreateEnemy(HWND, UINT, WPARAM, LPARAM);

void BrowseFolder(void);
int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.11
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_LEVELEDITOR, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LEVELEDITOR));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LEVELEDITOR));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_LEVELEDITOR);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // Store instance handle in our global variable

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	HDC memoryDC;
	static HBITMAP tile;
	static HBITMAP space;
	static int tilesize = 32;
	static char paintchar = '1';
	static int xAdjustment = 0;
	static int yAdjustment = 0;
	static HWND hRebar;
	if (gamestate == 2)
	{
		InvalidateRect(hWnd, NULL, TRUE);
		gamestate -= 1;
		HWND hWndToolbar = CreateWindowEx(0, TOOLBARCLASSNAME, NULL,
			WS_CHILD | TBSTYLE_WRAPABLE, 0, 0, 0, 0,
			hWnd, NULL, hInst, NULL);
		ShowWindow(hWndToolbar, TRUE);

	}

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case ID_FILE_CONFIGURE:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_CONFIGURE), hWnd, Configure);
			break;
		case ID_TEXTURES_IMPORT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_IMPORT_TEXTURE), hWnd, ImportTexture);
			break;
		case ID_TEXTURES_VIEW:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_MANAGE_TEXTURES), hWnd, ManageTextures);
			break;
		case ID_LEVEL_CREATELEVEL:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_NEWLEVEL), hWnd, CreateLevel);
			break;
		case ID_CREATE_ENEMY:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ENTITY_CREATE_ENEMY), hWnd, CreateEnemy);
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break; case WM_KEYDOWN:
			switch (wParam)
			{
				//Arrow keys control the scrolling
			case VK_LEFT:
				if (gamestate == 1)
				{
					xAdjustment += 4;
				}

				InvalidateRect(hWnd, NULL, TRUE);
				// Process the LEFT ARROW key. 

				break;

			case VK_RIGHT:
				if (gamestate == 1)
				{
					xAdjustment -= 4;
				}

				InvalidateRect(hWnd, NULL, TRUE);
				// Process the RIGHT ARROW key. 

				break;

			case VK_UP:
				if (gamestate == 1)
				{
					yAdjustment += 4;
				}

				InvalidateRect(hWnd, NULL, TRUE);
				// Process the UP ARROW key. 

				break;

			case VK_DOWN:
				if (gamestate == 1)
				{
					yAdjustment -= 4;
				}

				InvalidateRect(hWnd, NULL, TRUE);
				// Process the DOWN ARROW key. 

				break;
			}
		case WM_LBUTTONDOWN:
		{
							   if (gamestate == 1)
							   {

								   int x = LOWORD(lParam);
								   int y = HIWORD(lParam);
								   if (x / tilesize - xAdjustment < levelManager.level->width && y / tilesize - yAdjustment < levelManager.level->height && x / tilesize - xAdjustment >= 0 && y / tilesize - yAdjustment >= 0)
								   {
									   RECT dst;
									   dst.left = x - x % tilesize;
									   dst.right = x + tilesize;
									   dst.top = y - y %tilesize;
									   dst.bottom = y + tilesize;
									   levelManager.level->levelArray[x / tilesize - xAdjustment][y / tilesize - yAdjustment] = paintchar;
									   InvalidateRect(hWnd, &dst, TRUE);

								   }
							   }
		}
			break;
		case WM_RBUTTONDOWN:
		{	if (gamestate == 1)
		{

			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			if (x / tilesize - xAdjustment < levelManager.level->width && y / tilesize - yAdjustment < levelManager.level->height && x / tilesize - xAdjustment >= 0 && y / tilesize - yAdjustment >= 0)
			{
				RECT dst;
				dst.left = x - x % tilesize;
				dst.right = x + tilesize;
				dst.top = y - y %tilesize;
				dst.bottom = y + tilesize;
				levelManager.level->levelArray[x / tilesize - xAdjustment][y / tilesize - yAdjustment] = '0';
				InvalidateRect(hWnd, &dst, TRUE);
			}
			break;
		}}
		case WM_MOUSEMOVE:
		{
							 if (gamestate == 1)
							 {

								 int x = LOWORD(lParam);
								 int y = HIWORD(lParam);
								 if (x / tilesize - xAdjustment < levelManager.level->width && y / tilesize - yAdjustment < levelManager.level->height && x / tilesize - xAdjustment >= 0 && y / tilesize - yAdjustment >= 0)
								 {
									 if (wParam == MK_LBUTTON)
									 {
										 if (levelManager.level->levelArray[x / tilesize - xAdjustment][y / tilesize - yAdjustment] != paintchar)
										 {
											 RECT dst;
											 dst.left = x - x % tilesize;
											 dst.right = x + tilesize;
											 dst.top = y - y %tilesize;
											 dst.bottom = y + tilesize;
											 levelManager.level->levelArray[x / tilesize - xAdjustment][y / tilesize - yAdjustment] = paintchar;
											 InvalidateRect(hWnd, &dst, TRUE);
										 }
										 break;
									 }
									 else if (wParam == MK_RBUTTON)
									 {
										 if (levelManager.level->levelArray[x / tilesize - xAdjustment][y / tilesize - yAdjustment] != '0')
										 {
											 RECT dst;
											 dst.left = x - x % tilesize;
											 dst.right = x + tilesize;
											 dst.top = y - y %tilesize;
											 dst.bottom = y + tilesize;
											 levelManager.level->levelArray[x / tilesize - xAdjustment][y / tilesize - yAdjustment] = '0';;
											 InvalidateRect(hWnd, &dst, TRUE);
										 }
										 break;
									 }
								 }
							 }
		}break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			// TODO: Add any drawing code here...
			if (gamestate == 1)
			{
				memoryDC = CreateCompatibleDC(hdc);

				CLevel * l = levelManager.level;
				for (int x = 0; x < l->width; x++)
				{
					for (int y = 0; y < l->height; y++)
					{
						if (l->levelArray[x][y] == '0')
						{
							SelectObject(memoryDC, space);
						}
						else if (l->levelArray[x][y] == '1')
						{
							SelectObject(memoryDC, tile);
						}
						DWORD r = BitBlt(hdc, x * tilesize + tilesize * xAdjustment, y * tilesize + tilesize * yAdjustment, tilesize, tilesize, memoryDC, 0, 0, SRCCOPY);
						r = r;
					}
				}
				DeleteDC(memoryDC);
			}

			EndPaint(hWnd, &ps);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_CREATE:
		{
						  tile = (HBITMAP)LoadImage(NULL, "data/textures/tile.bmp", IMAGE_BITMAP, 0, 0,
							  LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
						  space = (HBITMAP)LoadImage(NULL, "data/textures/space.bmp", IMAGE_BITMAP, 0, 0,
							  LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
						  CFileReader reader;
						  reader.ReadFile2(shapistPathFilePath, shapistPath);
						  break;
		}
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


INT_PTR CALLBACK CreateLevel(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDOK)
		{
			int w, h;
			BOOL success;
			bool fill;
			w = GetDlgItemInt(hDlg, IDC_LEVELW, &success, 1);
			if (success && w > 0)
			{
				h = GetDlgItemInt(hDlg, IDC_LEVELH, &success, 1);
				if (success && h > 0)
				{
					DWORD check = SendDlgItemMessage(hDlg, IDC_LEVELFILL, BM_GETCHECK, 0, 0);
					if (check == BST_CHECKED)
					{
						fill = true;
					}
					else fill = false;

					if (levelManager.level != NULL)
					{
						DWORD result = MessageBox(hDlg, "Creating a new level will close the current level. Save changes?", "Warning", MB_YESNOCANCEL);
						if (result == IDYES)
						{
							levelManager.Save(levelManager.currentName);
						}
						else if (result == IDCANCEL)
						{
							return (INT_PTR)TRUE;
						}
					}
					LPSTR name = new TCHAR[50];
					DWORD result = GetDlgItemText(hDlg, IDC_LEVELNAME, name, 50);
					if (result > 0 || name == "")
					{
						levelManager.New(w, h, fill);
						levelManager.currentName = name;
						gamestate = 2;

					}
					else
					{
						MessageBox(hDlg, "Invalid name, use between 1 and 50 characters.", "Invalid name", MB_OK);
						return (INT_PTR)TRUE;
					}

				}
				else MessageBox(hDlg, "Invalid deanmensions", "", MB_OK);
			}
			else MessageBox(hDlg, "Invalid dimensions", "", MB_OK);
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

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

INT_PTR CALLBACK CreateEnemy(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static CEntityManager factory;
	static int animData[factory.enemyAnimations][factory.animDataTypes] = { 0 };
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
	{
						  HWND texbox = GetDlgItem(hDlg, IDC_CREATEENEMY_TEXTURE);
						  CTextureManager tManager;
						  tManager.ResetTextureCursor();
						  LPSTR textureName;
						  while (tManager.GetNextTextureName(textureName))
						  {
							  SendMessage(texbox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)textureName);
						  }
						  HWND lootbox = GetDlgItem(hDlg, IDC_NE_LOOT);
						  SendMessage(lootbox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)"nothing");
						  SendMessage(lootbox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)"Red HP");
						  SendMessage(lootbox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)"Green HP");
						  SendMessage(lootbox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)"Yellow Key");
						  SendMessage(lootbox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)"Green Key");
						  SendMessage(lootbox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)"Red Key");
						  SendMessage(lootbox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)"Purple Key");
						  SendMessage(lootbox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)"Bullet Ammo");
						  SendMessage(lootbox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)"Energy Ammo");
						  SendMessage(lootbox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)"Explosive Ammo");
						  SendMessage(lootbox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)"Shotgun Ammo");

						  HWND animbox = GetDlgItem(hDlg, IDC_NE_ANIM);
						  SendMessage(animbox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)"Idle");
						  SendMessage(animbox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)"Idle (rare)");
						  SendMessage(animbox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)"Move");
						  SendMessage(animbox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)"Attack");
						  SendMessage(animbox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)"Take damage");
						  SendMessage(animbox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)"Die");
						  SendMessage(animbox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)"Jump");

						  HWND animdatabox = GetDlgItem(hDlg, IDC_NE_DATA);
						  SendMessage(animdatabox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)"Number of frames");
						  SendMessage(animdatabox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)"Milliseconds per frame");

	}
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			CFileReader converter;

			LPSTR error = new TCHAR[100];
			LPSTR name = new TCHAR[100];
			GetDlgItemText(hDlg, IDC_CREATEENEMY_NAME, name, 100);
			LPSTR texture = new TCHAR[100];
			GetDlgItemText(hDlg, IDC_CREATEENEMY_TEXTURE, texture, 100);
			int lootIndex = SendDlgItemMessage(hDlg, IDC_NE_LOOT, CB_GETCURSEL, 0, 0);

			int width = GetDlgItemInt(hDlg, IDC_NE_WIDTH, false, true);
			int height = GetDlgItemInt(hDlg, IDC_NE_HEIGHT, false, true);
			int hp = GetDlgItemInt(hDlg, IDC_NE_HP, false, true);
			LPSTR temp = new TCHAR[15];

			GetDlgItemText(hDlg, IDC_NE_WEIGHT, temp, 15);
			float weight = converter.StringToFloat(temp);

			GetDlgItemText(hDlg, IDC_NE_JUMPSTRENGTH, temp, 15);
			float jumpstr = converter.StringToFloat(temp);

			GetDlgItemText(hDlg, IDC_NE_ACCELERATION, temp, 15);
			float acceleration = converter.StringToFloat(temp);

			GetDlgItemText(hDlg, IDC_NE_MAXRUNSPEED, temp, 15);
			float maxSpeed = converter.StringToFloat(temp);



			int frameW = GetDlgItemInt(hDlg, IDC_NE_FRAMEW, false, true);
			int frameH = GetDlgItemInt(hDlg, IDC_NE_FRAMEH, false, true);


			int ai = SendDlgItemMessage(hDlg, IDC_NE_AI, LB_GETCURSEL, 0, 0);
			int aiparam[4];
			aiparam[0] = GetDlgItemInt(hDlg, IDC_NE_AIPARAM1, false, true);
			aiparam[1] = GetDlgItemInt(hDlg, IDC_NE_AIPARAM2, false, true);
			aiparam[2] = GetDlgItemInt(hDlg, IDC_NE_AIPARAM3, false, true);
			aiparam[3] = GetDlgItemInt(hDlg, IDC_NE_AIPARAM4, false, true);

			factory.CreateEnemy(error, name, texture, width, height, hp, weight, jumpstr, acceleration, maxSpeed, lootIndex, animData, frameW, frameH, ai, aiparam);
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDSAVE)
		{
			int x, y;
			x = SendDlgItemMessage(hDlg, IDC_NE_ANIM, CB_GETCURSEL, 0, 0);
			y = SendDlgItemMessage(hDlg, IDC_NE_DATA, CB_GETCURSEL, 0, 0);
			animData[x][y] = GetDlgItemInt(hDlg, IDC_NE_VALUE, false, true);
		}
		break;
	}	
return (INT_PTR)FALSE;
}

INT_PTR CALLBACK ImportTexture(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
	{
						  CFileReader test;
						  test.FloatToString(-136);
						  return (INT_PTR)TRUE;
	}
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			LPSTR path = new TCHAR[70];
			GetDlgItemText(hDlg, IDC_TEXTURE_PATH, path, 70);
			LPSTR name = new TCHAR[40];
			GetDlgItemText(hDlg, IDC_TEXTURENAME, name, 40);
			LPSTR fullPath;
			CFileReader reader;
			fullPath = strcat(shapistPath, path);

			reader.ReadFile2(shapistPathFilePath, shapistPath);
			if (reader.FileExists(fullPath) && name != "")
			{
				CTextureManager mgr;
				if (!mgr.AddTexture(name, path))
				{
					MessageBox(hDlg, "Could not create texture, does texture name already exist?", "Error creating texture", MB_OK);
				}
			}
			else MessageBox(hDlg, "Texture file could not be found or name field is empty", "Invalid entry", MB_OK);
			delete name;
			delete path;


			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK Configure(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
	{
						  CFileReader reader;
						  reader.ReadFile2(shapistPathFilePath, shapistPath);

						  SetDlgItemText(hDlg, IDC_SHAPISTPATH, shapistPath);

						  return (INT_PTR)TRUE;
	}
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			CFileReader reader;
			int bufferSize = 255;
			LPSTR shapistPathInput = new TCHAR[bufferSize];
			GetDlgItemText(hDlg, IDC_SHAPISTPATH, shapistPathInput, bufferSize);
			reader.ReplaceFileContent(shapistPathFilePath, shapistPathInput);
			delete shapistPathInput;

			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}

		switch (wParam)
		{
		case IDC_SHAPISTPATH_RESET:
		{
									  CFileReader reader;
									  reader.ReplaceString(shapistPath, "../SideScroller/");
									  SetDlgItemText(hDlg, IDC_SHAPISTPATH, shapistPath);
									  break;
		}
		}
		break;

	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK	ManageTextures(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
	{
						  int index = 0;
						  CFileReader reader;
						  LPSTR name = NULL;
						  while (reader.ReadRow("data/textures/texturelist.txt", name, index * 2))
						  {
							  int a = SendDlgItemMessage(hDlg, IDC_TEXTURELIST, LB_ADDSTRING, 0, (LPARAM)name);
							  index++;
						  }
						  if (SendDlgItemMessage(hDlg, IDC_TEXTURELIST, LB_GETCOUNT, 0, 0) > 0)
						  {

							  SendDlgItemMessage(hDlg, IDC_TEXTURELIST, LB_SETCURSEL, 0, 0);
							  index = SendDlgItemMessage(hDlg, IDC_TEXTURELIST, LB_GETCURSEL, 0, 0);
							  name = new TCHAR[SendDlgItemMessage(hDlg, IDC_TEXTURELIST, LB_GETTEXTLEN, index, NULL) + 1];

							  SendDlgItemMessage(hDlg, IDC_TEXTURELIST, LB_GETTEXT, index, (LPARAM)name);

							  SetDlgItemText(hDlg, IDC_MANAGE_TEX_NAME, name);
							  CTextureManager mgr;
							  LPSTR path = NULL;
							  if (mgr.GetTexturePath(name, path))
								  SetDlgItemText(hDlg, IDC_MANAGE_TEX_PATH, path);
							  else
								  SetDlgItemText(hDlg, IDC_MANAGE_TEX_PATH, "<error>");

							  delete[] name;
						  }

						  return (INT_PTR)TRUE;
	}
	case WM_COMMAND:
		if (LOWORD(wParam) == ID_MANAGE_TEX_CLOSE)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDC_TEXTURELIST)
		{
			if (HIWORD(wParam) == LBN_SELCHANGE)
			{
				//update text fields when texture is selected in listbox
				int index = SendDlgItemMessage(hDlg, IDC_TEXTURELIST, LB_GETCURSEL, 0, 0);
				LPSTR name = new TCHAR[SendDlgItemMessage(hDlg, IDC_TEXTURELIST, LB_GETTEXTLEN, index, NULL) + 1];

				SendDlgItemMessage(hDlg, IDC_TEXTURELIST, LB_GETTEXT, index, (LPARAM)name);

				SetDlgItemText(hDlg, IDC_MANAGE_TEX_NAME, name);
				CTextureManager mgr;
				LPSTR path = NULL;
				if (mgr.GetTexturePath(name, path))
					SetDlgItemText(hDlg, IDC_MANAGE_TEX_PATH, path);
				else
					SetDlgItemText(hDlg, IDC_MANAGE_TEX_PATH, "<error>");

				delete[] name;
			}
		}
		else if (LOWORD(wParam) == IDC_MANAGE_TEX_DELETE)
		{
			//when delete is pressed
			//delete from textfile
			CTextureManager mgr;
			int index = SendDlgItemMessage(hDlg, IDC_TEXTURELIST, LB_GETCURSEL, 0, 0);
			LPSTR name = new TCHAR[SendDlgItemMessage(hDlg, IDC_TEXTURELIST, LB_GETTEXTLEN, index, NULL) + 1];
			SendDlgItemMessage(hDlg, IDC_TEXTURELIST, LB_GETTEXT, index, (LPARAM)name);
			mgr.RemoveTexture(name);
			//delete from list
			SendDlgItemMessage(hDlg, IDC_TEXTURELIST, LB_DELETESTRING, index, NULL);
			//set selection
			SendDlgItemMessage(hDlg, IDC_TEXTURELIST, LB_SETCURSEL, 0, NULL);
			delete[] name;
			//update text fields
			index = 0; //if there still are textures
			if (SendDlgItemMessage(hDlg, IDC_TEXTURELIST, LB_GETCOUNT, 0, 0) > 0)
			{
				name = new TCHAR[SendDlgItemMessage(hDlg, IDC_TEXTURELIST, LB_GETTEXTLEN, index, NULL) + 1];

				SendDlgItemMessage(hDlg, IDC_TEXTURELIST, LB_GETTEXT, index, (LPARAM)name);

				SetDlgItemText(hDlg, IDC_MANAGE_TEX_NAME, name);
				CTextureManager mgr;
				LPSTR path = NULL;
				if (mgr.GetTexturePath(name, path))
					SetDlgItemText(hDlg, IDC_MANAGE_TEX_PATH, path);
				else
					SetDlgItemText(hDlg, IDC_MANAGE_TEX_PATH, "<error>");

				delete[] name;
			}
			else
			{
				SetDlgItemText(hDlg, IDC_MANAGE_TEX_NAME, "");
				SetDlgItemText(hDlg, IDC_MANAGE_TEX_PATH, "");
			}
		}
		else if (LOWORD(wParam) == IDC_ADD)
		{
			DialogBox(hInst, MAKEINTRESOURCE(IDD_IMPORT_TEXTURE), hDlg, ImportTexture);
		}
	}
	return (INT_PTR)FALSE;
}

