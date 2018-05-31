#include "Vector2.hpp"

using namespace std;

//Empty Constructor
Vector2::Vector2()
{
	x = 0.f;
	y = 0.f;
}

//Explicit Constructor
Vector2::Vector2(float x,float y)
{
	this->x = x;
	this->y = y;
}

//copy constructor
Vector2::Vector2(const Vector2& source)
{
	this->x = source.x;
	this->y = source.y;
}

//empty deconstructor
Vector2::~Vector2(){}

//normalize vector
void Vector2::Normalize()
{
	this->x /= this->CalcLength();
	this->y /= this->CalcLength();
}

void Vector2::SetXY(float x,float y)
{
	this->x = x;
	this->y = y;
};

void Vector2::SetX(float x)
{
	this->x = x;
}

void Vector2::SetY(float y)
{
	this->y = y;
}

float Vector2::GetX()
{
	return this->x;
}

float Vector2::GetY()
{
	return this->y;
}

void Vector2::SetYaw(float rad)
{
	this->x = cos(rad);
	this->y = sin(rad);
}

void Vector2::SetYawDeg(float deg)
{
	this->x = cos(MathUtilities::DegToRad(deg));
	this->y = sin(MathUtilities::DegToRad(deg));
}

void Vector2::SetLengthYaw(float length,float rad)
{
	this->x = length * cos(rad);
	this->y = length * sin(rad);
}

void Vector2::SetLengthYawDeg(float length,float deg)
{
	this->x = length * cos(MathUtilities::DegToRad(deg));
	this->y = length * sin(MathUtilities::DegToRad(deg));
}

void Vector2::SetLength(float length)
{
	float theta = atan2(this->y,this->x);
	this->x = length * cos(theta);
	this->y = length * sin(theta);
}

float Vector2::CalcLength()
{
	return (sqrt((this->x)*(this->x) 
			    +(this->y)*(this->y)));
}

float Vector2::CalcLengthSquare()
{
	return ((this->x)*(this->x) 
		   +(this->y)*(this->y));
}

float Vector2::DotProduct(const Vector2& other)
{
	return (this->x*other.x+this->y*other.y);
}

float Vector2::DotProduct(const Vector2& vec1,const Vector2& vec2)
{
	return (vec1.x * vec2.x + vec1.y*vec2.y);
}

void Vector2::Scale(float scale)
{
	this->x *= scale;
	this->y *= scale;
}

void Vector2::ScaleNonUniform(float xScale,float yScale)
{
	this->x *= xScale;
	this->y *= yScale;
}

void Vector2::Rotate(float rad)
{
	float temp_x;
	temp_x = this->x;
	this->x = this->x * cos(rad) - this->y * sin(rad);
	this->y = temp_x * sin(rad) + this->y * cos(rad);
}

void Vector2::RotateDegree(float deg)
{
	float temp_x;
	temp_x = this->x;
	this->x = this->x * cos(MathUtilities::DegToRad(deg)) - this->y * sin(MathUtilities::DegToRad(deg));
	this->y = temp_x * sin(MathUtilities::DegToRad(deg)) + this->y * cos(MathUtilities::DegToRad(deg));
}

void Vector2::Rotate90Deg()
{
	float temp;
	temp = this->x;
	this->x = -this->y;
	this->y = temp;
}

void Vector2::RotateMinus90Deg()
{
	float temp;
	temp = this->x;
	this->x = this->y;
	this->y = -temp;
}

void Vector2::Reverse()
{
	this->x = -this->x;
	this->y = -this->y;
}

Vector2 Vector2::operator=(const Vector2& other)
{
	this->x = other.x;
	this->y = other.y;

	return *this;
}

Vector2 Vector2::operator+=(const Vector2& other)
{
	this->x += other.x;
	this->y += other.y;
	return *this;
}

Vector2 Vector2::operator-=(const Vector2& other)
{
	this->x -= other.x;
	this->y -= other.y;
	return *this;
}

Vector2 Vector2::operator*=(float scale)
{
	this->x *= scale;
	this->y *= scale;
	return *this;
}

Vector2 Vector2::operator+(const Vector2& other)
{
	Vector2 result;
	result.x = this->x + other.x;
	result.y = this->y + other.y;

	return result;
}

Vector2 Vector2::operator-(const Vector2& other)
{
	Vector2 result;
	result.x = this->x - other.x;
	result.y = this->y - other.y;

	return result;
}

Vector2 Vector2::operator-()
{
	Vector2 result;
	result.x = -this->x;
	result.y = -this->y;
	return result;
}

void Vector2::operator/=(float scale)
{
	if(scale == 0)
	{
		ErrorMessage("Cannot divide by zero!");
		exit(0);
	}
	this->x /= scale;
	this->y /= scale;
}

Vector2 operator/(const Vector2& other,float scale)
{
	Vector2 result;
	if(scale == 0)
	{
		Vector2::ErrorMessage("Cannot divide by zero!");
		exit(0);
	}
	result.x = other.x/scale;
	result.y = other.y/scale;
	return result;
}

Vector2 operator*(const Vector2& other,float scale)
{
	Vector2 result;
	result.x = other.x*scale;
	result.y = other.y*scale;
	return result;
}

bool operator==(const Vector2& vec1, const Vector2& vec2)
{
	return ((vec1.x == vec2.x) && (vec1.y == vec2.y));
}

bool operator!=(const Vector2& vec1, const Vector2& vec2)
{
	return ((vec1.x != vec2.x) || (vec1.y != vec2.y));
}

float Vector2::CalcYaw()
{
	return atan2(this->y,this->x);
}

float Vector2::CalcYawDeg()
{
	return MathUtilities::RadToDeg(atan2(this->y,this->x));
}

Vector2 operator*(float scale, const Vector2& vector)
{
	Vector2 result;
	result.x = scale * vector.x;
	result.y = scale * vector.y;
	return result;
}

Vector2 Vector2::MultiplyComponents(const Vector2& vec1, const Vector2& vec2)
{
	Vector2 result;
	result.x = vec1.x * vec2.x;
	result.y = vec1.y * vec2.y;
	return result;
}

void Vector2::ErrorMessage(string message)
{
	cout << message << endl;
}

void Vector2::printVector()
{
	cout << "X: " << this->x << "  Y: " << this->y << endl;
}