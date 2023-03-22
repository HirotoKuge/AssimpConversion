/*****************************************************************//**
 * \file   Vector.h
 * \brief  �x�N�^�[�N���X
 * 
 * \author Hiroto Kuge
 * \date   December 2022
 *********************************************************************/

#pragma once

//=============================================================================
// Includes
//=============================================================================
#include <DirectXMath.h>
#include "math.h"

//=============================================================================
// Forword Decolations
//=============================================================================
class Matrix;


//=============================================================================
// �񎟌��x�N�g���N���X
//=============================================================================
class Vector2 {
	//-----------------------------------------------------------------------------
	// list of friend classes and methods.
	//-----------------------------------------------------------------------------
	//!{
			/* Nothing */
	//!} 
public:
	//-----------------------------------------------------------------------------
	// public variables.
	//-----------------------------------------------------------------------------
	//!{
	static const Vector2 Zero;
	
	/*****************************************************************//**
	 * \brief �������p��
	 *********************************************************************/
	union {
		DirectX::XMFLOAT2 vec;
		struct { float x, y; };
		float v[2];
	};

	//!} 
	//-----------------------------------------------------------------------------
	// public methods.
	//-----------------------------------------------------------------------------
	//!{
	
	/*****************************************************************//**
	 * \brief �R���X�g���N�^
	 * 
	 * \memo  �Ⴄ�^�̒l�𓯈ꎋ���Ȃ�  
	 *********************************************************************/
	explicit Vector2(){
		x = 0.0f;
		y = 0.0f;
	}
	
	/*****************************************************************//**
	 * \brief �����t���R���X�g���N�^�F������
	 *  
	 * \param x x�v�f�̏����l 
	 * \param y y�v�f�̏����l 
	 *********************************************************************/
	Vector2(float x, float y){
		this->x = x;
		this->y = y;
	}

	/*****************************************************************//**
	 * \brief ������Z�q
	 *  
	 * \param _v �v�Z�Ώ�
	 * \return �v�Z����(���g�̃|�C���^)
	 *********************************************************************/
	Vector2& operator=(const Vector2& _v)
	{
		vec = _v.vec;
		return *this;
	}

	/*****************************************************************//**
	 * \brief �l���Z�b�g
	 *  
	 * \param _x x�v�f
	 * \param _y y�v�f
	 *********************************************************************/
	void Set(float _x, float _y){
		vec.x = _x;
		vec.y = _y;
	}
	
	/*****************************************************************//**
	 * \brief ���`�⊮
	 *  
	 * \param t	 ��ԗ�
	 * \param v0 ��ԊJ�n�̃x�N�g��
	 * \param v1 ��ԏI���̃x�N�g��
	 *********************************************************************/
	void Lerp(float t, const Vector2& v0, const Vector2& v1){
		x = v0.x + (v1.x - v0.x) * t;
		y = v0.y + (v1.y - v0.y) * t;
	}
	
	/*****************************************************************//**
	 * \brief ���K��
	 *********************************************************************/
	void Normalize(){
		DirectX::XMVECTOR xmv = DirectX::XMLoadFloat2(&vec);
		xmv = DirectX::XMVector2Normalize(xmv);
		DirectX::XMStoreFloat2(&vec, xmv);
	}
	//!}
private:
	//-----------------------------------------------------------------------------
	// private variables.
	//-----------------------------------------------------------------------------
	//!{
			/* Nothing */
	//!}
	//-----------------------------------------------------------------------------
	// private methods.
	//-----------------------------------------------------------------------------
	//!{
			/* Nothing */
	//!} 
};


//=============================================================================
/// 3�����x�N�g���N���X 
//=============================================================================
class Vector3 {
	//-----------------------------------------------------------------------------
	// list of friends classes and methods.
	//-----------------------------------------------------------------------------	
	//!{
			/* Nothing */
	//!} 
public:
	//-----------------------------------------------------------------------------
	// public variables.
	//-----------------------------------------------------------------------------
	//!{
public:
	union {
		DirectX::XMFLOAT3 vec;
		float v[3];
		struct { float x, y, z; };
	};
	static const Vector3 Zero;
	static const Vector3 Right;
	static const Vector3 Left;
	static const Vector3 Up;
	static const Vector3 Down;
	static const Vector3 Front;
	static const Vector3 Back;
	static const Vector3 AxisX;
	static const Vector3 AxisY;
	static const Vector3 AxisZ;
	static const Vector3 One;
	//!} 
	//-----------------------------------------------------------------------------
	// public methods.
	//-----------------------------------------------------------------------------
	//!{
public:
	/*****************************************************************//**
	 * \brief XMVECTOR�ւ̈Öق̕ϊ�
	 *********************************************************************/
	operator DirectX::XMVECTOR() const {
		return DirectX::XMLoadFloat3(&vec);
	}

