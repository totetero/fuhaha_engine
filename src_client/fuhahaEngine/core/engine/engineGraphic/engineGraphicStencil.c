#include "../../library.h"
#include "../../engine/engineGraphic/engineGraphic.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

static struct{
	// 重複動作阻止のための状態記録
	struct{
		enum engineGraphicStencilMode modeStencil;
	} memory;
	int stackMaskLevel;
} localGlobal = {0};

// ステンシル初期化
void engineGraphicStencilInit(void){
	localGlobal.memory.modeStencil = ENGINEGRAPHICSTENCILMODE_UNKNOWN;
	glClearStencil(0);
}

// ステンシルバッファのクリア
void engineGraphicStencilClear(void){
	glClear(GL_STENCIL_BUFFER_BIT);
	localGlobal.stackMaskLevel = 0;
}

// ----------------------------------------------------------------

// ステンシルマスクモード設定
void engineGraphicStencilSetMode(enum engineGraphicStencilMode mode){
	if(localGlobal.memory.modeStencil == mode){return;}
	localGlobal.memory.modeStencil = mode;

	// ステンシル有効設定
	switch(mode){
		case ENGINEGRAPHICSTENCILMODE_NONE:
			glDisable(GL_STENCIL_TEST);
			break;
		default:
			glEnable(GL_STENCIL_TEST);
			break;
	}

	// ステンシル以外の描画制限
	switch(mode){
		case ENGINEGRAPHICSTENCILMODE_MASK_0:
		case ENGINEGRAPHICSTENCILMODE_MASK_1:
		case ENGINEGRAPHICSTENCILMODE_MASK_2:
		case ENGINEGRAPHICSTENCILMODE_READ_EQ1_MASK_0:
		case ENGINEGRAPHICSTENCILMODE_READ_EQ1_MASK_2:
		case ENGINEGRAPHICSTENCILMODE_READ_GE1_MASK_0:
		case ENGINEGRAPHICSTENCILMODE_READ_GE1_MASK_INCR:
		case ENGINEGRAPHICSTENCILMODE_READ_LE1_MASK_0:
		case ENGINEGRAPHICSTENCILMODE_READ_LE1_MASK_INCR:
			// ステンシル以外の描画制限設定
			glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
			engineGraphicEngineIgnoreDepthMask(true);
			break;
		default:
			// ステンシル以外の描画制限解除
			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
			engineGraphicEngineIgnoreDepthMask(false);
			break;
	}

	// ステンシル条件設定
	switch(mode){
		case ENGINEGRAPHICSTENCILMODE_NONE: break;
		case ENGINEGRAPHICSTENCILMODE_UNKNOWN: break;
		case ENGINEGRAPHICSTENCILMODE_MASK_0:              glStencilFunc(GL_ALWAYS, 0, ~0); break;
		case ENGINEGRAPHICSTENCILMODE_MASK_1:              glStencilFunc(GL_ALWAYS, 1, ~0); break;
		case ENGINEGRAPHICSTENCILMODE_MASK_2:              glStencilFunc(GL_ALWAYS, 2, ~0); break;
		case ENGINEGRAPHICSTENCILMODE_WRITE_0:             glStencilFunc(GL_ALWAYS, 0, ~0); break;
		case ENGINEGRAPHICSTENCILMODE_WRITE_1:             glStencilFunc(GL_ALWAYS, 1, ~0); break;
		case ENGINEGRAPHICSTENCILMODE_WRITE_2:             glStencilFunc(GL_ALWAYS, 2, ~0); break;
		case ENGINEGRAPHICSTENCILMODE_READ_EQ0:            glStencilFunc(GL_EQUAL,  0, ~0); break;
		case ENGINEGRAPHICSTENCILMODE_READ_EQ1:            glStencilFunc(GL_EQUAL,  1, ~0); break;
		case ENGINEGRAPHICSTENCILMODE_READ_EQ2:            glStencilFunc(GL_EQUAL,  2, ~0); break;
		case ENGINEGRAPHICSTENCILMODE_READ_EQ1_MASK_0:     glStencilFunc(GL_EQUAL,  1, ~0); break;
		case ENGINEGRAPHICSTENCILMODE_READ_EQ1_WRITE_0:    glStencilFunc(GL_EQUAL,  1, ~0); break;
		case ENGINEGRAPHICSTENCILMODE_READ_EQ1_MASK_2:     glStencilFunc(GL_EQUAL,  1, ~0); break;
		case ENGINEGRAPHICSTENCILMODE_READ_EQ1_WRITE_2:    glStencilFunc(GL_EQUAL,  1, ~0); break;
		case ENGINEGRAPHICSTENCILMODE_READ_GE1:            glStencilFunc(GL_LEQUAL, 1, ~0); break;
		case ENGINEGRAPHICSTENCILMODE_READ_GE1_MASK_0:     glStencilFunc(GL_LEQUAL, 1, ~0); break;
		case ENGINEGRAPHICSTENCILMODE_READ_GE1_WRITE_0:    glStencilFunc(GL_LEQUAL, 1, ~0); break;
		case ENGINEGRAPHICSTENCILMODE_READ_GE1_MASK_INCR:  glStencilFunc(GL_LEQUAL, 1, ~0); break;
		case ENGINEGRAPHICSTENCILMODE_READ_GE1_WRITE_INCR: glStencilFunc(GL_LEQUAL, 1, ~0); break;
		case ENGINEGRAPHICSTENCILMODE_READ_LE1:            glStencilFunc(GL_GEQUAL, 1, ~0); break;
		case ENGINEGRAPHICSTENCILMODE_READ_LE1_MASK_0:     glStencilFunc(GL_GEQUAL, 1, ~0); break;
		case ENGINEGRAPHICSTENCILMODE_READ_LE1_WRITE_0:    glStencilFunc(GL_GEQUAL, 1, ~0); break;
		case ENGINEGRAPHICSTENCILMODE_READ_LE1_MASK_INCR:  glStencilFunc(GL_GEQUAL, 1, ~0); break;
		case ENGINEGRAPHICSTENCILMODE_READ_LE1_WRITE_INCR: glStencilFunc(GL_GEQUAL, 1, ~0); break;
	}

	// ステンシル条件設定
	switch(mode){
		case ENGINEGRAPHICSTENCILMODE_NONE:
		case ENGINEGRAPHICSTENCILMODE_UNKNOWN:
			// 何もしない
			break;
		case ENGINEGRAPHICSTENCILMODE_MASK_0:
		case ENGINEGRAPHICSTENCILMODE_MASK_1:
		case ENGINEGRAPHICSTENCILMODE_MASK_2:
		case ENGINEGRAPHICSTENCILMODE_WRITE_0:
		case ENGINEGRAPHICSTENCILMODE_WRITE_1:
		case ENGINEGRAPHICSTENCILMODE_WRITE_2:
			// マスクの書き込み
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
			break;
		case ENGINEGRAPHICSTENCILMODE_READ_EQ0:
		case ENGINEGRAPHICSTENCILMODE_READ_EQ1:
		case ENGINEGRAPHICSTENCILMODE_READ_EQ2:
		case ENGINEGRAPHICSTENCILMODE_READ_GE1:
		case ENGINEGRAPHICSTENCILMODE_READ_LE1:
			// マスク使用
			glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
			break;
		case ENGINEGRAPHICSTENCILMODE_READ_EQ1_MASK_0:
		case ENGINEGRAPHICSTENCILMODE_READ_EQ1_WRITE_0:
		case ENGINEGRAPHICSTENCILMODE_READ_GE1_MASK_0:
		case ENGINEGRAPHICSTENCILMODE_READ_GE1_WRITE_0:
		case ENGINEGRAPHICSTENCILMODE_READ_LE1_MASK_0:
		case ENGINEGRAPHICSTENCILMODE_READ_LE1_WRITE_0:
			// マスク書き換え 0にする
			glStencilOp(GL_KEEP, GL_KEEP, GL_ZERO);
			break;
		case ENGINEGRAPHICSTENCILMODE_READ_EQ1_MASK_2:
		case ENGINEGRAPHICSTENCILMODE_READ_EQ1_WRITE_2:
		case ENGINEGRAPHICSTENCILMODE_READ_GE1_MASK_INCR:
		case ENGINEGRAPHICSTENCILMODE_READ_GE1_WRITE_INCR:
		case ENGINEGRAPHICSTENCILMODE_READ_LE1_MASK_INCR:
		case ENGINEGRAPHICSTENCILMODE_READ_LE1_WRITE_INCR:
			// マスク書き換え 1増加
			glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
			break;
	}
}

