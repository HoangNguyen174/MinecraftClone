#ifndef VECTOR3I_HEADER
#define VECTOR3I_HEADER
#include <math.h>


class Vector3i
{
	private:	
	public:
		int x;
		int y;
		int z;
	public:
		Vector3i();
		Vector3i(int x, int y, int z);
		Vector3i(const Vector3i& other);
		~Vector3i();
		void SetZ(int z) { this->z = z; }
		void SetY(int y) { this->y = y; }
		void SetX(int x) { this->x = x; }
		int GetX() { return x; }
		int GetY() { return y; }
		int GetZ() { return z; }
		void SetXYZ(int x, int y, int z) { this->x = x;this->y = y;this->z = z;}
		float CalcLength();
		int CalcLengthSquare();
		int DotProduct(const Vector3i& other);
		void Scale(int scale);
		void ScaleNonUniform(int xScale, int yScale, int zScale);
		void RotateRadian(float rad);
		void RotateDegree(float degree);
		void Rotate90Deg();
		void RotateMinus90Deg();
		void Reverse();
		Vector3i operator=(const Vector3i& other);
		Vector3i operator-(const Vector3i& other);
		Vector3i operator+(const Vector3i& other);
		Vector3i operator+=(const Vector3i& other);
		void operator-=(const Vector3i& other);
		Vector3i operator*(const int scale);
		Vector3i operator-();
		friend bool operator==(const Vector3i& v1,const Vector3i& v2);
		friend bool operator!=(const Vector3i& v1,const Vector3i& v2);
		
};
#endif