	/*****************************************************************//**
	 * \brief ������Z�q
	 *********************************************************************/
	Vector3& operator=(const Vector3& _v)
	{
		vec = _v.vec;
		return *this;
	}

	/*****************************************************************//**
	 * \brief �R���X�g���N�^
	 *
	 * \memo x,y,z�S�Ă̗v�f��0�ŏ�����
	 *********************************************************************/
	explicit Vector3() {
		x = y = z = 0.0f;
	}
	
	/*****************************************************************//**
	 * \brief �R���X�g���N�^
	 * \param x �������l	
	 * \param y �������l
	 * \param z �������l
	 *********************************************************************/
	Vector3(float x, float y, float z){
		Set(x, y, z);
	}
	
	/*****************************************************************//**
	 * \brief ���`�⊮
	 * \param t  �⊮��
	 * \param v0 �⊮�J�n�x�N�g��
	 * \param v1 �⊮�I���x�N�g��
	 *********************************************************************/
	void Lerp(float t, const Vector3& v0, const Vector3& v1){
		DirectX::XMVECTOR _v = DirectX::XMVectorLerp(
			DirectX::XMLoadFloat3(&v0.vec),
			DirectX::XMLoadFloat3(&v1.vec),
			t);
		DirectX::XMStoreFloat3(&vec, _v);
	}
	
	/*****************************************************************//**
	 * \brief �x�N�g���̃R�s�[
	 * \param dst �R�s�[��
	 *********************************************************************/
	template<class TVector>
	void CopyTo(TVector& dst) const{
		dst.x = x;
		dst.y = y;
		dst.z = z;
	}

	/*****************************************************************//**
	 * \brief �x�N�g���̐ݒ�
	 * \param _x
	 * \param _y
	 * \param _z
	 *********************************************************************/
	void Set(float _x, float _y, float _z){
		vec.x = _x;
		vec.y = _y;
		vec.z = _z;
	}
	template<class TVector>
	void Set(TVector& _v){
		Set(_v.x, _v.y, _v.z);
	}

	
	
	/*****************************************************************//**
	 * \brief �x�N�g���̉��Z
	 * 
	 * \param v1 ���Z�����x�N�g��
	 *********************************************************************/
	void Add(const Vector3& _v){
		DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat3(&vec);
		DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat3(&_v.vec);
		DirectX::XMVECTOR xmvr = DirectX::XMVectorAdd(xmv0, xmv1);
		DirectX::XMStoreFloat3(&vec, xmvr);
	}
	
	/*****************************************************************//**
	 * \brief �x�N�g���̉��Z
	 * 
	 * \param v0 ���Z����x�N�g��
	 * \param v1 ���Z�����x�N�g��
	 *********************************************************************/
	void Add(const Vector3& v0, const Vector3& v1){
		DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat3(&v0.vec);
		DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat3(&v1.vec);
		DirectX::XMVECTOR xmvr = DirectX::XMVectorAdd(xmv0, xmv1);
		DirectX::XMStoreFloat3(&vec, xmvr);
	}
	
	/*****************************************************************//**
	 * \brief �x�N�g���̌��Z
	 * 
	 * \param _v ���Z�����x�N�g��
	 *********************************************************************/
	void Subtract(const Vector3& _v){
		DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat3(&vec);
		DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat3(&_v.vec);
		DirectX::XMVECTOR xmvr = DirectX::XMVectorSubtract(xmv0, xmv1);
		DirectX::XMStoreFloat3(&vec, xmvr);
	}
	