// ----------------------------------------------------------------

// ステンシルスタックマスク設定
void engineGraphicStencilStackMaskRead(){
	localGlobal.memory.modeStencil = ENGINEGRAPHICSTENCILMODE_UNKNOWN;

	// ステンシル有効設定
	glEnable(GL_STENCIL_TEST);
	// ステンシル以外の描画制限解除
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	engineGraphicEngineIgnoreDepthMask(false);
	// ステンシル条件設定
	glStencilFunc(GL_LEQUAL, localGlobal.stackMaskLevel, ~0);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
}

// ステンシルスタックマスク加算描画
void engineGraphicStencilStackMaskWriteIncrement(){
	localGlobal.memory.modeStencil = ENGINEGRAPHICSTENCILMODE_UNKNOWN;

	// ステンシル有効設定
	glEnable(GL_STENCIL_TEST);
	// ステンシル以外の描画制限設定
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	engineGraphicEngineIgnoreDepthMask(true);
	// ステンシル条件設定
	glStencilFunc(GL_ALWAYS, 0, ~0);
	glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);

	// 加算
	localGlobal.stackMaskLevel++;
}

// ステンシルスタックマスク減算描画
void engineGraphicStencilStackMaskWriteDecrement(){
	localGlobal.memory.modeStencil = ENGINEGRAPHICSTENCILMODE_UNKNOWN;

	// ステンシル有効設定
	glEnable(GL_STENCIL_TEST);
	// ステンシル以外の描画制限設定
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	engineGraphicEngineIgnoreDepthMask(true);
	// ステンシル条件設定
	glStencilFunc(GL_ALWAYS, 0, ~0);
	glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);

	// 減算
	localGlobal.stackMaskLevel--;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

