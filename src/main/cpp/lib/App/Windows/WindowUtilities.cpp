
#include "../../N/NMath.h"
#include "../../N/NErrorHandling.h"
#include "WindowUtilities.h"

/*
case WM_INITDIALOG:
{
	HWND control = GetDlgItem(hDlg, IDC_EDIT);
	SetWindowSubclass(control, OwnerEditProc, 0, 0);
	return TRUE;
}
*/
LRESULT CALLBACK subEditProcRp(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	switch (msg)
	{

	case WM_CHAR:
		if (wParam == VK_RETURN)
		{
			SetFocus(NULL);
		}
		else if ((wParam >= '0') && (wParam <= '9') || (wParam == '.') || (wParam == '+'))
			break;
		else
			return 0;

		break;

		/*
	case WM_KEYDOWN:
		switch (wParam)
		{
			case VK_RETURN:
				SetFocus(NULL);
			break;
		}
		break;
		*/
	default:
		break;
	}
	return DefSubclassProc(hwnd, msg, wParam, lParam);
}


LRESULT CALLBACK subEditProcZ(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	switch (msg)
	{

	case WM_CHAR:
		if (wParam == VK_RETURN)
		{
			SetFocus(NULL);
		}
		else if ((wParam >= '0') && (wParam <= '9') || (wParam == '-') || (wParam == '+'))
			break;
		else
			return 0;

		break;

		/*
	case WM_KEYDOWN:
		switch (wParam)
		{
			case VK_RETURN:
				SetFocus(NULL);
			break;
		}
		break;
		*/
	default:
		break;
	}
	return DefSubclassProc(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK subEditProcN(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	switch (msg)
	{

	case WM_CHAR:
		if (wParam == VK_RETURN)
		{
			SetFocus(NULL);
		}
		else if ((wParam >= '0') && (wParam <= '9'))
			break;
		else
			return 0;

		break;

		/*
	case WM_KEYDOWN:
		switch (wParam)
		{
			case VK_RETURN:
				SetFocus(NULL);
			break;
		}
		break;
		*/
	default:
		break;
	}
	return DefSubclassProc(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK subEditProcR(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	switch (msg)
	{
		
	case WM_CHAR:
		if (wParam == VK_RETURN)
		{
			SetFocus(NULL);
		}
		else if ((wParam >= '0') && (wParam <= '9')|| (wParam == '.') || (wParam == '-') || (wParam == '+'))
			break;
		else
			return 0;

		break;

		/*
	case WM_KEYDOWN:
		switch (wParam)
		{
			case VK_RETURN:
				SetFocus(NULL);
			break;
		}
		break;
		*/
	default:
		break;
	}
	return DefSubclassProc(hwnd, msg, wParam, lParam);
}



Nu32 GetNu32FromEditControl(const HWND hctrl)
{
	Nchar txt[32];

	GetWindowText(hctrl, txt, 32);
	return (Nu32)atoi(txt);
}

Nu32 GetNu32FromEditControl(const HWND hctrl, const Nu32 min, const Nu32 max)
{
	Nchar txt[32];

	GetWindowText(hctrl, txt, 32);
	Nu32 u = atoi(txt);
	u = NCLAMP(min, u, max);
	return u;
}


Nf32 GetNf32FromEditControl(const HWND hctrl, const Nchar *pformat )
{
	Nchar txt[32];
	
	GetWindowText(hctrl, txt, 32);
	Nf32 f = (Nf32)atof(txt);
	if (pformat)
	{
		sprintf(txt, pformat, f);
		SetWindowText(hctrl, txt);
	}
	return f;
}

Nf32 GetNf32FromEditControl(const HWND hctrl, const Nf32 min, const Nf32 max, const Nchar* pformat)
{
	Nchar txt[32];

	GetWindowText(hctrl, txt, 32);
	Nf32 f = (Nf32)atof(txt);
	f = NCLAMP(min, f, max);
	if (pformat)
	{
		sprintf(txt, pformat, f);
		SetWindowText(hctrl, txt);
	}
	return f;
}



BOOL CALLBACK  EnableCtrl_Callback(HWND   hwnd, LPARAM lParam)
{
	EnableWindow(hwnd, TRUE);
	return TRUE;
}

BOOL CALLBACK  EnableCtrlButLParam_Callback(HWND   hwnd, LPARAM lParam)
{
	NErrorIf(!lParam, NERROR_NULL_VALUE); // Not an error but in this case user may use 'EnableCtrl_Callback'
	if ((HWND)lParam != hwnd)
		EnableWindow(hwnd, TRUE);

	return TRUE;
}

BOOL CALLBACK  DisableCtrl_Callback(HWND   hwnd, LPARAM lParam)
{
	EnableWindow(hwnd, FALSE);
	return TRUE;
}

BOOL CALLBACK  DestroyCtrl_Callback(HWND   hwnd, LPARAM lParam)
{
	DestroyWindow(hwnd);
	return TRUE;
}


BOOL CALLBACK  DisableCtrlButLParam_Callback(HWND   hwnd, LPARAM lParam)
{
	NErrorIf(!lParam, NERROR_NULL_VALUE); // Not an error but in this case user may use 'DisableCtrl_Callback'
	if ((HWND)lParam != hwnd)
		EnableWindow(hwnd, FALSE);

	return TRUE;
}
BOOL CALLBACK  DestroyCtrlButLParam_Callback(HWND   hwnd, LPARAM lParam)
{
	NErrorIf(!lParam, NERROR_NULL_VALUE); // Not an error but in this case user may use 'DestroyCtrl_Callback'
	if ((HWND)lParam != hwnd)
		DestroyWindow(hwnd);

	return TRUE;
}

BOOL CALLBACK  DestroyWindowLParam_Callback(HWND   hwnd, LPARAM lParam)
{
	if (lParam)
	{
		Nchar classname[32];
		GetClassName(hwnd, classname, 32);
		if(!strcmp(classname,(Nchar*)lParam))
			DestroyWindow(hwnd);
	}
	else
	{
		DestroyWindow(hwnd);
	}
	return TRUE;
}
BOOL CALLBACK SetFont(HWND child, LPARAM font) {SendMessage(child, WM_SETFONT, font, true);	return true;}