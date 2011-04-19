// Tetris.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "Tetris.h"

#include "DxLib.h"
#include "rogic.h"

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	if ( DxLib_Init() == -1 ) {
		return -1;
	}

	DrawPixel( 320, 240, 0xffff);

	WaitKey();

	DxLib_End();
	return 0;
}