#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

void e3dGameInit();
void e3dGameFinal();

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// テクスチャタイプ列挙型
enum e3dTexType{
	E3DTEXTYPE_LINEAR,
	E3DTEXTYPE_NEAREST
};

// e3d描画モード列挙型
enum e3dModeDraw{
	E3DMODEDRAW_NORMAL,
	E3DMODEDRAW_2D,
	E3DMODEDRAW_ALPHA_ADD,
	E3DMODEDRAW_HKNW,
	E3DMODEDRAW_SPHERE
};

// ステンシルマスクモード列挙型
enum e3dModeStencil{
	E3DMODESTENCIL_NONE,
	E3DMODESTENCIL_MASK_0,
	E3DMODESTENCIL_MASK_1,
	E3DMODESTENCIL_MASK_2,
	E3DMODESTENCIL_WRITE_0,
	E3DMODESTENCIL_WRITE_1,
	E3DMODESTENCIL_WRITE_2,
	E3DMODESTENCIL_READ_EQ0,
	E3DMODESTENCIL_READ_EQ1,
	E3DMODESTENCIL_READ_EQ2,
	E3DMODESTENCIL_READ_EQ1_MASK_0,
	E3DMODESTENCIL_READ_EQ1_WRITE_0,
	E3DMODESTENCIL_READ_EQ1_MASK_2,
	E3DMODESTENCIL_READ_EQ1_WRITE_2,
	E3DMODESTENCIL_READ_GE1,
	E3DMODESTENCIL_READ_GE1_MASK_0,
	E3DMODESTENCIL_READ_GE1_WRITE_0,
	E3DMODESTENCIL_READ_GE1_MASK_INCR,
	E3DMODESTENCIL_READ_GE1_WRITE_INCR,
	E3DMODESTENCIL_READ_LE1,
	E3DMODESTENCIL_READ_LE1_MASK_0,
	E3DMODESTENCIL_READ_LE1_WRITE_0,
	E3DMODESTENCIL_READ_LE1_MASK_INCR,
	E3DMODESTENCIL_READ_LE1_WRITE_INCR,
};

// ベクトル行列構造体
struct e3dVector3{union{struct{double x, y, z;}; double v[3];};};
struct e3dVector4{union{struct{double x, y, z, w;}; struct{double r, g, b, a;}; double v[4];};};
struct e3dMatrix44{union{struct{double m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33;}; double m[16];};};

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// 3Dオブジェクト

typedef uint32_t e3dVBOId;
typedef uint32_t e3dIBOId;
typedef uint32_t e3dTexId;

e3dVBOId e3dCreateVBO(uint32_t size, double *vertices);
e3dIBOId e3dCreateIBO(uint32_t size, uint32_t *indexes);
e3dTexId e3dCreateTex(char *src, enum e3dTexType type);

GLint e3dGetGLIdVBO(e3dVBOId e3dId);
GLint e3dGetGLIdIBO(e3dIBOId e3dId);
GLint e3dGetGLIdTex(e3dTexId e3dId);

void e3dDisposeVBO(e3dVBOId e3dId);
void e3dDisposeIBO(e3dIBOId e3dId);
void e3dDisposeTex(e3dTexId e3dId);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// 3Dエンジン

void e3dClearAll();
void e3dClearDepth();
void e3dClearStencil();

void e3dSetDrawMode(enum e3dModeDraw mode);
void e3dSetStencilMode(enum e3dModeStencil mode);
void e3dIgnoreDepthMode(bool isIgnore);

void e3dBindTexture(e3dTexId e3dId);
void e3dBindVertVBO(e3dVBOId e3dId);
void e3dBindClorVBO(e3dVBOId e3dId);
void e3dBindTexcVBO(e3dVBOId e3dId);
void e3dBindFaceIBO(e3dIBOId e3dId);

void e3dSetMatrix(struct e3dMatrix44 *matrix);
void e3dSetColor(double r, double g, double b, double a);

void e3dDrawIndex(uint32_t offset, uint32_t count);

void e3dFlush();

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// 行列演算

void mat4Copy(struct e3dMatrix44 *dst, struct e3dMatrix44 *src);
void mat4Identity(struct e3dMatrix44 *mat);
void mat4Ortho(struct e3dMatrix44 *mat, double left, double right, double bottom, double top, double near, double far);
void mat4Frustum(struct e3dMatrix44 *mat, double left, double right, double bottom, double top, double near, double far);
void mat4Multiply(struct e3dMatrix44 *mat, struct e3dMatrix44 *m0, struct e3dMatrix44 *m1);
void mat4Translate(struct e3dMatrix44 *mat, double x, double y, double z);
void mat4Scale(struct e3dMatrix44 *mat, double x, double y, double z);
void mat4RotateX(struct e3dMatrix44 *mat, double rad);
void mat4RotateY(struct e3dMatrix44 *mat, double rad);
void mat4RotateZ(struct e3dMatrix44 *mat, double rad);
void mat4Invert(struct e3dMatrix44 *mat);

void vec3MultiplyMat4(struct e3dVector3 *dst, struct e3dVector3 *src, struct e3dMatrix44 *mat);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// 3Dエンジンバッファ作成支援

// バッファ作成管理
void e3dBufferBegin();
void e3dBufferEnd(e3dVBOId *e3dIdVert, e3dVBOId *e3dIdClor, e3dVBOId *e3dIdTexc, e3dIBOId *e3dIdFace);

// バッファ配列に要素追加
void e3dBufferPushVert(double x, double y, double z);
void e3dBufferPushClor(double r, double g, double b);
void e3dBufferPushTexc(double u, double v);
void e3dBufferPushFace(uint32_t index, uint32_t t0, uint32_t t1, uint32_t t2);

// バッファ配列に四角形の要素追加
void e3dBufferPushTetraVert(double x, double y, double w, double h);
void e3dBufferPushTetraTexc(uint16_t imgw, uint16_t imgh, double u, double v, double w, double h);
void e3dBufferPushTetraFace(uint32_t index);

// バッファ配列内の位置獲得
uint32_t e3dBufferVretIndexGet();
uint32_t e3dBufferFaceIndexGet();

// バッファ片付け
void e3dBufferClean();

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

