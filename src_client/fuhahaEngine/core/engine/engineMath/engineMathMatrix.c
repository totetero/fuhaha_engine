#include "../../library.h"
#include "../../engine/engineMath/engineMath.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 行列複製
void engineMathMat4Copy(struct engineMathMatrix44 *dst, struct engineMathMatrix44 *src){
	memcpy(dst, src, sizeof(struct engineMathMatrix44));
}

// 行列複製
void engineMathMat4Copy3(struct engineMathMatrix44 *dst, struct engineMathMatrix33 *src){
	dst->m00 = src->m00; dst->m01 = src->m01; dst->m02 = src->m02; dst->m03 = 0.0;
	dst->m10 = src->m10; dst->m11 = src->m11; dst->m12 = src->m12; dst->m13 = 0.0;
	dst->m20 = src->m20; dst->m21 = src->m21; dst->m22 = src->m22; dst->m23 = 0.0;
	dst->m30 = 0.0; dst->m31 = 0.0; dst->m32 = 0.0; dst->m33 = 1.0;
}

// ----------------------------------------------------------------

// 単位行列
void engineMathMat4Identity(struct engineMathMatrix44 *mat){
	mat->m00 = 1.0; mat->m01 = 0.0; mat->m02 = 0.0; mat->m03 = 0.0;
	mat->m10 = 0.0; mat->m11 = 1.0; mat->m12 = 0.0; mat->m13 = 0.0;
	mat->m20 = 0.0; mat->m21 = 0.0; mat->m22 = 1.0; mat->m23 = 0.0;
	mat->m30 = 0.0; mat->m31 = 0.0; mat->m32 = 0.0; mat->m33 = 1.0;
}

// 正射影行列
void engineMathMat4Ortho(struct engineMathMatrix44 *mat, double left, double right, double bottom, double top, double near, double far){
	double rl = 1 / (right - left);
	double tb = 1 / (top - bottom);
	double nf = 1 / (near - far);
	mat->m00 = (GLfloat)(2 * rl);
	mat->m01 = 0;
	mat->m02 = 0;
	mat->m03 = 0;
	mat->m10 = 0;
	mat->m11 = (GLfloat)(2 * tb);
	mat->m12 = 0;
	mat->m13 = 0;
	mat->m20 = 0;
	mat->m21 = 0;
	mat->m22 = (GLfloat)(2 * nf);
	mat->m23 = 0;
	mat->m30 = (GLfloat)((right + left) * -rl);
	mat->m31 = (GLfloat)((top + bottom) * -tb);
	mat->m32 = (GLfloat)((far + near) * nf);
	mat->m33 = 1;
}

// 透視射影行列
void engineMathMat4Frustum(struct engineMathMatrix44 *mat, double left, double right, double bottom, double top, double near, double far){
	double rl = 1 / (right - left);
	double tb = 1 / (top - bottom);
	double nf = 1 / (near - far);
	mat->m00 = (GLfloat)((near * 2) * rl);
	mat->m01 = 0;
	mat->m02 = 0;
	mat->m03 = 0;
	mat->m10 = 0;
	mat->m11 = (GLfloat)((near * 2) * tb);
	mat->m12 = 0;
	mat->m13 = 0;
	mat->m20 = (GLfloat)((right + left) * rl);
	mat->m21 = (GLfloat)((top + bottom) * tb);
	mat->m22 = (GLfloat)((far + near) * nf);
	mat->m23 = -1;
	mat->m30 = 0;
	mat->m31 = 0;
	mat->m32 = (GLfloat)((far * near * 2) * nf);
	mat->m33 = 0;
}

// 透視射影行列
void engineMathMat4Perspective(struct engineMathMatrix44 *mat, double fovy, double aspect, double near, double far){
	double ratioX = (aspect > 1) ? (1.0 / aspect) : 1.0;
	double ratioY = (aspect > 1) ? 1.0 : (1.0 * aspect);
	double f = 1 / engineMathTan(fovy / 2);
	double nf = 1 / (near - far);
	mat->m00 = (GLfloat)(f * ratioX);
	mat->m01 = 0;
	mat->m02 = 0;
	mat->m03 = 0;
	mat->m10 = 0;
	mat->m11 = (GLfloat)(f * ratioY);
	mat->m12 = 0;
	mat->m13 = 0;
	mat->m20 = 0;
	mat->m21 = 0;
	mat->m22 = (GLfloat)((far + near) * nf);
	mat->m23 = -1;
	mat->m30 = 0;
	mat->m31 = 0;
	mat->m32 = (GLfloat)((far * near * 2) * nf);
	mat->m33 = 0;
}

