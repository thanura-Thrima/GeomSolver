#pragma once
#include <array>
#include <string>
#include <vector>

#include "Def.h"
#include "RectCoordinate.h"

namespace GeomSolver
{
	template<class T>
	class Node;

	template<class T>
	class IElement
	{
	public:

		IElement() = delete;
		IElement(size_t id, std::string& name) :m_id(id), m_name(name) {};
		virtual ~IElement() = 0;
		virtual bool isOverlap(const IElement<T>* elm, const T effcielon = 0.005) = 0;
		virtual bool isInterset(const RectCoordinate<T>& coord, const T effcielon = 0.005) const = 0;
		virtual  T minDistance(const RectCoordinate<T>& coord) const = 0;
		virtual void projectionVectorGeneration() = 0;
		virtual bool separatingAxisTheoremSingleElement(const IElement<T>* element) const = 0;
		virtual RectCoordinate<T> project(RectCoordinate<T> axis) const = 0;
		ElementId getId() const { return m_id; };
		ElementType getType() const { return m_type; };
		std::string getName() const { return m_name; };
		const std::vector<RectCoordinate<T>>& getPts() const { return m_pts; };

		bool overlap(RectCoordinate<T> proj1, RectCoordinate<T> proj2) const
		{
			if (proj1.x > proj2.y || proj1.y < proj2.x)
				return false;
			return true;
		}

		void setNode(Node<T>* node) { m_nodePtr = node; };
		Node<T>* getNode() { return m_nodePtr; };

		bool isInside(const RectCoordinate<T>& pt, const T effcielon = 0.005) const
		{
			if ((m_bounds[0] - effcielon) < pt.x && (m_bounds[1] + effcielon) >= pt.x &&
				(m_bounds[2] + effcielon) >= pt.y && (m_bounds[3] - effcielon) < pt.y)
			{
				return true;
			}
			return false;
		}
	public:
		std::array<T, 4> m_bounds;
	protected:
		ElementId m_id;
		std::vector<RectCoordinate<T>> m_pts;
		std::vector<RectCoordinate<T>> m_projVectors;
		std::string m_name{ "" };
		ElementType m_type;
		Node<T>* m_nodePtr{ nullptr };
	};

	template<class T>
	IElement<T>::~IElement() {}
} // namespace GeomSolver

