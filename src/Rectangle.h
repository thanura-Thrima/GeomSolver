#pragma once

#include "IElement.h"

#include <cmath>
#include <string>


namespace GeomSolver
{
	template<class T>
	class Rectangle :public IElement<T>
	{
	public:
		template<class... V>
		Rectangle(long long id, std::string& name, const std::tuple<V...>& arg) :IElement<T>(id, name)
		{
			p1.x = std::get<0>(arg);
			p1.y = std::get<1>(arg);;
			p2.x = std::get<2>(arg);
			p2.y = std::get<3>(arg);
			IElement<T>::m_pts.push_back(p1);
			IElement<T>::m_pts.push_back(RectCoordinate<T>(p1.x, p2.y));
			IElement<T>::m_pts.push_back(p2);
			IElement<T>::m_pts.push_back(RectCoordinate<T>(p2.x, p1.y));

			IElement<T>::m_bounds[0] = static_cast<T>(std::get<0>(arg));
			IElement<T>::m_bounds[1] = static_cast<T>(std::get<2>(arg));
			IElement<T>::m_bounds[2] = static_cast<T>(std::get<1>(arg));
			IElement<T>::m_bounds[3] = static_cast<T>(std::get<3>(arg));
			IElement<T>::m_type = ElementType::Rectangle;
			projectionVectorGeneration();
		}
		bool isOverlap(const IElement<T>* elm, const T effcielon = 0.005) override
		{
			if (elm == nullptr) return false;

			switch (elm->getType())
			{
			case ElementType::Rectangle:
			case ElementType::Triangle:
			case ElementType::Circle:
			{
				if (!separatingAxisTheoremSingleElement(elm))
				{
					return false;
				}
				if (!elm->separatingAxisTheoremSingleElement(this))
				{
					return false;
				}
				return true;
			}
			default:
			{
				LOGE("Unknown ElementType %d in %s", static_cast<int>(elm->getType()), elm->getName().c_str());
			}
			}
			return false;
		};
		bool isInterset(const RectCoordinate<T>& coord, const T effcielon = 0.005) const override
		{
			if (IElement<T>::isInside(coord, effcielon))
			{
				T minDist = minDistance(coord);
				if (abs(minDist) < effcielon)
					return true;
			}

			return false;
		};

		T minDistance(const RectCoordinate<T>& coord) const override
		{
			auto temp1 = RectCoordinate<T>(p1.x, p2.y);
			auto temp2 = RectCoordinate<T>(p2.x, p1.y);
			auto d1 = RectCoordinate<T>::ditanceToLine(p1, temp1, coord);
			auto d2 = RectCoordinate<T>::ditanceToLine(p1, temp2, coord);
			auto d3 = RectCoordinate<T>::ditanceToLine(p2, temp1, coord);
			auto d4 = RectCoordinate<T>::ditanceToLine(p2, temp1, coord);
			return std::min(std::min(d1, d2), std::min(d3, d4));
		};

		void projectionVectorGeneration() override
		{
			auto axis1 = RectCoordinate<T>(1, 0);
			auto axis2 = RectCoordinate<T>(0, 1);

			IElement<T>::m_projVectors.push_back(axis1);
			IElement<T>::m_projVectors.push_back(axis2);
		}
		bool separatingAxisTheoremSingleElement(const IElement<T>* element) const override
		{
			for (int i = 0; i < IElement<T>::m_projVectors.size(); i++) {
				auto axis = IElement<T>::m_projVectors[i];
				// project both shapes onto the axis
				auto proj1 = this->project(axis);
				auto proj2 = element->project(axis);
				// do the projections overlap?
				if (!IElement<T>::overlap(proj1, proj2)) {
					return false;
				}
			}
			return true;
		}

		RectCoordinate<T> project(RectCoordinate<T> axis) const override
		{
			T min = axis.dot(p1);
			T max = min;
			for (size_t i = 1; i < IElement<T>::m_pts.size(); i++) {
				T p = axis.dot(IElement<T>::m_pts[i]);
				if (p < min) {
					min = p;
				}
				else if (p > max) {
					max = p;
				}
			}
			return RectCoordinate<T>(min, max);
		}
	private:
		RectCoordinate<T> p1;
		RectCoordinate<T> p2;
	};
} // namespace GeomSolver

