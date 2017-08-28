#include "../../library.h"
#include "engineGraphic.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

static struct{
	// 重複動作阻止のための状態記録
	struct{
		enum engineGraphicStencilMode modeStencil;
	} memory;
} localGlobal = {0};

// ステンシル初期化
void engineGraphicStencilInit(void){
	localGlobal.memory.modeStencil = -1;
	glClearStencil(0);
}

// ステンシルバッファのクリア
void engineGraphicStencilClear(void){
	glClear(GL_STENCIL_BUFFER_BIT);
}

// ステンシルマスクモード設定
void engineGraphicStencilSetMode(enum engineGraphicStencilMode mode){
	if(localGlobal.memory.modeStencil == mode){return;}
	localGlobal.memory.modeStencil = mode;

	// ステンシル有効設定
	if(mode != ENGINEGRAPHICSTENCILMODE_NONE){glEnable(GL_STENCIL_TEST);}else{glDisable(GL_STENCIL_TEST);}

	// ステンシル以外の描画制限設定
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
			glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
			engineGraphicEngineIgnoreDepthMask(true);
			break;
		default:
			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
			engineGraphicEngineIgnoreDepthMask(false);
			break;
	}

	// ステンシル条件設定
	switch(mode){
		case ENGINEGRAPHICSTENCILMODE_NONE: break;
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
		default:
			break;
	}
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