	/*****************************************************************//**
	 * \brief �x�N�g���̌��Z
	 * \param v0 �������x�N�g��
	 * \param v1 �����x�N�g��
	 *********************************************************************/
	void Subtract(const Vector3& v0, const Vector3& v1){
		DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat3(&v0.vec);
		DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat3(&v1.vec);
		DirectX::XMVECTOR xmvr = DirectX::XMVectorSubtract(xmv0, xmv1);
		DirectX::XMStoreFloat3(&vec, xmvr);
	}

	
	/*****************************************************************//**
	 * \brief ���ς��v�Z
	 * 
	 * \param _v
	 * \return 
	 *********************************************************************/
	float Dot(const Vector3& _v) const{
		DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat3(&vec);
		DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat3(&_v.vec);
		return DirectX::XMVector3Dot(xmv0, xmv1).m128_f32[0];
	}

	/// <summary>
	/// �O��
	/// </summary>
	/// <remarks>
	/// Vector3 v ;
	/// v.x = this->y * _v.z - this->z * _v.y;
	/// v.y = thiz->z * _v.x - this->x * _v.z;
	/// v.z = this->x * _v.y - this->y * _v.x;
	/// this = v;
	/// </remarks>
	
	/*****************************************************************//**
	 * \brief �O�ς��v�Z
	 * 
	 * \param _v
	 *********************************************************************/
	void Cross(const Vector3& _v){
		DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat3(&vec);
		DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat3(&_v.vec);
		DirectX::XMVECTOR xmvr = DirectX::XMVector3Cross(xmv0, xmv1);
		DirectX::XMStoreFloat3(&vec, xmvr);
	}
	/// <summary>
	/// �O��
	/// </summary>
	/// <remarks>
	/// this->x = v0.y * v1.z - v0.z * v1.y;
	/// this->y = v0.z * v1.x - v0.x * v1.z;
	/// this->z = v0.x * v1.y - v0.y * v1.x; 
	/// </remarks>
	
	/*****************************************************************//**
	 * \brief �O�ς��v�Z
	 * 
	 * \param v0
	 * \param v1
	 *********************************************************************/
	void Cross(const Vector3& v0, const Vector3& v1){
		DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat3(&v0.vec);
		DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat3(&v1.vec);
		DirectX::XMVECTOR xmvr = DirectX::XMVector3Cross(xmv0, xmv1);
		DirectX::XMStoreFloat3(&vec, xmvr);
	}
	
	/*****************************************************************//**
	 * \brief �x�N�g���̒������擾
	 * 
	 * \return 
	 *********************************************************************/
	float Length() const{
		DirectX::XMVECTOR xmv = DirectX::XMLoadFloat3(&vec);
		return DirectX::XMVector3Length(xmv).m128_f32[0];
	}
	
	/*****************************************************************//**
	 * \brief �x�N�g���̒����̂Q����擾
	 * \return 
	 *********************************************************************/
	float LengthSq() const{
		DirectX::XMVECTOR xmv = DirectX::XMLoadFloat3(&vec);
		return DirectX::XMVector3LengthSq(xmv).m128_f32[0];
	}


	/*****************************************************************//**
	 * \brief �x�N�g�����X�J���[�Ŋg��
	 * 
	 * \param s
	 * \memo s���e�v�f�ɂ����Ă�
	 *********************************************************************/
	void Scale(float s){
		DirectX::XMVECTOR xmv = DirectX::XMLoadFloat3(&vec);
		xmv = DirectX::XMVectorScale(xmv, s);
		DirectX::XMStoreFloat3(&vec, xmv);
	}

	/*****************************************************************//**
	 * \brief �x�N�g���𐳋K��
	 *********************************************************************/
	void Normalize(){
		DirectX::XMVECTOR xmv = DirectX::XMLoadFloat3(&vec);
		xmv = DirectX::XMVector3Normalize(xmv);
		DirectX::XMStoreFloat3(&vec, xmv);
	}

	/*****************************************************************//**
	 * \brief �x�N�g�����X�J���[�ŏ��Z
	 * 
	 * \param d
	 * \memo d�Ŋe�v�f�������Ă�
	 *********************************************************************/
	void Div(float d){
		float scale = 1.0f / d;
		Scale(scale);
	}

	
	/*****************************************************************//**
	 * \brief �ő�l��ݒ�
	 * 
	 * \param v �v�f���Ƃɍő�l��������Vector3
	 * \memo �����ł͗v�fn�ɂ���std::max(v.n,this->n)�̏������s���Ă���
	 *********************************************************************/
	void Max(const Vector3& v){
		DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat3(&vec);
		DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat3(&v.vec);
		DirectX::XMStoreFloat3(&vec, DirectX::XMVectorMax(xmv0, xmv1));
	}

