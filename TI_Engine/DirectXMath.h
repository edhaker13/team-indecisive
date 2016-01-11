// Original Source DirectXTK/Inc/SimpleMath.h
//-------------------------------------------------------------------------------------
// SimpleMath.h -- Simplified C++ Math wrapper for DirectXMath
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//  
// Copyright (c) Microsoft Corporation. All rights reserved.
//
// http://go.microsoft.com/fwlink/?LinkId=248929
//-------------------------------------------------------------------------------------
#pragma once
#define NOMINMAX
#include <memory>
#include <directxmath.h>
#include <d3d11.h>
#include "LibraryAPI.h"

namespace Indecisive
{
	struct Vector4;
	struct Matrix;
	using namespace DirectX;

	struct Vector2 : public XMFLOAT2
	{
		Vector2() : XMFLOAT2(0.f, 0.f) {}
		explicit Vector2(float x) : XMFLOAT2(x, x) {}
		Vector2(float _x, float _y) : XMFLOAT2(_x, _y) {}
		explicit Vector2(_In_reads_(2) const float *pArray) : XMFLOAT2(pArray) {}
		Vector2(FXMVECTOR V) { XMStoreFloat2(this, V); }
		Vector2(const XMFLOAT2& V) { this->x = V.x; this->y = V.y; }

		operator XMVECTOR() const { return XMLoadFloat2(this); }

		// Comparision operators
		bool operator == (const Vector2& V) const;
		bool operator != (const Vector2& V) const;

		// Assignment operators
		Vector2& operator= (const Vector2& V) { x = V.x; y = V.y; return *this; }
		Vector2& operator= (const XMFLOAT2& V) { x = V.x; y = V.y; return *this; }
		Vector2& operator+= (const Vector2& V);
		Vector2& operator-= (const Vector2& V);
		Vector2& operator*= (const Vector2& V);
		Vector2& operator*= (float S);
		Vector2& operator/= (float S);

		// Urnary operators
		Vector2 operator+ () const { return *this; }
		Vector2 operator- () const { return Vector2(-x, -y); }

		// Vector operations
		bool InBounds(const Vector2& Bounds) const;

		float Length() const;
		float LengthSquared() const;

		float Dot(const Vector2& V) const;
		void Cross(const Vector2& V, Vector2& result) const;
		Vector2 Cross(const Vector2& V) const;

		void Normalize();
		void Normalize(Vector2& result) const;

		void Clamp(const Vector2& vmin, const Vector2& vmax);
		void Clamp(const Vector2& vmin, const Vector2& vmax, Vector2& result) const;

		// Static functions
		static float Distance(const Vector2& v1, const Vector2& v2);
		static float DistanceSquared(const Vector2& v1, const Vector2& v2);

		static void Min(const Vector2& v1, const Vector2& v2, Vector2& result);
		static Vector2 Min(const Vector2& v1, const Vector2& v2);

		static void Max(const Vector2& v1, const Vector2& v2, Vector2& result);
		static Vector2 Max(const Vector2& v1, const Vector2& v2);

		static void Lerp(const Vector2& v1, const Vector2& v2, float t, Vector2& result);
		static Vector2 Lerp(const Vector2& v1, const Vector2& v2, float t);

		static void SmoothStep(const Vector2& v1, const Vector2& v2, float t, Vector2& result);
		static Vector2 SmoothStep(const Vector2& v1, const Vector2& v2, float t);

		static void Barycentric(const Vector2& v1, const Vector2& v2, const Vector2& v3, float f, float g, Vector2& result);
		static Vector2 Barycentric(const Vector2& v1, const Vector2& v2, const Vector2& v3, float f, float g);

		static void CatmullRom(const Vector2& v1, const Vector2& v2, const Vector2& v3, const Vector2& v4, float t, Vector2& result);
		static Vector2 CatmullRom(const Vector2& v1, const Vector2& v2, const Vector2& v3, const Vector2& v4, float t);

		static void Hermite(const Vector2& v1, const Vector2& t1, const Vector2& v2, const Vector2& t2, float t, Vector2& result);
		static Vector2 Hermite(const Vector2& v1, const Vector2& t1, const Vector2& v2, const Vector2& t2, float t);

