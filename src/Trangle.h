#pragma once
#include "IElement.h"

#include <cmath>
#include <utility>

namespace GeomSolver
{
	template<class T>
	class Triangle :
		public IElement<T>
	{
	public:
		template<class... V>
		Triangle(long long id, std::string& name, const std::tuple<V...>& arg) :IElement<T>(id, name)
		{
			p1.x = std::get<0>(arg);
			p1.y = std::get<1>(arg);;
			p2.x = std::get<2>(arg);
			p2.y = std::get<3>(arg);
			p3.x = std::get<4>(arg);
			p3.y = std::get<5>(arg);
			IElement<T>::m_pts.push_back(p1);
			IElement<T>::m_pts.push_back(p2);
			IElement<T>::m_pts.push_back(p3);

			IElement<T>::m_bounds[0] = static_cast<T>(std::min(std::min(p1.x, p2.x), p3.x));
			IElement<T>::m_bounds[1] = static_cast<T>(std::max(std::max(p1.x, p2.x), p3.x));
			IElement<T>::m_bounds[2] = static_cast<T>(std::max(std::max(p1.y, p2.y), p3.y));
			IElement<T>::m_bounds[3] = static_cast<T>(std::min(std::min(p1.y, p2.y), p3.y));
			IElement<T>::m_type = ElementType::Triangle;
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
			auto minDist = minDistance(coord);
			if (IElement<T>::isInside(coord, effcielon))
			{
				if (abs(minDist) < effcielon)
					return true;
			}
			return false;
		};
		T minDistance(const RectCoordinate<T>& coord) const override
		{
			auto d1 = RectCoordinate<T>::ditanceToLine(p1, p2, coord);
			auto d2 = RectCoordinate<T>::ditanceToLine(p1, p3, coord);
			auto d3 = RectCoordinate<T>::ditanceToLine(p2, p3, coord);
			return std::min(std::min(d1, d2), d3);
		};

		void projectionVectorGeneration() override
		{
			// generate projections
			auto p1p2 = p2 - p1;
			auto axisp1p2 = RectCoordinate<T>(-1 * p1p2.y, p1p2.x);
			T norm = axisp1p2.norm();
			axisp1p2.x = axisp1p2.x / norm;
			axisp1p2.y = axisp1p2.y / norm;

			auto p1p3 = p3 - p1;
			auto axisp1p3 = RectCoordinate<T>(-1 * p1p3.y, p1p3.x);
			norm = axisp1p3.norm();
			axisp1p3.x = axisp1p3.x / norm;
			axisp1p3.y = axisp1p3.y / norm;

			auto p2p3 = p3 - p2;
			auto axisp2p3 = RectCoordinate<T>(-1 * p2p3.y, p2p3.x);
			norm = axisp2p3.norm();
			axisp2p3.x = axisp2p3.x / norm;
			axisp2p3.y = axisp2p3.y / norm;

			IElement<T>::m_projVectors.push_back(axisp1p2);
			IElement<T>::m_projVectors.push_back(axisp1p3);
			IElement<T>::m_projVectors.push_back(axisp2p3);
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
				// NOTE: the axis must be normalized to get accurate projections
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
		RectCoordinate<T> p3;
	};

} // namespace GeomSolver