	/*****************************************************************//**
	 * \brief �ŏ��l��ݒ�
	 *
	 * \param v �v�f���Ƃɍŏ��l��������Vector3
	 * \memo �����ł͗v�fn�ɂ���std::min(v.n,this->n)�̏������s���Ă���
	 *********************************************************************/
	void Min(const Vector3& v){
		DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat3(&vec);
		DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat3(&v.vec);
		DirectX::XMStoreFloat3(&vec, DirectX::XMVectorMin(xmv0, xmv1));
	}

	/*****************************************************************//**
	 * \brief ���Z������Z�q
	 * 
	 * \param _v ���Z�l
	 * \return 
	 *********************************************************************/
	const Vector3& operator+=(const Vector3& _v){
		Add(_v);
		return *this;
	}
	
	/*****************************************************************//**
	 * \brief ��Z������Z�q
	 * 
	 * \param s �{��
	 * \return 
	 *********************************************************************/
	const Vector3& operator*=(float s){
		Scale(s);
		return *this;
	}
	
	/*****************************************************************//**
	 * \brief ���Z������Z�q
	 * \param _v ���Z�l
	 * \return 
	 *********************************************************************/
	const Vector3& operator-=(const Vector3& _v){
		Subtract(_v);
		return *this;
	}
	
	/*****************************************************************//**
	 * \brief ���Z������Z�q
	 * 
	 * \param s ����l
	 * \return 
	 *********************************************************************/
	const Vector3& operator/=(const float s){
		Div(s);
		return *this;
	}
	//!} 
private:
	//-----------------------------------------------------------------------------
	// private variables.
	//-----------------------------------------------------------------------------
	//!{
			/* Nothing */
	//!}
	//-----------------------------------------------------------------------------
	// private methods.
	//----------------------------------------------------------------------------- 
	//!{
			/* Nothing */
	//!} 
};


//=============================================================================
// 4�����x�N�g��
//=============================================================================
class Vector4 {
	//-----------------------------------------------------------------------------
	// list of friends classes and methods.
	//-----------------------------------------------------------------------------	
	//!{
			/* Nothing */
	//!} 
public:
	//-----------------------------------------------------------------------------
	// public variables.
	//-----------------------------------------------------------------------------
	//!{

	union {
		DirectX::XMFLOAT4 vec;
		struct { float x, y, z, w; };
		struct { float r, g, b, a; };
		float v[4];
	};
	static const Vector4 White;
	static const Vector4 Black;
	static const Vector4 Yellow;
	static const Vector4 Gray;

	//!} 
	//-----------------------------------------------------------------------------
	// public methods.
	//-----------------------------------------------------------------------------
	//!{
public:
	
	/*****************************************************************//**
	 * \brief XMVECTOR�^�ւ̈Öق̌^�ϊ�
	 *********************************************************************/
	operator DirectX::XMVECTOR() const{
		return DirectX::XMLoadFloat4(&vec);
	}
	
	/*****************************************************************//**
	 * \brief ������
	 *********************************************************************/
	explicit Vector4(){
		x = y = z = 0.0f;
		w = 1.0f;
	}
	
	
	/*****************************************************************//**
	 * \brief �R�s�[
	 * 
	 * \param dst	
	 *********************************************************************/
	template<class TVector4>
	void  CopyTo(TVector4& dst) const{
		dst.x = x;
		dst.y = y;
		dst.z = z;
		dst.w = w;
	}

	/*****************************************************************//**
	 * \brief ������Z�q
	 *********************************************************************/
	Vector4& operator=(const Vector4& _v){
		vec = _v.vec;
		return *this;
	}

	/*****************************************************************//**
	 * \brief �R���X�g���N�^
	 * 
	 * \param x X����
	 * \param y	Y����
	 * \param z	Z����
	 * \param w	W����
	 *********************************************************************/
	Vector4(float x, float y, float z, float w){
		Set(x, y, z, w);
	}
	

	/*****************************************************************//**
	 * \brief �����t���R���X�g���N�^
	 * 
	 * \param v 3�����x�N�g��
	 * \memo w�ɂ�1.0���i�[
	 *********************************************************************/
	Vector4(const Vector3& v){
		Set(v);
	}

