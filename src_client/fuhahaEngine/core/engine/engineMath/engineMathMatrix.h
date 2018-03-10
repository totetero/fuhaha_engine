#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 行列複製
void engineMathMat4Copy(struct engineMathMatrix44 *dst, struct engineMathMatrix44 *src);
// 単位行列
void engineMathMat4Identity(struct engineMathMatrix44 *mat);
// 正射影行列
void engineMathMat4Ortho(struct engineMathMatrix44 *mat, double left, double right, double bottom, double top, double near, double far);
// 透視射影行列
void engineMathMat4Frustum(struct engineMathMatrix44 *mat, double left, double right, double bottom, double top, double near, double far);
void engineMathMat4Perspective(struct engineMathMatrix44 *mat, double fovy, double aspect, double near, double far);
// カメラ行列
void engineMathMat4LookAt(struct engineMathMatrix44 *mat, double eyeX, double eyeY, double eyeZ, double centerX, double centerY, double centerZ, double upX, double upY, double upZ);
// 行列の掛け合わせ
void engineMathMat4Multiply(struct engineMathMatrix44 *mat, struct engineMathMatrix44 *m0, struct engineMathMatrix44 *m1);
// 平行移動
void engineMathMat4Translate(struct engineMathMatrix44 *mat, double x, double y, double z);
// 拡大縮小
void engineMathMat4Scale(struct engineMathMatrix44 *mat, double x, double y, double z);
// x軸回転
void engineMathMat4RotateX(struct engineMathMatrix44 *mat, double rad);
// y軸回転
void engineMathMat4RotateY(struct engineMathMatrix44 *mat, double rad);
// z軸回転
void engineMathMat4RotateZ(struct engineMathMatrix44 *mat, double rad);
// 逆行列
void engineMathMat4Invert(struct engineMathMatrix44 *mat);

// 行列複製
void engineMathMat3Copy(struct engineMathMatrix33 *dst, struct engineMathMatrix33 *src);
void engineMathMat3Copy4(struct engineMathMatrix33 *dst, struct engineMathMatrix44 *src);
// 単位行列
void engineMathMat3Identity(struct engineMathMatrix33 *mat);
// 行列の掛け合わせ
void engineMathMat3Multiply(struct engineMathMatrix33 *mat, struct engineMathMatrix33 *m0, struct engineMathMatrix33 *m1);
// 逆行列
void engineMathMat3Invert(struct engineMathMatrix33 *mat);
// 転置行列
void engineMathMat3Transpose(struct engineMathMatrix33 *mat);

// ベクトル値設定
void engineMathVec3Set(struct engineMathVector3 *vec, double x, double y, double z);
void engineMathVec4Set(struct engineMathVector4 *vec, double x, double y, double z, double w);
// ベクトル行列積
void engineMathVec3MultiplyMat4(struct engineMathVector3 *dst, struct engineMathVector3 *src, struct engineMathMatrix44 *mat);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

