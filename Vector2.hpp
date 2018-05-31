#ifndef VECTOR2_H
#define VECTOR2_H
#include <cmath>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "MathUtilities.hpp"
#include <string>
#include <iostream>

using namespace std;

class Vector2
{
	private:
		//MathUtilities mathHelper;
		static void ErrorMessage(string message);

	public:
		float x;
		float y;

	public:
		Vector2();
		Vector2(float x,float y);
		Vector2(const Vector2& source);
		~Vector2();
		void Normalize();
		void SetXY(float x, float y);
		void SetX(float x);
		void SetY(float y);
		float GetX();
		float GetY();
		void SetYaw(float rad);
		void SetYawDeg(float deg);
		void SetLengthYaw(float length,float rad);
		void SetLengthYawDeg(float length,float deg);
		void SetLength(float length);
		void Scale(float scale);
		void ScaleNonUniform(float xScale,float yScale);
		static Vector2 MultiplyComponents(const Vector2& vec1, const Vector2& vec2);
		void RotateDegree(float deg);
		void Rotate(float rad);
		void Rotate90Deg();
		void RotateMinus90Deg();
		void Reverse();
		float CalcLength();
		float CalcLengthSquare();
		float CalcYaw();
		float CalcYawDeg();
		Vector2 operator=(const Vector2& other);
		Vector2 operator+=(const Vector2& other);
		Vector2 operator-=(const Vector2& other);
		Vector2 operator*=(float scale);
		void operator/=(float scale);
		Vector2 operator+(const Vector2& other);
		Vector2 operator-(const Vector2& other);
		Vector2 operator-();
		friend Vector2 operator*(const Vector2& other,float scale);
		friend Vector2 operator/(const Vector2& other,float scale);
		friend bool operator==(const Vector2& vec1,const Vector2& vec2);
		friend bool operator!=(const Vector2& vec1,const Vector2& vec2);
		float DotProduct(const Vector2& other);
		static float DotProduct(const Vector2& vec1,const Vector2& vec2);
		friend Vector2 operator*(float scale,const Vector2& vec);
		void printVector();
};
#endif