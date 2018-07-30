#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ステンシルマスクモード列挙型
enum engineGraphicStencilMode{
	ENGINEGRAPHICSTENCILMODE_NONE,
	ENGINEGRAPHICSTENCILMODE_UNKNOWN,
	ENGINEGRAPHICSTENCILMODE_MASK_0,
	ENGINEGRAPHICSTENCILMODE_MASK_1,
	ENGINEGRAPHICSTENCILMODE_MASK_2,
	ENGINEGRAPHICSTENCILMODE_WRITE_0,
	ENGINEGRAPHICSTENCILMODE_WRITE_1,
	ENGINEGRAPHICSTENCILMODE_WRITE_2,
	ENGINEGRAPHICSTENCILMODE_READ_EQ0,
	ENGINEGRAPHICSTENCILMODE_READ_EQ1,
	ENGINEGRAPHICSTENCILMODE_READ_EQ2,
	ENGINEGRAPHICSTENCILMODE_READ_EQ1_MASK_0,
	ENGINEGRAPHICSTENCILMODE_READ_EQ1_WRITE_0,
	ENGINEGRAPHICSTENCILMODE_READ_EQ1_MASK_2,
	ENGINEGRAPHICSTENCILMODE_READ_EQ1_WRITE_2,
	ENGINEGRAPHICSTENCILMODE_READ_GE1,
	ENGINEGRAPHICSTENCILMODE_READ_GE1_MASK_0,
	ENGINEGRAPHICSTENCILMODE_READ_GE1_WRITE_0,
	ENGINEGRAPHICSTENCILMODE_READ_GE1_MASK_INCR,
	ENGINEGRAPHICSTENCILMODE_READ_GE1_WRITE_INCR,
	ENGINEGRAPHICSTENCILMODE_READ_LE1,
	ENGINEGRAPHICSTENCILMODE_READ_LE1_MASK_0,
	ENGINEGRAPHICSTENCILMODE_READ_LE1_WRITE_0,
	ENGINEGRAPHICSTENCILMODE_READ_LE1_MASK_INCR,
	ENGINEGRAPHICSTENCILMODE_READ_LE1_WRITE_INCR,
};

// ステンシル初期化
void engineGraphicStencilInit(void);

// ステンシルバッファのクリア
void engineGraphicStencilClear(void);

// ステンシルマスクモード設定
void engineGraphicStencilSetMode(enum engineGraphicStencilMode mode);

// ステンシルスタックマスク設定
void engineGraphicStencilStackMaskRead(void);
// ステンシルスタックマスク加算描画
void engineGraphicStencilStackMaskWriteIncrement(void);
// ステンシルスタックマスク減算描画
void engineGraphicStencilStackMaskWriteDecrement(void);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

