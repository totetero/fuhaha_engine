#include "../../library.h"
#include "../engineMath/engineMath.h"
#include "../engineCtrl/engineCtrl.h"
#include "../engineGraphic/engineGraphic.h"
#include "engineLayout01.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 変形構造体 初期化
void engineLayout01TransInit(struct engineLayout01Trans *this){
	this->texture = NULL;
	this->vertVBO = NULL;
	this->clorVBO = NULL;
	this->texcVBO = NULL;
	this->faceIBO = NULL;
	this->parent = NULL;
	engineMathVec4Set(&this->color, 1.0, 1.0, 1.0, 1.0);
	this->isScale = false;
	this->isRotate = false;
	this->isTranslate = false;
	this->isExist = false;
	this->isVisible = true;
	this->zIndex = 0;
	this->draw = NULL;
}

// ----------------------------------------------------------------

// 変形構造体 拡大縮小
void engineLayout01TransSetScale(struct engineLayout01Trans *this, double x, double y, double z){
	bool isScalex = !(engineMathAbs(x - 1.0) < DBL_EPSILON);
	bool isScaley = !(engineMathAbs(y - 1.0) < DBL_EPSILON);
	bool isScalez = !(engineMathAbs(z - 1.0) < DBL_EPSILON);
	this->isScale = (isScalex || isScaley || isScalez);
	if(this->isScale){
		this->scale.x = x;
		this->scale.y = y;
		this->scale.z = z;
	}
}

// 変形構造体 回転
void engineLayout01TransSetRotate(struct engineLayout01Trans *this, double radx, double rady, double radz){
	bool isRotx = !(engineMathAbs(radx - 0.0) < DBL_EPSILON);
	bool isRoty = !(engineMathAbs(rady - 0.0) < DBL_EPSILON);
	bool isRotz = !(engineMathAbs(radz - 0.0) < DBL_EPSILON);
	this->isRotate = (isRotx || isRoty || isRotz);
	if(this->isRotate){
		engineMathMat4Identity(&this->rotate);
		if(isRotx){engineMathMat4RotateY(&this->rotate, rady);}
		if(isRoty){engineMathMat4RotateX(&this->rotate, radx);}
		if(isRotz){engineMathMat4RotateZ(&this->rotate, radz);}
	}
}

// 変形構造体 平行移動
void engineLayout01TransSetTranslate(struct engineLayout01Trans *this, double x, double y, double z){
	bool isTranslatex = !(engineMathAbs(x - 0.0) < DBL_EPSILON);
	bool isTranslatey = !(engineMathAbs(y - 0.0) < DBL_EPSILON);
	bool isTranslatez = !(engineMathAbs(z - 0.0) < DBL_EPSILON);
	this->isTranslate = (isTranslatex || isTranslatey || isTranslatez);
	if(this->isTranslate){
		this->translate.x = x;
		this->translate.y = y;
		this->translate.z = z;
	}
}


// ----------------------------------------------------------------

// 変形構造体 引数matに対して行列を掛け合わせる
void engineLayout01TransMultMatrix(struct engineLayout01Trans *this, struct engineMathMatrix44 *mat){
	if(this->parent != NULL){engineLayout01TransMultMatrix(this->parent, mat);}

	// 行列合成
	if(this->isTranslate){engineMathMat4Translate(mat, this->translate.x, this->translate.y, this->translate.z);}
	if(this->isRotate){engineMathMat4Multiply(mat, mat, &this->rotate);}
	if(this->isScale){engineMathMat4Scale(mat, this->scale.x, this->scale.y, this->scale.z);}
}

// ----------------------------------------------------------------

// 変形構造体 バッファ登録
void engineLayout01TransBindBuffer(struct engineLayout01Trans *this){
	struct engineLayout01Trans *trans;
	trans = this; while(trans != NULL){if(trans->texture != NULL){engineGraphicEngineBindTexture(*trans->texture); break;} trans = trans->parent;}
	trans = this; while(trans != NULL){if(trans->vertVBO != NULL){engineGraphicEngineBindVertVBO(*trans->vertVBO); break;} trans = trans->parent;}
	trans = this; while(trans != NULL){if(trans->clorVBO != NULL){engineGraphicEngineBindClorVBO(*trans->clorVBO); break;} trans = trans->parent;}
	trans = this; while(trans != NULL){if(trans->texcVBO != NULL){engineGraphicEngineBindTexcVBO(*trans->texcVBO); break;} trans = trans->parent;}
	trans = this; while(trans != NULL){if(trans->faceIBO != NULL){engineGraphicEngineBindFaceIBO(*trans->faceIBO); break;} trans = trans->parent;}
}

// 変形構造体 色情報登録
void engineLayout01TransBindColor(struct engineLayout01Trans *this, struct engineMathVector4 *color){
	struct engineMathVector4 totalColor;
	memcpy(&totalColor, color, sizeof(struct engineMathVector4));
	struct engineLayout01Trans *trans = this;
	while(trans != NULL){
		totalColor.r *= trans->color.r;
		totalColor.g *= trans->color.g;
		totalColor.b *= trans->color.b;
		totalColor.a *= trans->color.a;
		trans = trans->parent;
	}
	engineGraphicEngineSetColorVec(&totalColor);
}

// 変形構造体 いろいろ登録
void engineLayout01TransBindAll(struct engineLayout01Trans *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
	// バッファ登録
	engineLayout01TransBindBuffer(this);
	// 行列登録
	struct engineMathMatrix44 tempMat1;
	engineMathMat4Copy(&tempMat1, mat);
	engineLayout01TransMultMatrix(this, &tempMat1);
	engineGraphicEngineSetMatrix(&tempMat1);
	// 色登録
	engineLayout01TransBindColor(this, color);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