		static void Reflect(const Vector2& ivec, const Vector2& nvec, Vector2& result);
		static Vector2 Reflect(const Vector2& ivec, const Vector2& nvec);

		static void Refract(const Vector2& ivec, const Vector2& nvec, float refractionIndex, Vector2& result);
		static Vector2 Refract(const Vector2& ivec, const Vector2& nvec, float refractionIndex);

		static void Transform(const Vector2& v, const Matrix& m, Vector2& result);
		static Vector2 Transform(const Vector2& v, const Matrix& m);
		static void Transform(_In_reads_(count) const Vector2* varray, size_t count, const Matrix& m, _Out_writes_(count) Vector2* resultArray);

		static void Transform(const Vector2& v, const Matrix& m, Vector4& result);
		static void Transform(_In_reads_(count) const Vector2* varray, size_t count, const Matrix& m, _Out_writes_(count) Vector4* resultArray);

		static void TransformNormal(const Vector2& v, const Matrix& m, Vector2& result);
		static Vector2 TransformNormal(const Vector2& v, const Matrix& m);
		static void TransformNormal(_In_reads_(count) const Vector2* varray, size_t count, const Matrix& m, _Out_writes_(count) Vector2* resultArray);

		// Constants
		static const Vector2 Zero;
		static const Vector2 One;
		static const Vector2 UnitX;
		static const Vector2 UnitY;
	};

	// Binary operators
	Vector2 operator+ (const Vector2& V1, const Vector2& V2);
	Vector2 operator- (const Vector2& V1, const Vector2& V2);
	Vector2 operator* (const Vector2& V1, const Vector2& V2);
	Vector2 operator* (const Vector2& V, float S);
	Vector2 operator/ (const Vector2& V1, const Vector2& V2);
	Vector2 operator* (float S, const Vector2& V);

	struct Vector3 : public XMFLOAT3
	{
		Vector3() : XMFLOAT3(0.f, 0.f, 0.f) {}
		explicit Vector3(float x) : XMFLOAT3(x, x, x) {}
		Vector3(float _x, float _y, float _z) : XMFLOAT3(_x, _y, _z) {}
		explicit Vector3(_In_reads_(3) const float *pArray) : XMFLOAT3(pArray) {}
		Vector3(FXMVECTOR V) { XMStoreFloat3(this, V); }
		Vector3(const XMFLOAT3& V) { this->x = V.x; this->y = V.y; this->z = V.z; }

		operator XMVECTOR() const { return XMLoadFloat3(this); };

		// Comparision operators
		LIBRARY_API bool operator == (const Vector3& V) const;
		LIBRARY_API bool operator != (const Vector3& V) const;

		// Assignment operators
		LIBRARY_API Vector3& operator= (const Vector3& V) { x = V.x; y = V.y; z = V.z; return *this; }
		LIBRARY_API Vector3& operator= (const XMFLOAT3& V) { x = V.x; y = V.y; z = V.z; return *this; }
		LIBRARY_API Vector3& operator+= (const Vector3& V);
		LIBRARY_API Vector3& operator-= (const Vector3& V);
		LIBRARY_API Vector3& operator*= (const Vector3& V);
		LIBRARY_API Vector3& operator*= (float S);
		LIBRARY_API Vector3& operator/= (float S);

		// Urnary operators
		LIBRARY_API Vector3 operator+ () const { return *this; }
		LIBRARY_API Vector3 operator- () const;

		// Vector operations
		LIBRARY_API bool InBounds(const Vector3& Bounds) const;

		LIBRARY_API float Length() const;
		LIBRARY_API float LengthSquared() const;

		LIBRARY_API float Dot(const Vector3& V) const;
		LIBRARY_API void Cross(const Vector3& V, Vector3& result) const;
		LIBRARY_API Vector3 Cross(const Vector3& V) const;

		LIBRARY_API void Normalize();
		//LIBRARY_API void Normalize(Vector3& result) const;

		LIBRARY_API void Clamp(const Vector3& vmin, const Vector3& vmax);
		LIBRARY_API void Clamp(const Vector3& vmin, const Vector3& vmax, Vector3& result) const;

		// Static functions
		LIBRARY_API static float Distance(const Vector3& v1, const Vector3& v2);
		LIBRARY_API static float DistanceSquared(const Vector3& v1, const Vector3& v2);

