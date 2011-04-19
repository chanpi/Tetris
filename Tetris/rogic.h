#ifndef _TETORIS_ROGIC_
#define _TETORIS_ROGIC_

#define BOARD_WIDTH		8
#define BOARD_HEIGHT	10
#define BOARD_BUFFER	3

#define BOARD_OFFSET_X	16
#define BOARD_OFFSET_Y	96

#define BLOCK_WIDTH		4
#define BLOCK_HEIGHT	4

typedef enum {
	WALL = -1, NONE = 0, FIX = 1, MOVING = 2,
} COMPONENTS;

typedef enum {
	BLOCK_I, BLOCK_L, BLOCK_J, BLOCK_T, BLOCK_O, BLOCK_Z, BLOCK_S,
} BLOCK_TYPE;

typedef enum {
	BLOCK_BIRTH, BLOCK_FALL, BLOCK_RESTRICT
} BLOCK_STATE;

typedef struct {
	int x;
	int y;
	int block[BLOCK_HEIGHT][BLOCK_WIDTH];
	BLOCK_STATE state;
} Block;

#ifdef __cplusplus
extern "C" {
#endif
	
	void PrintBoard();
	void PrintBlock(Block* pBlock);

	void GameInitialize(Block* pBlock);
	BOOL IsGameOver();

	void BirthBlock(Block* pBlock);
	void FallBlock(Block* pBlock);
	void RestrictBlock(Block* pBlock);

	BOOL MoveLeft(Block* pBlock);
	BOOL MoveRight(Block* pBlock);
	BOOL MoveDown(Block* pBlock);
	BOOL Rotate(Block* pBlock);

#ifdef __cplusplus
}
#endif

#endif /* _TETORIS_ROGIC_ */