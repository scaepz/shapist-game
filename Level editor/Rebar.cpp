#include "Rebar.h"
#include "stdafx.h"
#include <CommCtrl.h>
HWND				CreateSimpleRebar(HWND hwndOwner, HWND hwndToolbar, HINSTANCE hInst)
{

	// Check parameters.
	if ((hwndToolbar == NULL))
	{
		return NULL;
	}

	// Initialize common controls.
	INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = ICC_COOL_CLASSES | ICC_BAR_CLASSES;
	InitCommonControlsEx(&icex);

	// Create the rebar.
	HWND hwndRebar = CreateWindowEx(WS_EX_TOOLWINDOW,
		REBARCLASSNAME,
		NULL,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS |
		WS_CLIPCHILDREN | RBS_VARHEIGHT |
		CCS_NODIVIDER | RBS_BANDBORDERS,
		0, 0, 0, 0,
		hwndOwner,
		NULL,
		hInst, // global instance handle
		NULL);

	if (!hwndRebar)
	{
		return NULL;
	}

	// Initialize band info used by both bands.
	REBARBANDINFO rbBand = { sizeof(REBARBANDINFO) };
	rbBand.fMask =
		RBBIM_STYLE       // fStyle is valid.
		| RBBIM_TEXT        // lpText is valid.
		| RBBIM_CHILD       // hwndChild is valid.
		| RBBIM_CHILDSIZE   // child size members are valid.
		| RBBIM_SIZE;       // cx is valid
	rbBand.fStyle = RBBS_CHILDEDGE | RBBS_GRIPPERALWAYS;

	// Get the height of the toolbar.
	DWORD dwBtnSize = (DWORD)SendMessage(hwndToolbar, TB_GETBUTTONSIZE, 0, 0);

	// Set values unique to the band with the toolbar.
	rbBand.lpText = TEXT("");
	rbBand.hwndChild = hwndToolbar;
	rbBand.cyChild = LOWORD(dwBtnSize);
	rbBand.cxMinChild = 3 * HIWORD(dwBtnSize);
	rbBand.cyMinChild = LOWORD(dwBtnSize);
	// The default width is the width of the buttons.
	rbBand.cx = 0;

	// Add the band that has the toolbar.
	SendMessage(hwndRebar, RB_INSERTBAND, (WPARAM)-1, (LPARAM)&rbBand);


	return (hwndRebar);

}
HWND				CreateAdvancedRebar(HWND hwndOwner, HWND hwndFileToolbar, HWND hwndSettingsToolbar, HWND hwndBrushToolbar, HWND hwndEntityComboBox, HINSTANCE hInst);
/*HWND				CreateFileToolbar(HWND hWndParent, HINSTANCE g_hInst)
{
	// Declare and initialize local constants.
	const int ImageListID = 0;
	const int numButtons = 3;
	const int bitmapSize = 16;

	const DWORD buttonStyles = BTNS_AUTOSIZE;

	// Create the toolbar.
	HWND hWndToolbar = CreateWindowEx(0, TOOLBARCLASSNAME, NULL,
		WS_CHILD | TBSTYLE_WRAPABLE, 0, 0, 0, 0,
		hWndParent, NULL, g_hInst, NULL);

	if (hWndToolbar == NULL)
		return NULL;

	// Create the image list.
	g_hImageList = ImageList_Create(bitmapSize, bitmapSize,   // Dimensions of individual bitmaps.
		ILC_COLOR16 | ILC_MASK,   // Ensures transparent background.
		numButtons, 0);

	// Set the image list.
	SendMessage(hWndToolbar, TB_SETIMAGELIST,
		(WPARAM)ImageListID,
		(LPARAM)g_hImageList);

	// Load the button images.
	SendMessage(hWndToolbar, TB_LOADIMAGES,
		(WPARAM)IDB_STD_SMALL_COLOR,
		(LPARAM)HINST_COMMCTRL);

	// Initialize button info.
	// IDM_NEW, IDM_OPEN, and IDM_SAVE are application-defined command constants.

	TBBUTTON tbButtons[numButtons] =
	{
		{ MAKELONG(STD_FILENEW, ImageListID), IDM_NEW, TBSTATE_ENABLED, buttonStyles, { 0 }, 0, (INT_PTR)L"New" },
		{ MAKELONG(STD_FILEOPEN, ImageListID), IDM_OPEN, TBSTATE_ENABLED, buttonStyles, { 0 }, 0, (INT_PTR)L"Open" },
		{ MAKELONG(STD_FILESAVE, ImageListID), IDM_SAVE, 0, buttonStyles, { 0 }, 0, (INT_PTR)L"Save" }
	};

	// Add buttons.
	SendMessage(hWndToolbar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
	SendMessage(hWndToolbar, TB_ADDBUTTONS, (WPARAM)numButtons, (LPARAM)&tbButtons);

	// Resize the toolbar, and then show it.
	SendMessage(hWndToolbar, TB_AUTOSIZE, 0, 0);
	ShowWindow(hWndToolbar, TRUE);

	return hWndToolbar;

}*/
HWND				CreateSettingsToolbar(HWND hwndOwner, HINSTANCE hInst);
HWND				CreateBrushToolbar(HWND hwndOwner, HINSTANCE hInst);
HWND				CreateEntityComboBox(HWND hwndOwner, HINSTANCE hInst);