		LIBRARY_API static void Min(const Vector3& v1, const Vector3& v2, Vector3& result);
		LIBRARY_API static Vector3 Min(const Vector3& v1, const Vector3& v2);

		LIBRARY_API static void Max(const Vector3& v1, const Vector3& v2, Vector3& result);
		LIBRARY_API static Vector3 Max(const Vector3& v1, const Vector3& v2);

		LIBRARY_API static void Normalize(const Vector3& V, Vector3& result);
		LIBRARY_API static Vector3 Normalize(const Vector3& V);

		LIBRARY_API static void Lerp(const Vector3& v1, const Vector3& v2, float t, Vector3& result);
		LIBRARY_API static Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);

		LIBRARY_API static void SmoothStep(const Vector3& v1, const Vector3& v2, float t, Vector3& result);
		LIBRARY_API static Vector3 SmoothStep(const Vector3& v1, const Vector3& v2, float t);

		LIBRARY_API static void Barycentric(const Vector3& v1, const Vector3& v2, const Vector3& v3, float f, float g, Vector3& result);
		LIBRARY_API static Vector3 Barycentric(const Vector3& v1, const Vector3& v2, const Vector3& v3, float f, float g);

		LIBRARY_API static void CatmullRom(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& v4, float t, Vector3& result);
		LIBRARY_API static Vector3 CatmullRom(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& v4, float t);

		LIBRARY_API static void Hermite(const Vector3& v1, const Vector3& t1, const Vector3& v2, const Vector3& t2, float t, Vector3& result);
		LIBRARY_API static Vector3 Hermite(const Vector3& v1, const Vector3& t1, const Vector3& v2, const Vector3& t2, float t);

		LIBRARY_API static void Reflect(const Vector3& ivec, const Vector3& nvec, Vector3& result);
		LIBRARY_API static Vector3 Reflect(const Vector3& ivec, const Vector3& nvec);

		LIBRARY_API static void Refract(const Vector3& ivec, const Vector3& nvec, float refractionIndex, Vector3& result);
		LIBRARY_API static Vector3 Refract(const Vector3& ivec, const Vector3& nvec, float refractionIndex);

		LIBRARY_API static void Transform(const Vector3& v, const Matrix& m, Vector3& result);
		LIBRARY_API static Vector3 Transform(const Vector3& v, const Matrix& m);
		LIBRARY_API static void Transform(_In_reads_(count) const Vector3* varray, size_t count, const Matrix& m, _Out_writes_(count) Vector3* resultArray);

		LIBRARY_API static void Transform(const Vector3& v, const Matrix& m, Vector4& result);
		LIBRARY_API static void Transform(_In_reads_(count) const Vector3* varray, size_t count, const Matrix& m, _Out_writes_(count) Vector4* resultArray);

		LIBRARY_API static void TransformNormal(const Vector3& v, const Matrix& m, Vector3& result);
		LIBRARY_API static Vector3 TransformNormal(const Vector3& v, const Matrix& m);
		LIBRARY_API static void TransformNormal(_In_reads_(count) const Vector3* varray, size_t count, const Matrix& m, _Out_writes_(count) Vector3* resultArray);

