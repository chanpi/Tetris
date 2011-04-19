#include "stdafx.h"
#include "rogic.h"

#include <time.h>

// □□■□□□□□□□□■□□　↑
// □□■□□□□□□□□■□□　　待機
// □□■□□□□□□□□■□□
// □□■□□□□□□□□■□□　↓
// □□■　　　　　　　　■□□
// □□■　　　　　　　　■□□
// □□■　　　　　　　　■□□
// □□■　　　　　　　　■□□
// □□■　　　　　　　　■□□
// □□■　　　　　　　　■□□
// □□■　　　　　　　　■□□
// □□■　　　　　　　　■□□
// □□■　　　　　　　　■□□
// □□■　　　　　　　　■□□
// □□■■■■■■■■■■□□
// □□□□□□□□□□□□□□
// □□□□□□□□□□□□□□
//


int g_board[ BOARD_HEIGHT + BOARD_BUFFER +4 ][ BOARD_WIDTH + BOARD_BUFFER * 2 ];	// +4はBLOCK_BIRTH状態のブロックを置くための領域

static void InitBoard();
static BLOCK_TYPE ChooseBlockType();
static void InitializeBlock(Block* pBlock);
static BOOL IsDeleteAvailable(int line);
static int CheckAndDeleteBlocks();


void PrintBoard()
{
	int i, j;
	PCTSTR out = NULL;
	OutputDebugString(_T("\n"));
	for (i = 0; i < BOARD_HEIGHT + BOARD_BUFFER +4; i++) {
		for (j = 0; j < BOARD_WIDTH + BOARD_BUFFER * 2; j++) {
			switch (g_board[i][j]) {
			case NONE:
				out = _T("_");
				break;
			case WALL:
				out = _T("#");
				break;
			case FIX:
				out = _T("*");
				break;
			case MOVING:
				out = _T("@");
				break;
			default:
				out = _T("?");
				break;
			}
			OutputDebugString(out);
		}
		OutputDebugString(_T("\n"));
	}
}

void PrintBlock(Block* pBlock)
{
	int i, j;
	PCTSTR out = NULL;
	for (i = 0; i < BLOCK_HEIGHT; i++) {
		for (j = 0; j < BLOCK_WIDTH; j++) {
			switch (pBlock->block[i][j]) {
			case NONE:
				out = _T("_");
				break;
			case WALL:
				out = _T("#");
				break;
			case FIX:
				out = _T("*");
				break;
			case MOVING:
				out = _T("@");
				break;
			default:
				out = _T("?");
				break;
			}
			OutputDebugString(out);
		}
		OutputDebugString(_T("\n"));
	}
}

void GameInitialize(Block* pBlock)
{
	srand((UINT)time(NULL));
	InitBoard();

	pBlock->state = BLOCK_BIRTH;
}

