#pragma once
#include<DirectXMath.h>

constexpr double PI = 3.1415926535;//�~����

/*****************************************************************//**
 * \brief ���W�A���ɕϊ�
 * \param radius �x���@�̊p
 * \return �ϊ������p
 *********************************************************************/
inline float ToRad(float radius) {
	float rad = radius * PI / 180;

	return rad;
}


//=============================================================================
// MathClass
//=============================================================================
class Math {
public:
	//�~����
	static constexpr float PI = 3.14159265358979323846f;
	//�~�����̂Q��
	static constexpr float PI2 = PI * 2.0f;

	/*****************************************************************//**
	 * \brief �p�x��Degree����Radian�ɕϊ�
	 * 
	 * \param deg Degree�p(�x���@)
	 * \return Radian�p
	 *********************************************************************/
	constexpr static inline float DegToRad(float deg){
		return deg * (PI / 180.0f);
	}

	/*****************************************************************//**
	 * \brief �p�x��radian����Degree�ɕϊ�
	 * 
	 * \param rad Radian�p
	 * \return Degree�p 
	 *********************************************************************/
	constexpr static inline float RadToDeg(float rad){
		return rad / (PI / 180.0f);
	}
	
	/*****************************************************************//**
	 * \brief T1��T2�̊Ԃ���`�ۊ�
	 * 
	 * \param rate	�ϊ���(0.0�`1.0)
	 * \param t0	�⊮�J�n�̒l
	 * \param t1	�⊮�I���̒l
	 * \return �⊮���ꂽ�l
	 *********************************************************************/
	template<class T> static inline T Lerp(float rate, T t0, T t1){
		T ret;
		ret.Lerp(rate, t0, t1);
		return ret;
	}

	/*****************************************************************//**
	 * \brief T1��T2�̊Ԃ���`�ۊ�
	 *
	 * \param rate	�ϊ���(0.0�`1.0)
	 * \param t0	�⊮�J�n�̒l
	 * \param t1	�⊮�I���̒l
	 * \return �⊮���ꂽ�l
	 *********************************************************************/
	template<> static inline float Lerp(float rate, float t0, float t1){
		return t0 + (t1 - t0) * rate;
	}
};