		// Constants
		LIBRARY_API static const Vector3 Zero;
		LIBRARY_API static const Vector3 One;
		LIBRARY_API static const Vector3 UnitX;
		LIBRARY_API static const Vector3 UnitY;
		LIBRARY_API static const Vector3 UnitZ;
		LIBRARY_API static const Vector3 Up;
		LIBRARY_API static const Vector3 Down;
		LIBRARY_API static const Vector3 Right;
		LIBRARY_API static const Vector3 Left;
		LIBRARY_API static const Vector3 Forward;
		LIBRARY_API static const Vector3 Backward;
	};

	// Binary operators
	LIBRARY_API Vector3 operator+ (const Vector3& V1, const Vector3& V2);
	LIBRARY_API Vector3 operator- (const Vector3& V1, const Vector3& V2);
	LIBRARY_API Vector3 operator* (const Vector3& V1, const Vector3& V2);
	LIBRARY_API Vector3 operator* (const Vector3& V, float S);
	LIBRARY_API Vector3 operator/ (const Vector3& V1, const Vector3& V2);
	LIBRARY_API Vector3 operator* (float S, const Vector3& V);

	// Stream operators
	LIBRARY_API std::istream& operator >>(std::istream& s, Vector3& v);
	LIBRARY_API std::ostream& operator <<(std::ostream& s, const Vector3& v);

	//------------------------------------------------------------------------------
	// 4D vector
	struct Vector4 : public XMFLOAT4
	{
		Vector4() : XMFLOAT4(0.f, 0.f, 0.f, 0.f) {}
		explicit Vector4(float x) : XMFLOAT4(x, x, x, x) {}
		Vector4(float _x, float _y, float _z, float _w) : XMFLOAT4(_x, _y, _z, _w) {}
		explicit Vector4(_In_reads_(4) const float *pArray) : XMFLOAT4(pArray) {}
		Vector4(FXMVECTOR V) { XMStoreFloat4(this, V); }
		Vector4(const XMFLOAT4& V) { this->x = V.x; this->y = V.y; this->z = V.z; this->w = V.w; }

		operator XMVECTOR() const { return XMLoadFloat4(this); }

		// Comparision operators
		bool operator == (const Vector4& V) const;
		bool operator != (const Vector4& V) const;

		// Assignment operators
		Vector4& operator= (const Vector4& V) { x = V.x; y = V.y; z = V.z; w = V.w; return *this; }
		Vector4& operator= (const XMFLOAT4& V) { x = V.x; y = V.y; z = V.z; w = V.w; return *this; }
		Vector4& operator+= (const Vector4& V);
		Vector4& operator-= (const Vector4& V);
		Vector4& operator*= (const Vector4& V);
		Vector4& operator*= (float S);
		Vector4& operator/= (float S);

		// Urnary operators
		Vector4 operator+ () const { return *this; }
		Vector4 operator- () const;

		// Vector operations
		bool InBounds(const Vector4& Bounds) const;

		float Length() const;
		float LengthSquared() const;

		float Dot(const Vector4& V) const;
		void Cross(const Vector4& v1, const Vector4& v2, Vector4& result) const;
		Vector4 Cross(const Vector4& v1, const Vector4& v2) const;

		void Normalize();
		void Normalize(Vector4& result) const;

		void Clamp(const Vector4& vmin, const Vector4& vmax);
		void Clamp(const Vector4& vmin, const Vector4& vmax, Vector4& result) const;

		// Static functions
		static float Distance(const Vector4& v1, const Vector4& v2);
		static float DistanceSquared(const Vector4& v1, const Vector4& v2);

		static void Min(const Vector4& v1, const Vector4& v2, Vector4& result);
		static Vector4 Min(const Vector4& v1, const Vector4& v2);

		static void Max(const Vector4& v1, const Vector4& v2, Vector4& result);
		static Vector4 Max(const Vector4& v1, const Vector4& v2);

		static void Lerp(const Vector4& v1, const Vector4& v2, float t, Vector4& result);
		static Vector4 Lerp(const Vector4& v1, const Vector4& v2, float t);

		static void SmoothStep(const Vector4& v1, const Vector4& v2, float t, Vector4& result);
		static Vector4 SmoothStep(const Vector4& v1, const Vector4& v2, float t);

		static void Barycentric(const Vector4& v1, const Vector4& v2, const Vector4& v3, float f, float g, Vector4& result);
		static Vector4 Barycentric(const Vector4& v1, const Vector4& v2, const Vector4& v3, float f, float g);

		static void CatmullRom(const Vector4& v1, const Vector4& v2, const Vector4& v3, const Vector4& v4, float t, Vector4& result);
		static Vector4 CatmullRom(const Vector4& v1, const Vector4& v2, const Vector4& v3, const Vector4& v4, float t);

		static void Hermite(const Vector4& v1, const Vector4& t1, const Vector4& v2, const Vector4& t2, float t, Vector4& result);
		static Vector4 Hermite(const Vector4& v1, const Vector4& t1, const Vector4& v2, const Vector4& t2, float t);

		static void Reflect(const Vector4& ivec, const Vector4& nvec, Vector4& result);
		static Vector4 Reflect(const Vector4& ivec, const Vector4& nvec);

		static void Refract(const Vector4& ivec, const Vector4& nvec, float refractionIndex, Vector4& result);
		static Vector4 Refract(const Vector4& ivec, const Vector4& nvec, float refractionIndex);

		static void Transform(const Vector4& v, const Matrix& m, Vector4& result);
		static Vector4 Transform(const Vector4& v, const Matrix& m);
		static void Transform(_In_reads_(count) const Vector4* varray, size_t count, const Matrix& m, _Out_writes_(count) Vector4* resultArray);

		// Constants
		static const Vector4 Zero;
		static const Vector4 One;
		static const Vector4 UnitX;
		static const Vector4 UnitY;
		static const Vector4 UnitZ;
		static const Vector4 UnitW;
	};

	// Binary operators
	Vector4 operator+ (const Vector4& V1, const Vector4& V2);
	Vector4 operator- (const Vector4& V1, const Vector4& V2);
	Vector4 operator* (const Vector4& V1, const Vector4& V2);
	Vector4 operator* (const Vector4& V, float S);
	Vector4 operator/ (const Vector4& V1, const Vector4& V2);
	Vector4 operator* (float S, const Vector4& V);

	//------------------------------------------------------------------------------
	// 4x4 Matrix (assumes right-handed cooordinates)
	struct Matrix : public XMFLOAT4X4
	{
		Matrix() : XMFLOAT4X4(
			1.f, 0, 0, 0,
			0, 1.f, 0, 0,
			0, 0, 1.f, 0,
			0, 0, 0, 1.f)
		{}
		Matrix(float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33) : XMFLOAT4X4(m00, m01, m02, m03,
			m10, m11, m12, m13,
			m20, m21, m22, m23,
			m30, m31, m32, m33)
		{}
		explicit Matrix(const Vector3& r0, const Vector3& r1, const Vector3& r2) : XMFLOAT4X4(r0.x, r0.y, r0.z, 0,
			r1.x, r1.y, r1.z, 0,
			r2.x, r2.y, r2.z, 0,
			0, 0, 0, 1.f)
		{}
		explicit Matrix(const Vector4& r0, const Vector4& r1, const Vector4& r2, const Vector4& r3) : XMFLOAT4X4(r0.x, r0.y, r0.z, r0.w,
			r1.x, r1.y, r1.z, r1.w,
			r2.x, r2.y, r2.z, r2.w,
			r3.x, r3.y, r3.z, r3.w)
		{}
		Matrix(const XMFLOAT4X4& M) { memcpy_s(this, sizeof(float) * 16, &M, sizeof(XMFLOAT4X4)); }

		explicit Matrix(_In_reads_(16) const float *pArray) : XMFLOAT4X4(pArray) {}
		Matrix(CXMMATRIX M) { XMStoreFloat4x4(this, M); }

		operator XMMATRIX() const { return XMLoadFloat4x4(this); }

		// Comparision operators
		LIBRARY_API bool operator == (const Matrix& M) const;
		LIBRARY_API bool operator != (const Matrix& M) const;

		// Assignment operators
		LIBRARY_API Matrix& operator= (const Matrix& M) { memcpy_s(this, sizeof(float) * 16, &M, sizeof(float) * 16); return *this; }
		LIBRARY_API Matrix& operator= (const XMFLOAT4X4& M) { memcpy_s(this, sizeof(float) * 16, &M, sizeof(XMFLOAT4X4)); return *this; }
		LIBRARY_API Matrix& operator+= (const Matrix& M);
		LIBRARY_API Matrix& operator-= (const Matrix& M);
		LIBRARY_API Matrix& operator*= (const Matrix& M);
		LIBRARY_API Matrix& operator*= (float S);
		LIBRARY_API Matrix& operator/= (float S);

		// Element-wise divide
		LIBRARY_API Matrix& operator/= (const Matrix& M);

		// Urnary operators
		Matrix operator+ () const { return *this; }
		Matrix operator- () const;

		// Properties
		Vector3 Up() const { return Vector3(_21, _22, _23); }
		void Up(const Vector3& v) { _21 = v.x; _22 = v.y; _23 = v.z; }

		Vector3 Down() const { return Vector3(-_21, -_22, -_23); }
		void Down(const Vector3& v) { _21 = -v.x; _22 = -v.y; _23 = -v.z; }

		Vector3 Right() const { return Vector3(_11, _12, _13); }
		void Right(const Vector3& v) { _11 = v.x; _12 = v.y; _13 = v.z; }

		Vector3 Left() const { return Vector3(-_11, -_12, -_13); }
		void Left(const Vector3& v) { _11 = -v.x; _12 = -v.y; _13 = -v.z; }

		Vector3 Forward() const { return Vector3(-_31, -_32, -_33); }
		void Forward(const Vector3& v) { _31 = -v.x; _32 = -v.y; _33 = -v.z; }

		Vector3 Backward() const { return Vector3(_31, _32, _33); }
		void Backward(const Vector3& v) { _31 = v.x; _32 = v.y; _33 = v.z; }

		Vector3 Translation() const { return Vector3(_41, _42, _43); }
		void Translation(const Vector3& v) { _41 = v.x; _42 = v.y; _43 = v.z; }

		// Matrix operations
		LIBRARY_API Matrix Transpose() const;
		LIBRARY_API void Transpose(Matrix& result) const;

		LIBRARY_API Matrix Invert() const;
		LIBRARY_API void Invert(Matrix& result) const;

		LIBRARY_API float Determinant() const;

		// Static functions
		LIBRARY_API static Matrix CreateBillboard(const Vector3& object, const Vector3& cameraPosition, const Vector3& cameraUp, _In_opt_ const Vector3* cameraForward = nullptr);

		LIBRARY_API static Matrix CreateConstrainedBillboard(const Vector3& object, const Vector3& cameraPosition, const Vector3& rotateAxis,
			_In_opt_ const Vector3* cameraForward = nullptr, _In_opt_ const Vector3* objectForward = nullptr);

		LIBRARY_API static Matrix CreateTranslation(const Vector3& position);
		LIBRARY_API static Matrix CreateTranslation(float x, float y, float z);

		LIBRARY_API static Matrix CreateScale(const Vector3& scales);
		LIBRARY_API static Matrix CreateScale(float xs, float ys, float zs);
		LIBRARY_API static Matrix CreateScale(float scale);

		LIBRARY_API static Matrix CreateRotationX(float radians);
		LIBRARY_API static Matrix CreateRotationY(float radians);
		LIBRARY_API static Matrix CreateRotationZ(float radians);

		LIBRARY_API static Matrix CreateFromAxisAngle(const Vector3& axis, float angle);

		LIBRARY_API static Matrix CreatePerspectiveFieldOfView(float fov, float aspectRatio, float nearPlane, float farPlane);
		LIBRARY_API static Matrix CreatePerspective(float width, float height, float nearPlane, float farPlane);
		LIBRARY_API static Matrix CreatePerspectiveOffCenter(float left, float right, float bottom, float top, float nearPlane, float farPlane);
		LIBRARY_API static Matrix CreateOrthographic(float width, float height, float zNearPlane, float zFarPlane);
		LIBRARY_API static Matrix CreateOrthographicOffCenter(float left, float right, float bottom, float top, float zNearPlane, float zFarPlane);

		LIBRARY_API static Matrix CreateLookAt(const Vector3& position, const Vector3& target, const Vector3& up);
		LIBRARY_API static Matrix CreateWorld(const Vector3& position, const Vector3& forward, const Vector3& up);

		LIBRARY_API static Matrix CreateFromYawPitchRoll(float yaw, float pitch, float roll);

		LIBRARY_API static void Lerp(const Matrix& M1, const Matrix& M2, float t, Matrix& result);
		LIBRARY_API static Matrix Lerp(const Matrix& M1, const Matrix& M2, float t);

		// Constants
		LIBRARY_API static const Matrix Identity;
	};

	// Binary operators
	Matrix operator+ (const Matrix& M1, const Matrix& M2);
	Matrix operator- (const Matrix& M1, const Matrix& M2);
	LIBRARY_API Matrix operator* (const Matrix& M1, const Matrix& M2);
	Matrix operator* (const Matrix& M, float S);
	Matrix operator/ (const Matrix& M, float S);
	Matrix operator/ (const Matrix& M1, const Matrix& M2);
	// Element-wise divide
	Matrix operator* (float S, const Matrix& M);
};