#pragma once
#include <set>

#include "Def.h"
#include "IElement.h"

namespace GeomSolver
{
	template<class T>
	class Node {
		template<class T>
		friend class IElement;
	public:
		/*
		* These are the Node bounding box bounds
		*/
		T left;
		T right;
		T top;
		T bottom;
		std::set<ElementId> elements;
		/*
		* These are the 4 quadrant as bellow
		* :-------:
		* :	2 : 1 :
		* :___:___:
		* :   :   :
		* : 3 : 4 :
		* :-------:
		*/
		Node* first{ nullptr };
		Node* second{ nullptr };
		Node* third{ nullptr };
		Node* forth{ nullptr };

		Node* parent{ nullptr };

		Integer level{ -1 };
	public:
		Node(Integer lvl, Node* root) :parent(root), level(lvl)
		{
		}

		bool isInside(T x, T y)
		{
			if (left < x && right >= x && top >= y && bottom < y)
			{
				return true;
			}
			return false;
		}

		bool addElement(IElement<T>* elem)
		{
			bool res = false;
			// check whether element bounding box within the current Node.
			if (isInside(elem->m_bounds[0], elem->m_bounds[2]) && isInside(elem->m_bounds[1], elem->m_bounds[3]))
			{
				if (this->first && this->second && this->third && this->forth)
				{
					// check any of the children can contain element bounding box.
					res |= this->first->addElement(elem);
					res |= this->second->addElement(elem);
					res |= this->third->addElement(elem);
					res |= this->forth->addElement(elem);
				}

				// if non of the children can contain element add to this node
				if (!res)
				{
					this->elements.insert(elem->getId());
					elem->setNode(this);
					return true;
				}
			}
			return res;
		}

		Node<T>* findNodeAt(const RectCoordinate<T>& pt)
		{
			Node<T>* res = nullptr;
			if (isInside(pt.x, pt.y) && isInside(pt.x, pt.y))
			{
				if (this->first != nullptr && this->second != nullptr && this->third != nullptr && this->forth != nullptr)
				{
					// check any of the children can contain element bounding box.
					res = this->first->findNodeAt(pt);
					if (res != nullptr) return res;
					res = this->second->findNodeAt(pt);
					if (res != nullptr) return res;
					res = this->third->findNodeAt(pt);
					if (res != nullptr) return res;
					res = this->forth->findNodeAt(pt);
					if (res != nullptr) return res;
				}
				{
					return this;
				}
			} {
				return nullptr;
			}
		}

		void findPotentialOverlappingElementsCurrentLevelAndDownwards(ElementId id, std::vector<ElementId>& elementIds)
		{
			for (auto it = elements.begin(); it != elements.end(); it++)
			{
				if (*it != id)
				{
					elementIds.push_back(*it);
				}
			}

			if (this->first != nullptr && this->second != nullptr && this->third != nullptr && this->forth != nullptr)
			{
				// check any of the children Nodes has elements
				this->first->findPotentialOverlappingElementsCurrentLevelAndDownwards(id, elementIds);
				this->second->findPotentialOverlappingElementsCurrentLevelAndDownwards(id, elementIds);
				this->third->findPotentialOverlappingElementsCurrentLevelAndDownwards(id, elementIds);
				this->forth->findPotentialOverlappingElementsCurrentLevelAndDownwards(id, elementIds);
			}
		}

		void findPotentialOverlappingElementsUpward(std::vector<ElementId>& elementIds)
		{
			if (this->parent == nullptr)
				return;
			for (auto it = this->parent->elements.begin(); it != this->parent->elements.end(); it++)
			{
				elementIds.push_back(*it);
			}
			this->parent->findPotentialOverlappingElementsUpward(elementIds);
		}
	};

} // namespace NAPA
