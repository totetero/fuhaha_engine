#include "../../library.h"
#include "../../engine/engineMath/engineMath.h"
#include "../../engine/engineCtrl/engineCtrl.h"
#include "../../engine/engineGraphic/engineGraphic.h"
#include "../../engine/engineLayout01/engineLayout01.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ボタン構造体 初期化
void engineLayout01ButtonInit(struct engineLayout01Button *this){
	this->x = 0;
	this->y = 0;
	this->w = 0;
	this->h = 0;
	this->active = false;
	this->inactive = false;
	this->trigger = false;
	this->child = NULL;
	this->zKey = false;
	this->xKey = false;
	this->cKey = false;
	this->vKey = false;
	this->inner = true;
	this->isMatrix = false;
	this->hitVertexLength = 0;
}

// ボタン構造体 静的位置設定
void engineLayout01ButtonSetPosition(struct engineLayout01Button *this, double x, double y, double w, double h){
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;

	// 直方体のタッチ範囲
	this->hitVertexLength = 4;
	this->hitVertex0[0].x = x;
	this->hitVertex0[0].y = y;
	this->hitVertex0[0].z = 0;
	this->hitVertex0[1].x = x + w;
	this->hitVertex0[1].y = y;
	this->hitVertex0[1].z = 0;
	this->hitVertex0[2].x = x + w;
	this->hitVertex0[2].y = y + h;
	this->hitVertex0[2].z = 0;
	this->hitVertex0[3].x = x;
	this->hitVertex0[3].y = y + h;
	this->hitVertex0[3].z = 0;
}

// ボタン構造体 領域内確認行列読み込み
void engineLayout01ButtonSetMatrix(struct engineLayout01Button *this, struct engineMathMatrix44 *mat){
	this->isMatrix = true;
	for(int i = 0; i < this->hitVertexLength; i++){
		engineMathVec3MultiplyMat4(&this->hitVertex1[i], &this->hitVertex0[i], mat);
		this->hitVertex1[i].x = (this->hitVertex1[i].x + 1) * 0.5 * global.window.w;
		this->hitVertex1[i].y = (1 - this->hitVertex1[i].y) * 0.5 * global.window.h;
	}
}

// ボタン構造体 計算
void engineLayout01ButtonCalc(struct engineLayout01Button *this){
	struct engineCtrlTouch *t = engineCtrlTouchGet(ENGINECTRLTOUCHID_BUTTON);
	engineLayout01ButtonSubCalc(this, t, true);
	if(t != NULL){
		if(this->active && !t->active){
			engineCtrlTouchOwn();
		}else if(!t->dn){
			engineCtrlTouchFree();
		}
	}
}

// ボタン構造体 タッチ情報を使い回す計算
void engineLayout01ButtonSubCalc(struct engineLayout01Button *this, struct engineCtrlTouch *t, bool clickable){
	if(this->inactive || !clickable){
		// ボタン無効状態
		this->active = false;
	}else if((this->zKey && global.key.zb.isActive) || (this->xKey && global.key.xb.isActive) || (this->cKey && global.key.cb.isActive) || (this->vKey && global.key.vb.isActive)){
		// 対応キー押下中
		this->active = true;
	}else if(t == NULL){
		// タッチ無し
		this->active = false;
	}else if(t->dn){
		// ボタン押下中
		this->active = (engineLayout01ButtonIsInner(this, t) == !!this->inner);
		// 子要素の範囲内では押下状態にならない
		if(this->child != NULL){this->active = this->active && !(engineLayout01ButtonIsInner(this->child, t) == !!this->child->inner);}
	}else if(this->active){
		// ボタンを放した瞬間
		this->active = false;
		this->trigger = true;
	}
}

// ボタン構造体 タッチ領域内確認
bool engineLayout01ButtonIsInner(struct engineLayout01Button *this, struct engineCtrlTouch *t){
	if(this->isMatrix){
		// すべての辺に対し対象点との外積が全て同一符号なら内部
		double crossz[sizeof(this->hitVertex1) / sizeof(*this->hitVertex1)];
		for(int i = 0; i < this->hitVertexLength; i++){
			struct engineMathVector3 *v0 = &this->hitVertex1[i];
			struct engineMathVector3 *v1 = &this->hitVertex1[(i + 1) % this->hitVertexLength];
			crossz[i] = (t->window.x - v0->x) * (v1->y - v0->y) - (v1->x - v0->x) * (t->window.y - v0->y);
		}
		// 符号がすべて等しいことを確認
		for(int i = 0; i < this->hitVertexLength - 1; i++){if(crossz[i] * crossz[i + 1] <= 0){return false;}}
		return (this->hitVertexLength > 0);
	}else{
		double x0 = this->x;
		double y0 = this->y;
		double x1 = x0 + this->w;
		double y1 = y0 + this->h;
		return (x0 < t->screen.x && t->screen.x < x1 && y0 < t->screen.y && t->screen.y < y1);
	}
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