	/*****************************************************************//**
	 * \brief �x�N�g���̊e�v�f��ݒ�
	 * \param _x X����
	 * \param _y Y����
	 * \param _z Z����
	 * \param _w W����
	 *********************************************************************/
	void Set(float _x, float _y, float _z, float _w){
		this->x = _x;
		this->y = _y;
		this->z = _z;
		this->w = _w;
	}
	
	/*****************************************************************//**
	 * \brief �x�N�g���𐳋K��
	 *********************************************************************/
	void Normalize(){
		DirectX::XMVECTOR xmv = DirectX::XMLoadFloat4(&vec);
		xmv = DirectX::XMVector4Normalize(xmv);
		DirectX::XMStoreFloat4(&vec, xmv);
	}
	
	/*****************************************************************//**
	 * \brief �x�N�g����ݒ�
	 * 
	 * \param _v �ݒ茳
	 *********************************************************************/
	void Set(const Vector4& _v){
		*this = _v;
	}

	/*****************************************************************//**
	 * \brief 3�����x�N�g����ݒ�
	 * 
	 * \param _v
	 * \memo W�����ɂ�1.0���ݒ肳��܂�
	 *********************************************************************/
	void Set(const Vector3& _v){
		this->x = _v.x;
		this->y = _v.y;
		this->z = _v.z;
		this->w = 1.0f;
	}
	
	/*****************************************************************//**
	 * \brief �x�N�g�������Z
	 * 
	 * \param _v
	 *********************************************************************/
	void Add(const Vector4& _v){
		DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat4(&vec);
		DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat4(&_v.vec);
		DirectX::XMVECTOR xmvr = DirectX::XMVectorAdd(xmv0, xmv1);
		DirectX::XMStoreFloat4(&vec, xmvr);
	}

	/*****************************************************************//**
	 * \brief �x�N�g�������Z
	 * 
	 * \param v0
	 * \param v1
	 * \memo this = v0+v1
	 *********************************************************************/
	void Add(const Vector4& v0, const Vector4& v1){
		DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat4(&v0.vec);
		DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat4(&v1.vec);
		DirectX::XMVECTOR xmvr = DirectX::XMVectorAdd(xmv0, xmv1);
		DirectX::XMStoreFloat4(&vec, xmvr);
	}
	
	/*****************************************************************//**
	 * \brief �x�N�g���̌��Z
	 * 
	 * \param _v
	 *********************************************************************/
	void Subtract(const Vector4& _v){
		DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat4(&vec);
		DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat4(&_v.vec);
		DirectX::XMVECTOR xmvr = DirectX::XMVectorSubtract(xmv0, xmv1);
		DirectX::XMStoreFloat4(&vec, xmvr);
	}
	
	/*****************************************************************//**
	 * \brief �x�N�g���̌��Z
	 * 
	 * \param v0
	 * \param v1
	 * \memo this = v0 - v1
	 *********************************************************************/
	void Subtract(const Vector4& v0, const Vector4& v1){
		DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat4(&v0.vec);
		DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat4(&v1.vec);
		DirectX::XMVECTOR xmvr = DirectX::XMVectorSubtract(xmv0, xmv1);
		DirectX::XMStoreFloat4(&vec, xmvr);
	}
	
	/*****************************************************************//**
	 * \brief ���ς��v�Z
	 * 
	 * \param _v
	 * \return ����
	 *********************************************************************/
	float Dot(const Vector4& _v) const{
		DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat4(&vec);
		DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat4(&_v.vec);
		return DirectX::XMVector4Dot(xmv0, xmv1).m128_f32[0];
	}
	
	/*****************************************************************//**
	 * \brief �x�N�g���̒������擾
	 * 
	 * \return ����
	 *********************************************************************/
	float Length(){
		DirectX::XMVECTOR xmv = DirectX::XMLoadFloat4(&vec);
		return DirectX::XMVector4Length(xmv).m128_f32[0];
	}

	/*****************************************************************//**
	 * \brief �x�N�g���̒����̂Q����擾
	 * 
	 * \return �����̂Q��
	 *********************************************************************/
	float LengthSq(){
		DirectX::XMVECTOR xmv = DirectX::XMLoadFloat4(&vec);
		return DirectX::XMVector4LengthSq(xmv).m128_f32[0];
	}
	
