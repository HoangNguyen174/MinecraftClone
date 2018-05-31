#ifndef AABB3_HEADER
#define AABB3_HEADER

#include "AABB2.hpp"
#include "Vector3.h"
#include "MathUtilities.hpp"

using namespace std;

class AABB3
{
	public:
		Vector3 m_maxPoint;
		Vector3 m_minPoint;

	public:
		AABB3();
		AABB3(Vector3 minPoint,Vector3 maxPoint);
		AABB3(const AABB3& other);
		~AABB3();
		void SetMinMax(Vector3 min, Vector3 max);
		void GenerateAABB3(vector<Vector3> list);
		bool IsPointInside(const Vector3& point);
		bool AABB3ToAABB3(const AABB3& otherBoundingBox);
		AABB3 operator=(const AABB3& otherBoundingBox);
};
#endif