// カメラ行列
void engineMathMat4LookAt(struct engineMathMatrix44 *mat, double eyeX, double eyeY, double eyeZ, double centerX, double centerY, double centerZ, double upX, double upY, double upZ){
	double zx = eyeX - centerX;
	double zy = eyeY - centerY;
	double zz = eyeZ - centerZ;
	if(engineMathAbs(zx) < DBL_EPSILON && engineMathAbs(zy) < DBL_EPSILON && engineMathAbs(zz) < DBL_EPSILON){engineMathMat4Identity(mat); return;}
	double zr = engineMathSqrt(zx * zx + zy * zy + zz * zz);
	double zri = 1 / zr;
	zx *= zri;
	zy *= zri;
	zz *= zri;
	double xx = upY * zz - upZ * zy;
	double xy = upZ * zx - upX * zz;
	double xz = upX * zy - upY * zx;
	double xr = engineMathSqrt(xx * xx + xy * xy + xz * xz);
	double xri = (engineMathAbs(xr) < DBL_EPSILON) ? 0 : (1 / xr);
	xx *= xri;
	xy *= xri;
	xz *= xri;
	double yx = zy * xz - zz * xy;
	double yy = zz * xx - zx * xz;
	double yz = zx * xy - zy * xx;
	double yr = engineMathSqrt(yx * yx + yy * yy + yz * yz);
	double yri = (engineMathAbs(yr) < DBL_EPSILON) ? 0 : (1 / yr);
	yx *= yri;
	yy *= yri;
	yz *= yri;
	mat->m00 = (GLfloat)xx;
	mat->m01 = (GLfloat)yx;
	mat->m02 = (GLfloat)zx;
	mat->m03 = 0;
	mat->m10 = (GLfloat)xy;
	mat->m11 = (GLfloat)yy;
	mat->m12 = (GLfloat)zy;
	mat->m13 = 0;
	mat->m20 = (GLfloat)xz;
	mat->m21 = (GLfloat)yz;
	mat->m22 = (GLfloat)zz;
	mat->m23 = 0;
	mat->m30 = -(GLfloat)(xx * eyeX + xy * eyeY + xz * eyeZ);
	mat->m31 = -(GLfloat)(yx * eyeX + yy * eyeY + yz * eyeZ);
	mat->m32 = -(GLfloat)(zx * eyeX + zy * eyeY + zz * eyeZ);
	mat->m33 = 1;
}

// ----------------------------------------------------------------

// 行列の掛け合わせ
void engineMathMat4Multiply(struct engineMathMatrix44 *mat, struct engineMathMatrix44 *m0, struct engineMathMatrix44 *m1){
	struct engineMathMatrix44 a;
	struct engineMathMatrix44 b;
	if(mat == m0){engineMathMat4Copy(&a, m0); m0 = &a;}
	if(mat == m1){engineMathMat4Copy(&b, m1); m1 = &b;}
	mat->m00 = m1->m00 * m0->m00 + m1->m01 * m0->m10 + m1->m02 * m0->m20 + m1->m03 * m0->m30;
	mat->m01 = m1->m00 * m0->m01 + m1->m01 * m0->m11 + m1->m02 * m0->m21 + m1->m03 * m0->m31;
	mat->m02 = m1->m00 * m0->m02 + m1->m01 * m0->m12 + m1->m02 * m0->m22 + m1->m03 * m0->m32;
	mat->m03 = m1->m00 * m0->m03 + m1->m01 * m0->m13 + m1->m02 * m0->m23 + m1->m03 * m0->m33;
	mat->m10 = m1->m10 * m0->m00 + m1->m11 * m0->m10 + m1->m12 * m0->m20 + m1->m13 * m0->m30;
	mat->m11 = m1->m10 * m0->m01 + m1->m11 * m0->m11 + m1->m12 * m0->m21 + m1->m13 * m0->m31;
	mat->m12 = m1->m10 * m0->m02 + m1->m11 * m0->m12 + m1->m12 * m0->m22 + m1->m13 * m0->m32;
	mat->m13 = m1->m10 * m0->m03 + m1->m11 * m0->m13 + m1->m12 * m0->m23 + m1->m13 * m0->m33;
	mat->m20 = m1->m20 * m0->m00 + m1->m21 * m0->m10 + m1->m22 * m0->m20 + m1->m23 * m0->m30;
	mat->m21 = m1->m20 * m0->m01 + m1->m21 * m0->m11 + m1->m22 * m0->m21 + m1->m23 * m0->m31;
	mat->m22 = m1->m20 * m0->m02 + m1->m21 * m0->m12 + m1->m22 * m0->m22 + m1->m23 * m0->m32;
	mat->m23 = m1->m20 * m0->m03 + m1->m21 * m0->m13 + m1->m22 * m0->m23 + m1->m23 * m0->m33;
	mat->m30 = m1->m30 * m0->m00 + m1->m31 * m0->m10 + m1->m32 * m0->m20 + m1->m33 * m0->m30;
	mat->m31 = m1->m30 * m0->m01 + m1->m31 * m0->m11 + m1->m32 * m0->m21 + m1->m33 * m0->m31;
	mat->m32 = m1->m30 * m0->m02 + m1->m31 * m0->m12 + m1->m32 * m0->m22 + m1->m33 * m0->m32;
	mat->m33 = m1->m30 * m0->m03 + m1->m31 * m0->m13 + m1->m32 * m0->m23 + m1->m33 * m0->m33;
}