	/*****************************************************************//**
	 * \brief �x�N�g�����X�J���[�Ŋg��
	 * 
	 * \param s �g�嗦
	 *********************************************************************/
	void Scale(float s){
		DirectX::XMVECTOR xmv = DirectX::XMLoadFloat4(&vec);
		xmv = DirectX::XMVectorScale(xmv, s);
		DirectX::XMStoreFloat4(&vec, xmv);
	}
	//!} 
private:
	//-----------------------------------------------------------------------------
	// private variables.
	//-----------------------------------------------------------------------------
	//!{
	//!}
	//-----------------------------------------------------------------------------
	// private methods.
	//----------------------------------------------------------------------------- 
	//!{
	//!} 
};

//=============================================================================
// �萔
//=============================================================================
const Vector2 g_vec2Zero = { 0.0f, 0.0f };
const Vector3 g_vec3Zero = { 0.0f,  0.0f,  0.0f };
const Vector3 g_vec3Right = { 1.0f,  0.0f,  0.0f };
const Vector3 g_vec3Left = { -1.0f,  0.0f,  0.0f };
const Vector3 g_vec3Up = { 0.0f,  1.0f,  0.0f };
const Vector3 g_vec3Down = { 0.0f, -1.0f,  0.0f };
const Vector3 g_vec3Front = { 0.0f,   0.0f,  1.0f };
const Vector3 g_vec3Back = { 0.0f,   0.0f, -1.0f };
const Vector3 g_vec3AxisX = { 1.0f,  0.0f,  0.0f };
const Vector3 g_vec3AxisY = { 0.0f,  1.0f,  0.0f };
const Vector3 g_vec3AxisZ = { 0.0f,  0.0f,  1.0f };
const Vector3 g_vec3One = { 1.0f, 1.0f, 1.0f };

const Vector4 g_vec4White = { 1.0f, 1.0f, 1.0f, 1.0f };
const Vector4 g_vec4Black = { 0.0f, 0.0f, 0.0f, 1.0f };
const Vector4 g_vec4Gray = { 0.5f, 0.5f, 0.5f, 1.0f };
const Vector4 g_vec4Yellow = { 1.0f, 1.0f, 0.0f, 1.0f };

//=============================================================================
//�@�N�H�[�^�j�I�� 
//=============================================================================
class Quaternion : public Vector4 {
public:
	static const Quaternion Identity; // �P�ʃN�H�[�^�j�I��
	Quaternion()
	{
		x = y = z = 0.0f;
		w = 1.0f;
	}
	
	/*****************************************************************//**
	 * \brief �R���X�g���N�^
	 * \param x X�����@
	 * \param y	Y����
	 * \param z	Z����
	 * \param w	W����
	 *********************************************************************/
	Quaternion(float x, float y, float z, float w) :
		Vector4(x, y, z, w)
	{}
	
	/*****************************************************************//**
	 * \brief X������̉�]�N�H�[�^�j�I�����쐬
	 * 
	 * \param angle �p�x(Radian)
	 *********************************************************************/
	void SetRotationX(float angle){
		SetRotation(g_vec3AxisX, angle);
	}
	
	/*****************************************************************//**
	 * \brief X������̉�]�N�H�[�^�j�I�����쐬
	 * 
	 * \param angle �p�x(Degree)
	 *********************************************************************/
	void SetRotationDegX(float angle){
		SetRotationDeg(g_vec3AxisX, angle);
	}

	/*****************************************************************//**
	 * \brief Y������̉�]�N�H�[�^�j�I�����쐬
	 *
	 * \param angle �p�x(Radian)
	 *********************************************************************/
	void SetRotationY(float angle){
		SetRotation(g_vec3AxisY, angle);
	}

	/*****************************************************************//**
	 * \brief Y������̉�]�N�H�[�^�j�I�����쐬
	 *
	 * \param angle �p�x(Degree)
	 *********************************************************************/
	void SetRotationDegY(float angle){
		SetRotationDeg(g_vec3AxisY, angle);
	}

	/*****************************************************************//**
	 * \brief Z������̉�]�N�H�[�^�j�I�����쐬
	 *
	 * \param angle �p�x(Radian)
	 *********************************************************************/
	void SetRotationZ(float angle){
		SetRotation(g_vec3AxisZ, angle);
	}

	/*****************************************************************//**
	 * \brief Z������̉�]�N�H�[�^�j�I�����쐬
	 *
	 * \param angle �p�x(Radian)
	 *********************************************************************/
	void SetRotationDegZ(float angle){
		SetRotationDeg(g_vec3AxisZ, angle);
	}

