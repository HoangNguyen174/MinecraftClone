#include "Vector2i.hpp"

//Empty Constructor
Vector2i::Vector2i()
{
	x = 0;
	y = 0;
}

//Explicit Constructor
Vector2i::Vector2i(int x,int y)
{
	this->x = x;
	this->y = y;
}

//copy constructor
Vector2i::Vector2i(const Vector2i& source)
{
	this->x = source.x;
	this->y = source.y;
}

//empty deconstruction
Vector2i::~Vector2i(){}


void Vector2i::SetXY(int x,int y)
{
	this->x = x;
	this->y = y;
};

void Vector2i::SetX(int x)
{
	this->x = x;
}

void Vector2i::SetY(int y)
{
	this->y = y;
}

int Vector2i::GetX()
{
	return this->x;
}

int Vector2i::GetY()
{
	return this->y;
}

Vector2i Vector2i::operator=(const Vector2i& other)
{
	this->x = other.x;
	this->y = other.y;

	return *this;
}

Vector2i Vector2i::operator+=(const Vector2i& other)
{
	this->x += other.x;
	this->y += other.y;
	return *this;
}

Vector2i Vector2i::operator-=(const Vector2i& other)
{
	this->x -= other.x;
	this->y -= other.y;
	return *this;
}

Vector2i Vector2i::operator+(const Vector2i& other)
{
	Vector2i result;
	result.x = this->x + other.x;
	result.y = this->y + other.y;

	return result;
}

Vector2i Vector2i::operator-(const Vector2i& other)
{
	Vector2i result;
	result.x = this->x - other.x;
	result.y = this->y - other.y;

	return result;
}

Vector2i Vector2i::operator-()
{
	Vector2i result;
	result.x = -this->x;
	result.y = -this->y;
	return result;
}

bool operator==(const Vector2i& vec1, const Vector2i& vec2)
{
	return ((vec1.x == vec2.x) && (vec1.y == vec2.y));
}

bool operator!=(const Vector2i& vec1, const Vector2i& vec2)
{
	return ((vec1.x != vec2.x) || (vec1.y != vec2.y));
}

bool operator<(const Vector2i& v1, const Vector2i& v2)
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
		else
			return false;
	}
}

void Vector2i::ErrorMessage(string message)
{
	cout << message << endl;
}

void Vector2i::printVector()
{
	cout << "X: " << this->x << "  Y: " << this->y << endl;
}