// 平行移動
void engineMathMat4Translate(struct engineMathMatrix44 *mat, double x, double y, double z){
	mat->m30 += (GLfloat)(mat->m00 * x + mat->m10 * y + mat->m20 * z);
	mat->m31 += (GLfloat)(mat->m01 * x + mat->m11 * y + mat->m21 * z);
	mat->m32 += (GLfloat)(mat->m02 * x + mat->m12 * y + mat->m22 * z);
	mat->m33 += (GLfloat)(mat->m03 * x + mat->m13 * y + mat->m23 * z);
}

// 拡大縮小
void engineMathMat4Scale(struct engineMathMatrix44 *mat, double x, double y, double z){
	mat->m00 *= (GLfloat)x;
	mat->m01 *= (GLfloat)x;
	mat->m02 *= (GLfloat)x;
	mat->m03 *= (GLfloat)x;
	mat->m10 *= (GLfloat)y;
	mat->m11 *= (GLfloat)y;
	mat->m12 *= (GLfloat)y;
	mat->m13 *= (GLfloat)y;
	mat->m20 *= (GLfloat)z;
	mat->m21 *= (GLfloat)z;
	mat->m22 *= (GLfloat)z;
	mat->m23 *= (GLfloat)z;
}

// x軸回転
void engineMathMat4RotateX(struct engineMathMatrix44 *mat, double rad){
	double c = engineMathCos(rad);
	double s = engineMathSin(rad);
	double m10 = mat->m10;
	double m11 = mat->m11;
	double m12 = mat->m12;
	double m13 = mat->m13;
	double m20 = mat->m20;
	double m21 = mat->m21;
	double m22 = mat->m22;
	double m23 = mat->m23;
	mat->m10 = (GLfloat)(m10 * c + m20 * s);
	mat->m11 = (GLfloat)(m11 * c + m21 * s);
	mat->m12 = (GLfloat)(m12 * c + m22 * s);
	mat->m13 = (GLfloat)(m13 * c + m23 * s);
	mat->m20 = (GLfloat)(m20 * c - m10 * s);
	mat->m21 = (GLfloat)(m21 * c - m11 * s);
	mat->m22 = (GLfloat)(m22 * c - m12 * s);
	mat->m23 = (GLfloat)(m23 * c - m13 * s);
}

// y軸回転
void engineMathMat4RotateY(struct engineMathMatrix44 *mat, double rad){
	double c = engineMathCos(rad);
	double s = engineMathSin(rad);
	double m00 = mat->m00;
	double m01 = mat->m01;
	double m02 = mat->m02;
	double m03 = mat->m03;
	double m20 = mat->m20;
	double m21 = mat->m21;
	double m22 = mat->m22;
	double m23 = mat->m23;
	mat->m00 = (GLfloat)(m00 * c - m20 * s);
	mat->m01 = (GLfloat)(m01 * c - m21 * s);
	mat->m02 = (GLfloat)(m02 * c - m22 * s);
	mat->m03 = (GLfloat)(m03 * c - m23 * s);
	mat->m20 = (GLfloat)(m20 * c + m00 * s);
	mat->m21 = (GLfloat)(m21 * c + m01 * s);
	mat->m22 = (GLfloat)(m22 * c + m02 * s);
	mat->m23 = (GLfloat)(m23 * c + m03 * s);
}

