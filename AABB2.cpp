#include "AABB2.hpp"

AABB2::AABB2()
{
	v_upper_left.SetXY(0.f,0.f);
	v_lower_right.SetXY(0.f,0.f);
}

AABB2::~AABB2()
{
}

void AABB2::GenerateAABB2(vector<Vector2> list)
{
	vector<float> x_list;
	vector<float> y_list;
	for(auto i = 0; i < list.size();i++)
	{
		x_list.push_back(list[i].GetX());
		y_list.push_back(list[i].GetY());
	}
	v_upper_left.x = mathHelper.GetMin(x_list);
	v_upper_left.y = mathHelper.GetMax(y_list);
	v_lower_right.x = mathHelper.GetMax(x_list);
	v_lower_right.y = mathHelper.GetMin(y_list);
}

bool AABB2::IsCollide(const AABB2& other)
{
	return !(this->v_upper_left.x > other.v_lower_right.x
		   ||this->v_lower_right.x < other.v_upper_left.x
		   ||this->v_upper_left.y < other.v_lower_right.y
		   ||this->v_lower_right.y > other.v_upper_left.y);
}