BOOL IsGameOver()
{
	int i, j;
	for (i = 0; i < BLOCK_HEIGHT; i++) {
		for (j = BOARD_BUFFER; j < BOARD_BUFFER+BOARD_WIDTH; j++) {
			if (g_board[i][j] == FIX) {
				return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL MoveLeft(Block* pBlock)
{
	int i, j;
	if (pBlock->x <= 0) {
		return FALSE;
	}
	for (i = 0; i < BLOCK_HEIGHT; i++) {
		for (j = 0; j < BLOCK_WIDTH; j++) {
			if ((g_board[pBlock->y+i][pBlock->x+j-1] & pBlock->block[i][j]) &&
				(g_board[pBlock->y+i][pBlock->x+j-1] != MOVING)) {
				return FALSE;
			}
		}
	}
	pBlock->x--;
	return TRUE;
}

BOOL MoveRight(Block* pBlock)
{
	int i, j;
	if (BOARD_WIDTH + BOARD_BUFFER*2 -4 <= pBlock->x) {
		return FALSE;
	}
	for (i = 0; i < BLOCK_HEIGHT; i++) {
		for (j = 0; j < BLOCK_WIDTH; j++) {
			if ((g_board[pBlock->y+i][pBlock->x+j+1] & pBlock->block[i][j]) &&
				(g_board[pBlock->y+i][pBlock->x+j+1] != MOVING)) {
				return FALSE;
			}
		}
	}
	pBlock->x++;
	return TRUE;
}

BOOL MoveDown(Block* pBlock)
{
	int i, j;
	if (BOARD_HEIGHT + 4 - 1 < pBlock->y) {
		return FALSE;
	}
	for (i = BLOCK_HEIGHT - 1; i >= 0; i--) {
		for (j = 0; j < BLOCK_WIDTH; j++) {
			if ((g_board[pBlock->y+i+1][pBlock->x+j] & pBlock->block[i][j]) &&
				(g_board[pBlock->y+1+i][pBlock->x+j] != MOVING)) {
				return FALSE;
			}
		}
	}
	pBlock->y++;
	return TRUE;
}

BOOL Rotate(Block* pBlock)
{
	int temp;

	// 回転
	//temp = pBlock->block[0][0];
	//pBlock->block[0][0] = pBlock->block[0][3];
	//pBlock->block[0][3] = pBlock->block[3][3];
	//pBlock->block[3][3] = pBlock->block[3][0];
	//pBlock->block[3][0] = temp;

	//temp = pBlock->block[0][1];
	//pBlock->block[0][1] = pBlock->block[1][3];
	//pBlock->block[1][3] = pBlock->block[3][2];
	//pBlock->block[3][2] = pBlock->block[2][0];
	//pBlock->block[2][0] = temp;

	//temp = pBlock->block[0][2];
	//pBlock->block[0][2] = pBlock->block[2][3];
	//pBlock->block[2][3] = pBlock->block[3][1];
	//pBlock->block[3][1] = pBlock->block[1][0];
	//pBlock->block[1][0] = temp;

	//temp = pBlock->block[1][1];
	//pBlock->block[1][1] = pBlock->block[1][2];
	//pBlock->block[1][2] = pBlock->block[2][2];
	//pBlock->block[2][2] = pBlock->block[2][1];
	//pBlock->block[2][1] = temp;

	int i, j;
	// 反時計回り回転
	for (i = 0; i < BLOCK_HEIGHT/2; i++) {
		for (j = 0; j < BLOCK_WIDTH - i*2 - 1; j++) {
			temp = pBlock->block[i][i+j];
			pBlock->block[i][i+j] = pBlock->block[i+j][BLOCK_WIDTH-i-1];//[i + BLOCK_WIDTH - i*2 - 1];
			pBlock->block[i+j][BLOCK_WIDTH-i-1] = pBlock->block[BLOCK_WIDTH-i-1][BLOCK_WIDTH-i-1-j];
			pBlock->block[BLOCK_WIDTH-i-1][BLOCK_WIDTH-i-1-j] = pBlock->block[BLOCK_WIDTH-i-1-j][i];
			pBlock->block[BLOCK_WIDTH-i-1-j][i] = temp;
			
		}
	}

	// 重なりチェック
	// 重なっていなければreturn;


	// 時計回り回転（もとに戻す）
	for (i = 0; i < BLOCK_HEIGHT/2; i++) {
		for (j = 0; j < BLOCK_WIDTH - i*2 - 1; j++) {
			temp = pBlock->block[BLOCK_WIDTH-i-1-j][i];
			pBlock->block[BLOCK_WIDTH-i-1-j][i] = pBlock->block[BLOCK_WIDTH-i-1][BLOCK_WIDTH-i-1-j];
			pBlock->block[BLOCK_WIDTH-i-1][BLOCK_WIDTH-i-1-j] = pBlock->block[i+j][BLOCK_WIDTH-i-1];
			pBlock->block[i+j][BLOCK_WIDTH-i-1] = pBlock->block[i][i+j];
			pBlock->block[i][i+j] = temp;
		}
	}
}

void BirthBlock(Block* pBlock)
{
	// 初期化
	InitializeBlock(pBlock);

	// ボード上にセット
	pBlock->x = BOARD_BUFFER + (BOARD_WIDTH - BLOCK_WIDTH) / 2;
	pBlock->y = 0;

	pBlock->state = BLOCK_FALL;	// 次は落ちるフェーズ
}

void FallBlock(Block* pBlock)
{
	MoveDown(pBlock);
}

void RestrictBlock(Block* pBlock)
{
	int i, j;
	for (i = 0; i < BLOCK_HEIGHT; i++) {
		for (j = 0; j < BLOCK_WIDTH; j++) {
			if (pBlock->block[i][j]) {
				g_board[pBlock->y+i][pBlock->x+j] += FIX;
			}
		}
	}

	CheckAndDeleteBlocks();
	pBlock->state = BLOCK_BIRTH;
}

//////////////////////////////////////////////////////////////

void InitBoard() {
	int i;
	for (i = 0; i < BOARD_WIDTH+BOARD_BUFFER*2; i++)  {
		g_board[BOARD_HEIGHT+4][i] = WALL;
		g_board[BOARD_HEIGHT+5][i] = WALL;
		g_board[BOARD_HEIGHT+6][i] = WALL;
	}
	for (i = 0; i < BOARD_HEIGHT+BLOCK_HEIGHT; i++) {
		g_board[i][0] = WALL;
		g_board[i][1] = WALL;
		g_board[i][2] = WALL;
		g_board[i][BOARD_BUFFER+BOARD_WIDTH] = WALL;
		g_board[i][BOARD_BUFFER+BOARD_WIDTH+1] = WALL;
		g_board[i][BOARD_BUFFER+BOARD_WIDTH+2] = WALL;
	}
}

BLOCK_TYPE ChooseBlockType()
{
	return (BLOCK_TYPE)(rand() % 7);
}

void InitializeBlock(Block* pBlock)
{
	if (pBlock == NULL) {
		OutputDebugString(_T("[ERROR] pBlock == NULL; <InitializeBlock(Block* pBlock)>"));
		return;
	}

	ZeroMemory(pBlock, sizeof(Block));
	switch (ChooseBlockType()) {
	case BLOCK_I:
		pBlock->block[0][1] = MOVING;
		pBlock->block[1][1] = MOVING;
		pBlock->block[2][1] = MOVING;
		pBlock->block[3][1] = MOVING;
		break;
	case BLOCK_L:
		pBlock->block[0][1] = MOVING;
		pBlock->block[1][1] = MOVING;
		pBlock->block[2][1] = MOVING;
		pBlock->block[2][2] = MOVING;
		break;
	case BLOCK_J:
		pBlock->block[0][2] = MOVING;
		pBlock->block[1][2] = MOVING;
		pBlock->block[2][2] = MOVING;
		pBlock->block[2][1] = MOVING;
		break;
	case BLOCK_T:
		pBlock->block[1][1] = MOVING;
		pBlock->block[2][0] = MOVING;
		pBlock->block[2][1] = MOVING;
		pBlock->block[2][2] = MOVING;
		break;
	case BLOCK_O:
		pBlock->block[1][1] = MOVING;
		pBlock->block[1][2] = MOVING;
		pBlock->block[2][1] = MOVING;
		pBlock->block[2][2] = MOVING;
		break;
	case BLOCK_Z:
		pBlock->block[1][0] = MOVING;
		pBlock->block[1][1] = MOVING;
		pBlock->block[2][1] = MOVING;
		pBlock->block[2][2] = MOVING;
		break;
	case BLOCK_S:
		pBlock->block[1][2] = MOVING;
		pBlock->block[1][3] = MOVING;
		pBlock->block[2][1] = MOVING;
		pBlock->block[2][2] = MOVING;
		break;
	}
}

int CheckAndDeleteBlocks()
{
	int lines[BOARD_HEIGHT];
	int i, j, count = 0;
	for (i = BOARD_HEIGHT+3; i >= 4; i--) {
		if (IsDeleteAvailable(i)) {
			lines[count++] = i;
		}
	}

	// Delete
	for (i = 0; i < count; i++) {
		for (j = BOARD_BUFFER; j < BOARD_BUFFER+BOARD_WIDTH; j++) {
			g_board[i][j] = NONE;
		}
	}

}

// CheckAndDeleteBlocksから呼び出されるヘルパー関数
BOOL IsDeleteAvailable(int line)
{
	int i;
	for (i = BOARD_BUFFER; i < BOARD_BUFFER+BOARD_WIDTH; i++) {
		if (g_board[line][i] == NONE) {
			return FALSE;
		}
	}
	return TRUE;
}