	/*****************************************************************//**
	 * \brief �C�ӂ̎�����̉�]�N�H�[�^�j�I�����쐬
	 * 
	 * \param axis ��]��
	 * \param angle ��]�p�x(Radian)
	 *********************************************************************/
	void SetRotation(const Vector3& axis, float angle){
		float s;
		float halfAngle = angle * 0.5f;
		s = sinf(halfAngle);
		w = cosf(halfAngle);
		x = axis.x * s;
		y = axis.y * s;
		z = axis.z * s;
	}

	/*****************************************************************//**
	 * \brief �C�ӂ̎�����̉�]�N�H�[�^�j�I�����쐬
	 * 
	 * \param axis	��]��
	 * \param angle	��]�p�x(Degree)
	 *********************************************************************/
	void SetRotationDeg(const Vector3& axis, float angle){
		float s;
		float halfAngle = ToRad(angle) * 0.5f;
		s = sinf(halfAngle);
		w = cosf(halfAngle);
		x = axis.x * s;
		y = axis.y * s;
		z = axis.z * s;
	}
	
	/*****************************************************************//**
	 * \brief �s�񂩂�N�H�[�^�j�I�����쐬
	 * 
	 * \param m
	 *********************************************************************/
	void SetRotation(const Matrix& m);
	
	/*****************************************************************//**
	 * \brief From�x�N�g������To�x�N�g���։�]�������]�N�H�[�^�j�I�����쐬
	 * 
	 * \param from
	 * \param to
	 *********************************************************************/
	void SetRotation(Vector3 from, Vector3 to);
	
	/*****************************************************************//**
	 * \brief ���ʐ��`�ۊ�
	 * 
	 * \param t	 �⊮��
	 * \param q1 �J�n
	 * \param q2 �I��
	 *********************************************************************/
	void Slerp(float t, Quaternion q1, Quaternion q2){
		DirectX::XMVECTOR xmv = DirectX::XMQuaternionSlerp(
			DirectX::XMLoadFloat4(&q1.vec),
			DirectX::XMLoadFloat4(&q2.vec),
			t
		);
		DirectX::XMStoreFloat4(&vec, xmv);
	}
	
	/*****************************************************************//**
	 * \brief Y���ł̉�]�����Z
	 * 
	 * \param angle ���Z�����]�p�x(Radian)
	 *********************************************************************/
	void AddRotationY(float angle){
		Quaternion addRot;
		addRot.SetRotation(Vector3::AxisY, angle);
		*this *= addRot;
	}
	
	/*****************************************************************//**
	 * \brief �N�H�[�^�j�I�����m�̏�Z
	 * 
	 * \param rot 
	 * \memo this*=rot
	 *********************************************************************/
	void Multiply(const Quaternion& rot){
		float pw, px, py, pz;
		float qw, qx, qy, qz;

		qw = w; qx = x; qy = y; qz = z;
		pw = rot.w; px = rot.x; py = rot.y; pz = rot.z;

		w = pw * qw - px * qx - py * qy - pz * qz;
		x = pw * qx + px * qw + py * qz - pz * qy;
		y = pw * qy - px * qz + py * qw + pz * qx;
		z = pw * qz + px * qy - py * qx + pz * qw;

	}
	
	/*****************************************************************//**
	 * \brief �N�H�[�^�j�I�����m�̏�Z
	 * 
	 * \param rot0
	 * \param rot1
	 * \memo this = rot0 * rot1
	 *********************************************************************/
	void Multiply(const Quaternion& rot0, const Quaternion& rot1){
		float pw, px, py, pz;
		float qw, qx, qy, qz;

		qw = rot0.w; qx = rot0.x; qy = rot0.y; qz = rot0.z;
		pw = rot1.w; px = rot1.x; py = rot1.y; pz = rot1.z;

		w = pw * qw - px * qx - py * qy - pz * qz;
		x = pw * qx + px * qw + py * qz - pz * qy;
		y = pw * qy - px * qz + py * qw + pz * qx;
		z = pw * qz + px * qy - py * qx + pz * qw;
	}
	
	/*****************************************************************//**
	 * \brief �N�I�[�^�j�I���̑�����Z�q
	 * 
	 * \param rot0
	 * \return 
	 *********************************************************************/
	const Quaternion& operator*=(const Quaternion& rot0){
		Multiply(rot0, *this);
		return *this;
	}
	
