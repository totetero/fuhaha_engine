#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 表示要素構造体描画情報
struct engineLayoutViewGearGraphicObject{
	struct engineLayoutView *prev;
	struct engineLayoutView *next;
	bool (*shouldBufferCreate)(struct engineLayoutView *this);
	void (*bufferCreate)(struct engineLayoutView *this);
	bool isBufferChecked;
	engineGraphicObjectVBOId egoIdVert;
	engineGraphicObjectVBOId egoIdTexc;
	engineGraphicObjectIBOId egoIdFace;
};

// 表示要素構造体描画情報 初期化
void engineLayoutViewGearGraphicObjectInit(struct engineLayoutView *this);
// 表示要素構造体描画情報 破棄
void engineLayoutViewGearGraphicObjectDispose(struct engineLayoutView *this);

// 表示要素構造体描画情報 関連づける
void engineLayoutViewGearGraphicObjectConnect(struct engineLayoutView *this, struct engineLayoutView *view);
// 表示要素構造体描画情報 関連を外す
void engineLayoutViewGearGraphicObjectDisconnect(struct engineLayoutView *this);

// 表示要素構造体描画情報 バッファ作成
void engineLayoutViewGearGraphicObjectBufferCreate(struct engineLayoutView *this);
void engineLayoutViewGearGraphicObjectBufferCreateAll(struct engineLayoutView *this);
void engineLayoutViewGearGraphicObjectBufferActiveAll(struct engineLayoutView *this);

// 表示要素構造体描画情報 デフォルト関数
bool engineLayoutViewGearGraphicObjectDefaultShouldBufferCreate(struct engineLayoutView *this);
void engineLayoutViewGearGraphicObjectDefaultBufferCreate(struct engineLayoutView *this);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

