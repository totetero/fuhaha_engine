#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// テクスチャタイプ列挙型
enum engineGraphicTextureType{
	ENGINEGRAPHICTEXTURETYPE_LINEAR,
	ENGINEGRAPHICTEXTURETYPE_NEAREST,
};


// 3Dオブジェクト識別子
typedef int engineGraphicObjectVBOId;
typedef int engineGraphicObjectIBOId;
typedef int engineGraphicTextureId;

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

#include "./engineGraphicBuffer.h"
#include "./engineGraphicEngine.h"
#include "./engineGraphicMain.h"
#include "./engineGraphicObject.h"
#include "./engineGraphicShaderColorBlend.h"
#include "./engineGraphicShaderPhong.h"
#include "./engineGraphicShaderTexture.h"
#include "./engineGraphicShaderTextureAlphaMask.h"
#include "./engineGraphicShaderTextureColorBlendAlphaMask.h"
#include "./engineGraphicShaderTextureFilterColor.h"
#include "./engineGraphicStencil.h"
#include "./engineGraphicTexture.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

