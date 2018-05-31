#ifndef VECTOR3_HEADER
#define VECTOR3_HEADER
#include <math.h>
#include "MathUtilities.hpp"

class Vector3
{
	private:	
	public:
		float x;
		float y;
		float z;
	public:
		Vector3();
		Vector3(float x, float y, float z);
		Vector3(const Vector3& other);
		~Vector3();
		void Normalize();
		void SetZ(float z) { this->z = z; }
		float GetX() { return x; }
		float GetY() { return y; }
		float GetZ() { return z; }
		void SetXYZ(float x, float y, float z) { this->x = x;this->y = y;this->z = z;}
		float CalcLength();
		float CalcLengthSquare();
		float DotProduct(const Vector3& other);
		Vector3 CrossProduct(const Vector3& other);
		void Scale(float scale);
		void ScaleNonUniform(float xScale, float yScale, float zScale);
		void RotateAboutYAxisDegree(float degree);
		void RotateRadian(float rad);
		void RotateDegree(float degree);
		void Rotate90Deg();
		void RotateMinus90Deg();
		void Reverse();
		Vector3 operator=(const Vector3& other);
		Vector3 operator-(const Vector3& other);
		Vector3 operator+(const Vector3& other);
		Vector3 operator+=(const Vector3& other);
		void operator-=(const Vector3& other);
		Vector3 operator*(const float scale);
		Vector3 operator-();
		friend bool operator==(const Vector3& v1,const Vector3& v2);
		friend bool operator!=(const Vector3& v1,const Vector3& v2);
		friend bool operator<(const Vector3& v1,const Vector3& v2);
		/*
		Vector3 operator*=(const Vector3& other);
		
		Vector3 operator/=(float scale);
		friend Vector3 operator/(const Vector3& other, float scale);*/
		
};
#endif