// z軸回転
void engineMathMat4RotateZ(struct engineMathMatrix44 *mat, double rad){
	double c = engineMathCos(rad);
	double s = engineMathSin(rad);
	double m00 = mat->m00;
	double m01 = mat->m01;
	double m02 = mat->m02;
	double m03 = mat->m03;
	double m10 = mat->m10;
	double m11 = mat->m11;
	double m12 = mat->m12;
	double m13 = mat->m13;
	mat->m00 = (GLfloat)(m00 * c + m10 * s);
	mat->m01 = (GLfloat)(m01 * c + m11 * s);
	mat->m02 = (GLfloat)(m02 * c + m12 * s);
	mat->m03 = (GLfloat)(m03 * c + m13 * s);
	mat->m10 = (GLfloat)(m10 * c - m00 * s);
	mat->m11 = (GLfloat)(m11 * c - m01 * s);
	mat->m12 = (GLfloat)(m12 * c - m02 * s);
	mat->m13 = (GLfloat)(m13 * c - m03 * s);
}

// クォータニオン回転
void engineMathMat4RotateQuat(struct engineMathMatrix44 *mat, struct engineMathQuaternion *quat){
	struct engineMathMatrix44 temp;
	double xx = 2 * quat->x * quat->x;
	double xy = 2 * quat->x * quat->y;
	double xz = 2 * quat->x * quat->z;
	double xw = 2 * quat->x * quat->w;
	double yy = 2 * quat->y * quat->y;
	double yz = 2 * quat->y * quat->z;
	double yw = 2 * quat->y * quat->w;
	double zz = 2 * quat->z * quat->z;
	double zw = 2 * quat->z * quat->w;
	temp.m00 = (GLfloat)(1 - yy - zz);
	temp.m01 = (GLfloat)(xy + zw);
	temp.m02 = (GLfloat)(xz - yw);
	temp.m03 = 0;
	temp.m10 = (GLfloat)(xy - zw);
	temp.m11 = (GLfloat)(1 - xx - zz);
	temp.m12 = (GLfloat)(yz + xw);
	temp.m13 = 0;
	temp.m20 = (GLfloat)(xz + yw);
	temp.m21 = (GLfloat)(yz - xw);
	temp.m22 = (GLfloat)(1 - xx - yy);
	temp.m23 = 0;
	temp.m30 = 0;
	temp.m31 = 0;
	temp.m32 = 0;
	temp.m33 = 1;
	engineMathMat4Multiply(mat, mat, &temp);
}

// ----------------------------------------------------------------

