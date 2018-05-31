#ifndef	VECTOR2INT_H
#define VECTOR2INT_H
#include <cmath>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "MathUtilities.hpp"
#include <string>
#include <iostream>

using namespace std;

class Vector2i
{
	private:
		MathUtilities mathHelper;
		static void ErrorMessage(string message);

	public:
		int x;
		int y;
		Vector2i();
		Vector2i(int x,int y);
		Vector2i(const Vector2i& source);
		~Vector2i();
		void Normalize();
		void SetXY(int x, int y);
		void SetX(int x);
		void SetY(int y);
		int GetX();
		int GetY();
		static Vector2i MultiplyComponents(const Vector2i& vec1, const Vector2i& vec2);
		Vector2i operator=(const Vector2i& other);
		Vector2i operator+=(const Vector2i& other);
		Vector2i operator-=(const Vector2i& other);
		Vector2i operator+(const Vector2i& other);
		Vector2i operator-(const Vector2i& other);
		Vector2i operator-();
		friend bool operator==(const Vector2i& vec1,const Vector2i& vec2);
		friend bool operator!=(const Vector2i& vec1,const Vector2i& vec2);
		friend bool operator<(const Vector2i& v1, const Vector2i& v2);
		void printVector();
};
#endif