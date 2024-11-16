#pragma once
#include "ICoordinate.h"

#include <cmath>

namespace GeomSolver
{
	template<class T>
	class RectCoordinate : public ICoordinate<T>
	{
	public:
		RectCoordinate() :ICoordinate<T>()
			, x(static_cast<T>(nan("NAN")))
			, y(static_cast<T>(nan("NAN"))) {}
		RectCoordinate(T _x, T _y) :ICoordinate<T>(), x(_x), y(_y) {}
		~RectCoordinate() {}
		RectCoordinate(const RectCoordinate& cord)
		{
			this->x = cord.x;
			this->y = cord.y;
		}
		RectCoordinate(RectCoordinate&& cord)
		{
			this->x = cord.x;
			this->y = cord.y;
		}

		RectCoordinate& operator=(RectCoordinate& cord)
		{
			if (this == &cord)
				return *this;

			this->x = cord.x;
			this->y = cord.y;
			return *this;
		}

		RectCoordinate& operator+=(const RectCoordinate& rhs)
		{
			this->x += rhs.x;
			this->y += rhs.y;
			return *this;
		}
		friend RectCoordinate operator+(RectCoordinate lhs, RectCoordinate rhs)
		{
			lhs += rhs;
			return lhs;
		}

		RectCoordinate& operator-=(const RectCoordinate rhs)
		{
			this->x -= rhs.x;
			this->y -= rhs.y;
			return *this;
		}
		friend RectCoordinate operator-(RectCoordinate lhs, RectCoordinate rhs)
		{
			lhs -= rhs;
			return lhs;
		}
		T norm()
		{
			return sqrt((this->x * this->x) + (this->y * this->y));
		}
		static T ditanceToLine(const RectCoordinate& linePt1, const RectCoordinate& linePt2, const RectCoordinate& extPt)
		{
			auto ptTemp = linePt2 - linePt1;

			return abs(ptTemp.y * extPt.x - ptTemp.x * extPt.y + linePt2.x * linePt1.y - linePt2.y * linePt1.x) / ptTemp.norm();
		}
		static T distanceToPoint(const RectCoordinate& pt1, const RectCoordinate& pt2)
		{
			auto ptTemp = pt1 - pt2;
			return ptTemp.norm();
		}

		static RectCoordinate intersectionLines(const RectCoordinate& l1Pt1, const RectCoordinate& l1Pt2, const RectCoordinate& l2Pt1,
			const RectCoordinate& l2Pt2, T effcilon = static_cast<T>(0.00005))
		{
			RectCoordinate res;
			auto tmp1 = l1Pt1 - l1Pt2;
			auto tmp2 = l2Pt1 - l2Pt2;
			auto denom = tmp1.x * tmp2.y - tmp1.y * tmp2.x;
			if (abs(denom) < effcilon)
			{
				return res;
			}
			res.x = ((l1Pt1.x * l1Pt2.y - l1Pt1.y * l1Pt2.x) * tmp2.x - tmp1.x * (l2Pt1.x * l2Pt2.y - l2Pt1.y * l2Pt2.x)) / denom;
			res.y = ((l1Pt1.x * l1Pt2.y - l1Pt1.y * l1Pt2.x) * tmp2.y - tmp1.y * (l2Pt1.x * l2Pt2.y - l2Pt1.y * l2Pt2.x)) / denom;
			return res;
		}

		T dot(RectCoordinate<T> other)
		{
			return x * other.x + y * other.y;
		}

	public:
		T x;
		T y;
	};
} // namespace GeomSolver

