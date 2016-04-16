#pragma once
#include "stdafx.h"
#include <CommCtrl.h>



HWND				CreateSimpleRebar(HWND hwndOwner, HWND hwndToolbar, HINSTANCE hInst);
HWND				CreateAdvancedRebar(HWND hwndOwner, HWND hwndFileToolbar, HWND hwndSettingsToolbar, HWND hwndBrushToolbar, HWND hwndEntityComboBox, HINSTANCE hInst);
HWND				CreateFileToolbar(HWND hwndOwner, HINSTANCE hInst);
HWND				CreateSettingsToolbar(HWND hwndOwner, HINSTANCE hInst);
HWND				CreateBrushToolbar(HWND hwndOwner, HINSTANCE hInst);
HWND				CreateEntityComboBox(HWND hwndOwner, HINSTANCE hInst);