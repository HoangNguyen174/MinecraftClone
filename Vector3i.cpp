#include "Vector3i.hpp"

Vector3i::Vector3i()
{
	x = 0;
	y = 0;
	z = 0;
}

Vector3i::Vector3i(int x, int y, int z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector3i::Vector3i(const Vector3i& other)
{
	x = other.x;
	y = other.y;
	z = other.z;
}

Vector3i::~Vector3i()
{
}

float Vector3i::CalcLength()
{
	return ( sqrt( static_cast<float> ( (this->x) * (this->x) + (this->y) * (this->y) + (this->z) * (this->z) ) ) );
}

int Vector3i::CalcLengthSquare()
{
	return ((this->x)*(this->x) +(this->y)*(this->y)+(this->z)*(this->z));
}

int Vector3i::DotProduct(const Vector3i& other)
{
	return ( x * other.x + y * other.y + z * other.z );
}

void Vector3i::Reverse()
{
	x = -x;
	y = -y;
	z = -z;
}

Vector3i Vector3i::operator=(const Vector3i& other)
{
	x = other.x;
	y = other.y;
	z = other.z;
	return *this;
}

Vector3i Vector3i::operator-(const Vector3i& other)
{
	Vector3i temp;
	temp.x = x - other.x;
	temp.y = y - other.y;
	temp.z = z - other.z;
	return temp;
}

Vector3i Vector3i::operator+(const Vector3i& other)
{
	Vector3i temp;
	temp.x = x + other.x;
	temp.y = y + other.y;
	temp.z = z + other.z;
	return temp;
}

Vector3i Vector3i::operator+=(const Vector3i& other)
{
	x += other.x;
	y += other.y;
	z += other.z;

	return *this;
}

void Vector3i::operator-=(const Vector3i& other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
}

Vector3i Vector3i::operator-()
{
	Vector3i temp;
	temp.x = -x;
	temp.y = -y;
	temp.z = -z;
	return temp;
}

Vector3i Vector3i::operator*(int scale)
{
	Vector3i temp;
	temp.x = x*scale;
	temp.y = y*scale;
	temp.z = z*scale;
	return temp;
}

bool operator==(const Vector3i& v1,const Vector3i& v2)
{
	if(v1.x == v2.x && v1.y == v2.y && v1.z == v2.z )
		return true;
	return false;
}

bool operator!=(const Vector3i& v1,const Vector3i& v2)
{
	if(v1.x != v2.x || v1.y != v2.y || v1.z != v2.z )
		return true;
	return false;
}