// 逆行列
void engineMathMat4Invert(struct engineMathMatrix44 *mat){
	struct engineMathMatrix44 a;
	engineMathMat4Copy(&a, mat);
	double b00 = a.m00 * a.m11 - a.m01 * a.m10;
	double b01 = a.m00 * a.m12 - a.m02 * a.m10;
	double b02 = a.m00 * a.m13 - a.m03 * a.m10;
	double b03 = a.m01 * a.m12 - a.m02 * a.m11;
	double b04 = a.m01 * a.m13 - a.m03 * a.m11;
	double b05 = a.m02 * a.m13 - a.m03 * a.m12;
	double b06 = a.m20 * a.m31 - a.m21 * a.m30;
	double b07 = a.m20 * a.m32 - a.m22 * a.m30;
	double b08 = a.m20 * a.m33 - a.m23 * a.m30;
	double b09 = a.m21 * a.m32 - a.m22 * a.m31;
	double b10 = a.m21 * a.m33 - a.m23 * a.m31;
	double b11 = a.m22 * a.m33 - a.m23 * a.m32;
	double det = b00 * b11 - b01 * b10 + b02 * b09 + b03 * b08 - b04 * b07 + b05 * b06;
	if(engineMathAbs(det) < DBL_EPSILON){return;}
	double idet = 1 / det;
	mat->m00 = (GLfloat)((a.m11 * b11 - a.m12 * b10 + a.m13 * b09) * idet);
	mat->m01 = (GLfloat)((a.m02 * b10 - a.m01 * b11 - a.m03 * b09) * idet);
	mat->m02 = (GLfloat)((a.m31 * b05 - a.m32 * b04 + a.m33 * b03) * idet);
	mat->m03 = (GLfloat)((a.m22 * b04 - a.m21 * b05 - a.m23 * b03) * idet);
	mat->m10 = (GLfloat)((a.m12 * b08 - a.m10 * b11 - a.m13 * b07) * idet);
	mat->m11 = (GLfloat)((a.m00 * b11 - a.m02 * b08 + a.m03 * b07) * idet);
	mat->m12 = (GLfloat)((a.m32 * b02 - a.m30 * b05 - a.m33 * b01) * idet);
	mat->m13 = (GLfloat)((a.m20 * b05 - a.m22 * b02 + a.m23 * b01) * idet);
	mat->m20 = (GLfloat)((a.m10 * b10 - a.m11 * b08 + a.m13 * b06) * idet);
	mat->m21 = (GLfloat)((a.m01 * b08 - a.m00 * b10 - a.m03 * b06) * idet);
	mat->m22 = (GLfloat)((a.m30 * b04 - a.m31 * b02 + a.m33 * b00) * idet);
	mat->m23 = (GLfloat)((a.m21 * b02 - a.m20 * b04 - a.m23 * b00) * idet);
	mat->m30 = (GLfloat)((a.m11 * b07 - a.m10 * b09 - a.m12 * b06) * idet);
	mat->m31 = (GLfloat)((a.m00 * b09 - a.m01 * b07 + a.m02 * b06) * idet);
	mat->m32 = (GLfloat)((a.m31 * b01 - a.m30 * b03 - a.m32 * b00) * idet);
	mat->m33 = (GLfloat)((a.m20 * b03 - a.m21 * b01 + a.m22 * b00) * idet);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 行列複製
void engineMathMat3Copy(struct engineMathMatrix33 *dst, struct engineMathMatrix33 *src){
	memcpy(dst, src, sizeof(struct engineMathMatrix33));
}

// 行列複製
void engineMathMat3Copy4(struct engineMathMatrix33 *dst, struct engineMathMatrix44 *src){
	dst->m00 = src->m00; dst->m01 = src->m01; dst->m02 = src->m02;
	dst->m10 = src->m10; dst->m11 = src->m11; dst->m12 = src->m12;
	dst->m20 = src->m20; dst->m21 = src->m21; dst->m22 = src->m22;
}

// ----------------------------------------------------------------

// 単位行列
void engineMathMat3Identity(struct engineMathMatrix33 *mat){
	mat->m00 = 1.0; mat->m01 = 0.0; mat->m02 = 0.0;
	mat->m10 = 0.0; mat->m11 = 1.0; mat->m12 = 0.0;
	mat->m20 = 0.0; mat->m21 = 0.0; mat->m22 = 1.0;
}

// ----------------------------------------------------------------

// 行列の掛け合わせ
void engineMathMat3Multiply(struct engineMathMatrix33 *mat, struct engineMathMatrix33 *m0, struct engineMathMatrix33 *m1){
	struct engineMathMatrix33 a;
	struct engineMathMatrix33 b;
	if(mat == m0){engineMathMat3Copy(&a, m0); m0 = &a;}
	if(mat == m1){engineMathMat3Copy(&b, m1); m1 = &b;}
	mat->m00 = m1->m00 * m0->m00 + m1->m01 * m0->m10 + m1->m02 * m0->m20;
	mat->m01 = m1->m00 * m0->m01 + m1->m01 * m0->m11 + m1->m02 * m0->m21;
	mat->m02 = m1->m00 * m0->m02 + m1->m01 * m0->m12 + m1->m02 * m0->m22;
	mat->m10 = m1->m10 * m0->m00 + m1->m11 * m0->m10 + m1->m12 * m0->m20;
	mat->m11 = m1->m10 * m0->m01 + m1->m11 * m0->m11 + m1->m12 * m0->m21;
	mat->m12 = m1->m10 * m0->m02 + m1->m11 * m0->m12 + m1->m12 * m0->m22;
	mat->m20 = m1->m20 * m0->m00 + m1->m21 * m0->m10 + m1->m22 * m0->m20;
	mat->m21 = m1->m20 * m0->m01 + m1->m21 * m0->m11 + m1->m22 * m0->m21;
	mat->m22 = m1->m20 * m0->m02 + m1->m21 * m0->m12 + m1->m22 * m0->m22;
}

// ----------------------------------------------------------------

// 逆行列
void engineMathMat3Invert(struct engineMathMatrix33 *mat){
	struct engineMathMatrix33 a;
	engineMathMat3Copy(&a, mat);
	double b01 = a.m22 * a.m11 - a.m12 * a.m21;
	double b11 = a.m12 * a.m20 - a.m22 * a.m10;
	double b21 = a.m21 * a.m10 - a.m11 * a.m20;
	double det = a.m00 * b01 + a.m01 * b11 + a.m02 * b21;
	if(engineMathAbs(det) < DBL_EPSILON){return;}
	double idet = 1 / det;
	mat->m00 = (GLfloat)(b01 * idet);
	mat->m01 = (GLfloat)((a.m02 * a.m21 - a.m22 * a.m01) * idet);
	mat->m02 = (GLfloat)((a.m12 * a.m01 - a.m02 * a.m11) * idet);
	mat->m10 = (GLfloat)(b11 * idet);
	mat->m11 = (GLfloat)((a.m22 * a.m00 - a.m02 * a.m20) * idet);
	mat->m12 = (GLfloat)((a.m02 * a.m10 - a.m12 * a.m00) * idet);
	mat->m20 = (GLfloat)(b21 * idet);
	mat->m21 = (GLfloat)((a.m01 * a.m20 - a.m21 * a.m00) * idet);
	mat->m22 = (GLfloat)((a.m11 * a.m00 - a.m01 * a.m10) * idet);
}

// 転置行列
void engineMathMat3Transpose(struct engineMathMatrix33 *mat){
	double m01 = mat->m01;
	double m02 = mat->m02;
	double m12 = mat->m12;
	mat->m01 = mat->m10;
	mat->m02 = mat->m20;
	mat->m12 = mat->m21;
	mat->m10 = (GLfloat)m01;
	mat->m20 = (GLfloat)m02;
	mat->m21 = (GLfloat)m12;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ベクトル値設定
void engineMathVec3Set(struct engineMathVector3 *vec, double x, double y, double z){
	vec->x = (GLfloat)x;
	vec->y = (GLfloat)y;
	vec->z = (GLfloat)z;
}

// ベクトル値設定
void engineMathVec4Set(struct engineMathVector4 *vec, double x, double y, double z, double w){
	vec->x = (GLfloat)x;
	vec->y = (GLfloat)y;
	vec->z = (GLfloat)z;
	vec->w = (GLfloat)w;
}

// ----------------------------------------------------------------

// ベクトル行列積
void engineMathVec3MultiplyMat4(struct engineMathVector3 *dst, struct engineMathVector3 *src, struct engineMathMatrix44 *mat){
	struct engineMathVector3 a;
	if(dst == src){
		a.x = src->x;
		a.y = src->y;
		a.z = src->z;
		src = &a;
	}
	dst->x = mat->m00 * src->x + mat->m10 * src->y + mat->m20 * src->z + mat->m30;
	dst->y = mat->m01 * src->x + mat->m11 * src->y + mat->m21 * src->z + mat->m31;
	dst->z = mat->m02 * src->x + mat->m12 * src->y + mat->m22 * src->z + mat->m32;
}

// ベクトルクォータニオン積
void engineMathVec3MultiplyQuat(struct engineMathVector3 *dst, struct engineMathVector3 *src, struct engineMathQuaternion *quat){
	double x = + quat->x *      0 + quat->y * src->z - quat->z * src->y + quat->w * src->x;
	double y = - quat->x * src->z + quat->y *      0 + quat->z * src->x + quat->w * src->y;
	double z = + quat->x * src->y - quat->y * src->x + quat->z *      0 + quat->w * src->z;
	double w = - quat->x * src->x - quat->y * src->y - quat->z * src->z + quat->w *      0;
	dst->x = (GLfloat)(+ x *  quat->w + y * -quat->z - z * -quat->y + w * -quat->x);
	dst->y = (GLfloat)(- x * -quat->z + y *  quat->w + z * -quat->x + w * -quat->y);
	dst->z = (GLfloat)(+ x * -quat->y - y * -quat->x + z *  quat->w + w * -quat->z);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// クォータニオン値設定
void engineMathQuatSet(struct engineMathQuaternion *quat, double x, double y, double z, double w){
	quat->x = (GLfloat)x;
	quat->y = (GLfloat)y;
	quat->z = (GLfloat)z;
	quat->w = (GLfloat)w;
}

// クォータニオン複製
void engineMathQuatCopy(struct engineMathQuaternion *dst, struct engineMathQuaternion *src){
	memcpy(dst, src, sizeof(struct engineMathQuaternion));
}

// ----------------------------------------------------------------

// 単位クォータニオン設定
void engineMathQuatIdentity(struct engineMathQuaternion *quat){
	quat->x = 0.0;
	quat->y = 0.0;
	quat->z = 0.0;
	quat->w = 1.0;
}

// 回転クォータニオン設定
void engineMathQuatRotation(struct engineMathQuaternion *quat, struct engineMathVector3 *axis, double rad){
	double c = engineMathCos(rad * 0.5);
	double s = engineMathSin(rad * 0.5);
	double dot = axis->x * axis->x + axis->y * axis->y + axis->z * axis->z;
	if(engineMathAbs(dot) < DBL_EPSILON){
		quat->x = 0.0;
		quat->y = 0.0;
		quat->z = 0.0;
		quat->w = 1.0;
	}else if(engineMathAbs(dot - 1.0) < DBL_EPSILON){
		quat->x = (GLfloat)(s * axis->x);
		quat->y = (GLfloat)(s * axis->y);
		quat->z = (GLfloat)(s * axis->z);
		quat->w = (GLfloat)c;
	}else{
		double idot = 1 / engineMathSqrt(dot);
		quat->x = (GLfloat)(s * axis->x * idot);
		quat->y = (GLfloat)(s * axis->y * idot);
		quat->z = (GLfloat)(s * axis->z * idot);
		quat->w = (GLfloat)c;
	}
}

// ----------------------------------------------------------------

// クォータニオン積
void engineMathQuatMultiply(struct engineMathQuaternion *quat, struct engineMathQuaternion *q0, struct engineMathQuaternion *q1){
	struct engineMathQuaternion a;
	struct engineMathQuaternion b;
	if(quat == q0){engineMathQuatCopy(&a, q0); q0 = &a;}
	if(quat == q1){engineMathQuatCopy(&b, q1); q1 = &b;}
	quat->x = + q0->x * q1->w + q0->y * q1->z - q0->z * q1->y + q0->w * q1->x;
	quat->y = - q0->x * q1->z + q0->y * q1->w + q0->z * q1->x + q0->w * q1->y;
	quat->z = + q0->x * q1->y - q0->y * q1->x + q0->z * q1->w + q0->w * q1->z;
	quat->w = - q0->x * q1->x - q0->y * q1->y - q0->z * q1->z + q0->w * q1->w;
}

// ----------------------------------------------------------------

// クォータニオン正規化
void engineMathQuatNormalize(struct engineMathQuaternion *quat){
	double dot = quat->x * quat->x + quat->y * quat->y + quat->z * quat->z + quat->w * quat->w;
	if(engineMathAbs(dot) < DBL_EPSILON){return;}
	double idot = 1 / engineMathSqrt(dot);
	quat->x = (GLfloat)(quat->x * idot);
	quat->y = (GLfloat)(quat->y * idot);
	quat->z = (GLfloat)(quat->z * idot);
	quat->w = (GLfloat)(quat->w * idot);
}

// 逆クォータニオン
void engineMathQuatInvert(struct engineMathQuaternion *quat){
	double dot = quat->x * quat->x + quat->y * quat->y + quat->z * quat->z + quat->w * quat->w;
	if(engineMathAbs(dot) < DBL_EPSILON){return;}
	double idot = 1 / dot;
	quat->x = (GLfloat)(-quat->x * idot);
	quat->y = (GLfloat)(-quat->y * idot);
	quat->z = (GLfloat)(-quat->z * idot);
	quat->w = (GLfloat)( quat->w * idot);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

