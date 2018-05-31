#include "AABB3.hpp"

AABB3::AABB3()
{
	m_minPoint.SetXYZ(0.f,0.f,0.f);
	m_maxPoint.SetXYZ(0.f,0.f,0.f);
}

AABB3::AABB3(Vector3 minPoint,Vector3 maxPoint)
{
	m_minPoint = m_minPoint;
	m_maxPoint = m_maxPoint;
}

AABB3::AABB3(const AABB3& other)
{
	m_minPoint = other.m_minPoint;
	m_maxPoint = other.m_maxPoint;
}

AABB3::~AABB3()
{


}

void AABB3::SetMinMax(Vector3 min, Vector3 max)
{
	m_minPoint = min;
	m_maxPoint = max;
}

void AABB3::GenerateAABB3(vector<Vector3> list)
{
	vector<float> x_list;
	vector<float> y_list;
	vector<float> z_list;
	for(auto i = 0; i < list.size();i++)
	{
		x_list.push_back(list[i].GetX());
		y_list.push_back(list[i].GetY());
		z_list.push_back(list[i].GetZ());
	}
	m_minPoint.x = MathUtilities::GetMin(x_list);
	m_minPoint.y = MathUtilities::GetMin(y_list);
	m_minPoint.z = MathUtilities::GetMin(z_list);
	
	m_maxPoint.x = MathUtilities::GetMax(x_list);
	m_maxPoint.y = MathUtilities::GetMax(y_list);
	m_maxPoint.z = MathUtilities::GetMax(z_list);
}

bool AABB3::AABB3ToAABB3(const AABB3& otherBoundingBox)
{
	return(this->m_maxPoint.x > otherBoundingBox.m_minPoint.x &&
		   this->m_minPoint.x < otherBoundingBox.m_maxPoint.x &&
		   this->m_maxPoint.y > otherBoundingBox.m_minPoint.y &&
		   this->m_minPoint.y < otherBoundingBox.m_maxPoint.y &&
		   this->m_maxPoint.z > otherBoundingBox.m_minPoint.z &&
		   this->m_minPoint.z < otherBoundingBox.m_maxPoint.z);
}

bool AABB3::IsPointInside(const Vector3& point)
{
	if(point.x > m_minPoint.x && point.x < m_maxPoint.x &&
	   point.y > m_minPoint.y && point.y < m_maxPoint.y &&
	   point.z > m_minPoint.z && point.z < m_maxPoint.z)
			return true;
	else
		return false;
}

AABB3 AABB3::operator=(const AABB3& other)
{
	this->m_minPoint = other.m_minPoint;
	this->m_maxPoint = other.m_maxPoint;

	return *this;
}