	/*****************************************************************//**
	 * \brief �x�N�g���ɃN�H�[�^�j�I����K������
	 * \param _v
	 *********************************************************************/
	void Apply(Vector4& _v) const{
		DirectX::XMVECTOR xmv = DirectX::XMVector3Rotate(_v, *this);
		DirectX::XMStoreFloat4(&_v.vec, xmv);
	}

	/*****************************************************************//**
	 * \brief �x�N�g���ɃN�H�[�^�j�I����K������
	 * \param _v
	 *********************************************************************/
	void Apply(Vector3& _v) const{
		DirectX::XMVECTOR xmv = DirectX::XMVector3Rotate(_v, *this);
		DirectX::XMStoreFloat3(&_v.vec, xmv);
	}
};

//�@�����^�̃x�N�g���N���X
__declspec(align(16)) class Vector4i {
public:
	union {
		struct { int x, y, z, w; };
		int v[4];
	};
};


/*****************************************************************//**
 * \brief �x�N�g�����m�̉��Z
 * \param v0
 * \param v1
 * \return 
 *********************************************************************/
static inline Vector3 operator+(const Vector3& v0, const Vector3& v1){
	Vector3 result;
	result.Add(v0, v1);
	return result;
}

/*****************************************************************//**
 * \brief �x�N�g���ƃX�J���[�̏�Z
 * \param v
 * \param s
 * \return 
 *********************************************************************/
static inline Vector3 operator*(const Vector3& v, float s){
	Vector3 result;
	result = v;
	result.Scale(s);
	return result;
}

/*****************************************************************//**
 * \brief �x�N�g���ƃX�J���[�̏��Z
 * 
 * \param v
 * \param s
 * \return 
 *********************************************************************/
static inline Vector3 operator/(const Vector3& v, float s){
	Vector3 result;
	result = v;
	result.Div(s);
	return result;
}

/*****************************************************************//**
 * \brief �x�N�g�����m�̌��Z
 * \param v0
 * \param v1
 * \return 
 *********************************************************************/
static inline Vector3 operator-(const Vector3& v0, const Vector3& v1){
	Vector3 result;
	result.Subtract(v0, v1);
	return result;
}

/*****************************************************************//**
 * \brief �O�ς��v�Z
 * 
 * \param v0
 * \param v1
 * \return 
 *********************************************************************/
static inline Vector3 Cross(const Vector3& v0, const Vector3& v1){
	Vector3 result;
	result.Cross(v0, v1);
	return result;
}

/*****************************************************************//**
 * \brief ���ς��v�Z
 * 
 * \param v0
 * \param v1
 * \return 
 *********************************************************************/
static inline float Dot(const Vector3& v0, const Vector3& v1){
	return v0.Dot(v1);
}

/*****************************************************************//**
 * \brief �x�N�g�����m�̉��Z
 * 
 * \param v0
 * \param v1
 * \return 
 *********************************************************************/
static inline Vector4 operator+(const Vector4& v0, const Vector4& v1){
	Vector4 result;
	result.Add(v0, v1);
	return result;
}

/*****************************************************************//**
 * \brief �x�N�g���ƃX�J���[�̏�Z
 * 
 * \param v
 * \param s
 * \return 
 *********************************************************************/
static inline Vector4 operator*(const Vector4& v, float s){
	Vector4 result;
	result = v;
	result.Scale(s);
	return result;
}

/*****************************************************************//**
 * \brief �x�N�g�����m�̌��Z
 * 
 * \param v0
 * \param v1
 * \return 
 *********************************************************************/
static inline Vector4 operator-(const Vector4& v0, const Vector4& v1){
	Vector4 result;
	result.Subtract(v0, v1);
	return result;
}

/*****************************************************************//**
 * \brief ���ς��v�Z
 * 
 * \param v0
 * \param v1
 * \return 
 *********************************************************************/
static inline float Dot(const Vector4& v0, const Vector4& v1){
	return v0.Dot(v1);
}

/*****************************************************************//**
 * \brief �N�H�[�^�j�I�����m�̏�Z
 * 
 * \param q1
 * \param q2
 * \return 
 *********************************************************************/
static inline Quaternion operator*(const Quaternion& q1, const Quaternion q2){
	Quaternion qRet;
	qRet.Multiply(q2, q1);
	return qRet;
}

const Quaternion g_quatIdentity = { 0.0f,  0.0f, 0.0f, 1.0f };
