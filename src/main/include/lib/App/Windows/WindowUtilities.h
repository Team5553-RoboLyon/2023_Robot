#pragma once
#include <iostream>
#include <windows.h>
#include <CommCtrl.h>


// Fonction DE fenêtre pour subclasser certains contrôles et modifier leur comportement.
LRESULT CALLBACK subEditProcR(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData );
LRESULT CALLBACK subEditProcRp(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
LRESULT CALLBACK subEditProcN(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
LRESULT CALLBACK subEditProcZ(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

// Fonction callback à utiliser avec la fonction "EnumChildWindows"
BOOL CALLBACK  EnableCtrl_Callback(HWND   hwnd, LPARAM lParam);
BOOL CALLBACK  EnableCtrlButLParam_Callback(HWND   hwnd, LPARAM lParam);

BOOL CALLBACK  DisableCtrl_Callback(HWND   hwnd, LPARAM lParam);
BOOL CALLBACK  DisableCtrlButLParam_Callback(HWND   hwnd, LPARAM lParam);

BOOL CALLBACK  DestroyCtrl_Callback(HWND   hwnd, LPARAM lParam);
BOOL CALLBACK  DestroyCtrlButLParam_Callback(HWND   hwnd, LPARAM lParam);
BOOL CALLBACK  DestroyWindowLParam_Callback(HWND   hwnd, LPARAM lParam);
BOOL CALLBACK  SetFont(HWND child, LPARAM font);

Nu32 GetNu32FromEditControl(const HWND hctrl);
Nu32 GetNu32FromEditControl(const HWND hctrl, const Nu32 min, const Nu32 max);
Nf32 GetNf32FromEditControl(const HWND hctrl, const Nchar* pformat = "%.4f");
Nf32 GetNf32FromEditControl(const HWND hctrl, const Nf32 min, const Nf32 max, const Nchar* pformat = "%.4f");