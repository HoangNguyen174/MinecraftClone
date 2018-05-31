#include "Vector3.h"

Vector3::Vector3()
{
	x = 0.f;
	y = 0.f;
	z = 0.f;
}

Vector3::Vector3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector3::Vector3(const Vector3& other)
{
	x = other.x;
	y = other.y;
	z = other.z;
}

Vector3::~Vector3()
{
}

void Vector3::Normalize()
{
	float length = CalcLength();
	if(length != 0)
	{
		x = x/length;
		y = y/length;
		z = z/length;
	}
}

float Vector3::CalcLength()
{
	return (sqrt((this->x)*(this->x) 
		+(this->y)*(this->y)
		+(this->z)*(this->z)));
}

float Vector3::CalcLengthSquare()
{
	return ((this->x)*(this->x) +(this->y)*(this->y)+(this->z)*(this->z));
}

float Vector3::DotProduct(const Vector3& other)
{
	return (x*other.x + y*other.y + z*other.z);
}

Vector3 Vector3::CrossProduct(const Vector3& vec1)
{
	Vector3 normal;
	normal.x = this->y*vec1.z - vec1.y*this->z;
	normal.y = this->z*vec1.x - vec1.z*this->x;
	normal.z = this->x*vec1.y - vec1.x*this->y;
	return normal;
}

void Vector3::Reverse()
{
	x = -x;
	y = -y;
	z = -z;
}

Vector3 Vector3::operator=(const Vector3& other)
{
	x = other.x;
	y = other.y;
	z = other.z;
	return *this;
}

Vector3 Vector3::operator-(const Vector3& other)
{
	Vector3 temp;
	temp.x = x - other.x;
	temp.y = y - other.y;
	temp.z = z - other.z;
	return temp;
}

Vector3 Vector3::operator+(const Vector3& other)
{
	Vector3 temp;
	temp.x = x + other.x;
	temp.y = y + other.y;
	temp.z = z + other.z;
	return temp;
}

Vector3 Vector3::operator+=(const Vector3& other)
{
	x += other.x;
	y += other.y;
	z += other.z;

	return *this;
}

void Vector3::operator-=(const Vector3& other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
}

Vector3 Vector3::operator-()
{
	Vector3 temp;
	temp.x = -x;
	temp.y = -y;
	temp.z = -z;
	return temp;
}

Vector3 Vector3::operator*(float scale)
{
	Vector3 temp;
	temp.x = x*scale;
	temp.y = y*scale;
	temp.z = z*scale;
	return temp;
}

void Vector3::RotateAboutYAxisDegree(float degree)
{
	float radian = MathUtilities::DegToRad(degree);
	float tempX = x;
	x = x * cos(radian) + z * sin(radian);
	z = tempX * ( -sin(radian)) + z * cos(radian);
}

bool operator==(const Vector3& v1,const Vector3& v2)
{
	if(v1.x == v2.x && v1.y == v2.y && v1.z == v2.z )
		return true;
	return false;
}

bool operator!=(const Vector3& v1,const Vector3& v2)
{
	if(v1.x != v2.x || v1.y != v2.y || v1.z != v2.z )
		return true;
	return false;
}

bool operator<(const Vector3& v1, const Vector3& v2)
{
	if(v1.x < v2.x)
	{
		return true;
	}
	else if(v1.x > v2.x)
	{
		return false;
	}
	else 
	{
		if(v1.y < v2.y)
			return true;
		else if(v1.y > v2.y)
			return false;
		else
		{
			if(v1.z > v2.z)
				return true;
			else
				return false;
		}
	}
}