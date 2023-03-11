#include <iostream>
#include <windows.h>
#include "../../../resource.h"
#include "../../N/NTexture.h"
#include "../../N/NEditor.h"
#include "../../N/NObject.h"
#include "../../N/NCore.h"

#include "../../NL/NLProject.h"
#include "WindowUtilities.h"
#include "WndProcPlayground.h"

//extern NXNODELIST		MainXlist;
//extern NOBJECT			PlaygroundObject;;

extern NLPROJECT					Project;

HWND hwnd_Playground = NULL;

#define TXTOUT_SIZE		128

static void init(HWND hwnd)
{
	Nchar				txtout[TXTOUT_SIZE];

	sprintf(txtout, "%.4f", Project.m_playground.m_fieldImagePixelPerMeter);
	SetWindowText(GetDlgItem(hwnd, IDC_EDIT_FIELD_IMAGE_PPC), txtout);

	sprintf(txtout, "%.4f", Project.m_playground.getFieldTrueWidth());
	SetWindowText(GetDlgItem(hwnd, IDC_EDIT_FIELD_WIDTH), txtout);

	sprintf(txtout, "%.4f", Project.m_playground.getFieldTrueLength());
	SetWindowText(GetDlgItem(hwnd, IDC_EDIT_FIELD_LENGTH), txtout);
}


BOOL CALLBACK WndProc_Playground(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	Nf32		f;


	switch (message)
	{
	case WM_INITDIALOG:
		init(hwndDlg);

		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCANCEL:
			DestroyWindow(hwndDlg);
			hwnd_Playground = NULL;
			return TRUE;

		case IDC_LOAD_PLAYGROUND_IMAGE:
			char filename[MAX_PATH];

			OPENFILENAME ofn;
			ZeroMemory(&filename, sizeof(filename));
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
			ofn.lpstrFilter = "TGA Files\0*.tga\0\0";
			ofn.lpstrFile = filename;
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrTitle = "Select a TGA File for Playground.";
			ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;
			if (GetOpenFileNameA(&ofn))
			{
				NSwitchToFullPathFileSystem();
				Project.m_playground.setTexture( NCreateTextureFromTga("playground", filename),NTRUE );
				Project.m_playground.setTextureFilename(filename);
				NSwitchToRootRelativePathFileSystem();
			}
			return TRUE;

		case IDC_EDIT_FIELD_IMAGE_PPC:
			switch (HIWORD(wParam))
			{
			case EN_KILLFOCUS:
				f = GetNf32FromEditControl((HWND)lParam, 0.0f, NF32_MAX, "%.5f");
				Project.m_playground.setPpM(f,NTRUE);
				break;
			default:
				break;
			}
			return TRUE;

		case IDC_EDIT_FIELD_WIDTH:
			switch (HIWORD(wParam))
			{
			case EN_KILLFOCUS:
				f = GetNf32FromEditControl((HWND)lParam, 0.0f, NF32_MAX, "%.3f");
				Project.m_playground.setFieldTrueWidth(f);
				break;
			default:
				break;
			}
			return TRUE;


		case IDC_EDIT_FIELD_LENGTH:
			switch (HIWORD(wParam))
			{
			case EN_KILLFOCUS:
				f = GetNf32FromEditControl((HWND)lParam, 0.0f, NF32_MAX, "%.3f");
				Project.m_playground.setFieldTrueLength(f);
				break;
			default:
				break;
			}
			return TRUE;


/*
		case IDC_EDIT_FIELD_WIDTH:
			switch (HIWORD(wParam))
			{
			case EN_KILLFOCUS:
				f = GetNf32FromEditControl((HWND)lParam, 0.0f, NF32_MAX, "%.2f");
			//	Project.m_playground.updatePpcFromImageWidth(f,NTRUE);
				break;
			default:
				break;
			}

			return TRUE;
		case IDC_EDIT_FIELD_LENGTH:
			switch (HIWORD(wParam))
			{
			case EN_KILLFOCUS:
				f = GetNf32FromEditControl((HWND)lParam, 0.0f, NF32_MAX, "%.2f");
			//	Project.m_playground.updatePpcFromImageHeight(f, NTRUE);
				break;
			default:
				break;
			}
			return TRUE;
*/
		default:
			break;
		}
	}
	return FALSE;
}
