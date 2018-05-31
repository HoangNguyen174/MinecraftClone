#ifndef AABB2_H
#define AABB2_H
#include "Vector2.hpp"
#include "MathUtilities.hpp"
#include <vector>
#include <math.h>

using namespace std;

class AABB2
{
	private:
		MathUtilities mathHelper;
	public:
		Vector2 v_upper_left;
		Vector2 v_lower_right;
		AABB2();
		~AABB2();
		void GenerateAABB2(vector<Vector2> pointsList);
		bool IsCollide(const AABB2& other);
		bool IsPointInSideAABB2(Vector2 point);
};
#endif