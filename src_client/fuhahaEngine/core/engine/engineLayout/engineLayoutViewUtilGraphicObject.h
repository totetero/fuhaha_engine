#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 表示要素構造体描画情報
struct engineLayoutViewUtilGraphicObject{
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
void engineLayoutViewUtilGraphicObjectInit(struct engineLayoutView *this);
// 表示要素構造体描画情報 破棄
void engineLayoutViewUtilGraphicObjectDispose(struct engineLayoutView *this);

// 表示要素構造体描画情報 関連づける
void engineLayoutViewUtilGraphicObjectConnect(struct engineLayoutView *this, struct engineLayoutView *view);
// 表示要素構造体描画情報 関連を外す
void engineLayoutViewUtilGraphicObjectDisconnect(struct engineLayoutView *this);

// 表示要素構造体描画情報 バッファ作成
void engineLayoutViewUtilGraphicObjectBufferCreate(struct engineLayoutView *this);
void engineLayoutViewUtilGraphicObjectBufferCreateAll(struct engineLayoutView *this);
void engineLayoutViewUtilGraphicObjectBufferActiveAll(struct engineLayoutView *this);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

