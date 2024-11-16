#pragma once
#include <set>

#include "Def.h"
#include "IElement.h"
#include "Node.h"

namespace GeomSolver
{
	template<class T>
	class QuadTree
	{

	public:
		QuadTree() = delete;

		/*
		* creation of Quadtree with given bonds and level of details.
		*/
		QuadTree(T left, T right, T top, T bottom, int levels = 8) :m_levels(levels)
		{
			T midX = (left + right) / static_cast<T>(2.0);
			T midY = (top + bottom) / static_cast<T>(2.0);
			if ((right - left) > (top - bottom))
			{
				T half = (right - left) / static_cast<T>(2.0);
				m_left = midX - half;
				m_right = midX + half;
				m_top = midY + half;
				m_bottom = midY - half;
			}
			else {
				T half = (top - bottom) / static_cast<T>(2.0);
				m_left = midX - half;
				m_right = midX + half;
				m_top = midY + half;
				m_bottom = midY - half;
			}
			m_root = new Node<T>(0, nullptr);
			m_root->left = m_left;
			m_root->right = m_right;
			m_root->top = m_top;
			m_root->bottom = m_bottom;

			createNodes(m_root, 1, m_levels);
		}

		~QuadTree()
		{
			if (m_root) clearNodes(m_root);
		}

		/*
		* Adding element to Quadtree via hierachical manner. starting from root element
		*/
		bool addElement(IElement<T>* elem)
		{
			if (m_root)
			{
				return m_root->addElement(elem);

			}
			return false;
		}

		Node<T>* findNodeAt(const RectCoordinate<T> pt)
		{
			if (m_root)
			{
				return m_root->findNodeAt(pt);

			}
			return nullptr;
		}
	private:
		void createNodes(Node<T>* root, Integer level, Integer maxLevel)
		{
			if (level > maxLevel)
				return;
			if (root->first == nullptr)
			{
				auto* temp = new Node<T>(level, root);
				temp->left = (root->left + root->right) / static_cast<T>(2);
				temp->right = root->right;
				temp->top = root->top;
				temp->bottom = (root->top + root->bottom) / static_cast<T>(2);
				createNodes(temp, level + 1, maxLevel);
				root->first = temp;
				//LOGD("Create first level %d \n",level);
			}
			if (root->second == nullptr)
			{
				auto* temp = new Node<T>(level, root);
				temp->left = root->left;
				temp->right = (root->left + root->right) / static_cast<T>(2);
				temp->top = root->top;
				temp->bottom = (root->top + root->bottom) / static_cast<T>(2);
				createNodes(temp, level + 1, maxLevel);
				root->second = temp;
				//LOGD("Create second level %d \n", level);
			}
			if (root->third == nullptr)
			{
				auto* temp = new Node<T>(level, root);
				temp->left = root->left;
				temp->right = (root->left + root->right) / static_cast<T>(2);
				temp->top = (root->top + root->bottom) / static_cast<T>(2);
				temp->bottom = root->bottom;
				createNodes(temp, level + 1, maxLevel);
				root->third = temp;
				//LOGD("Create third level %d \n", level);
			}
			if (root->forth == nullptr)
			{
				auto* temp = new Node<T>(level, root);
				temp->left = (root->left + root->right) / static_cast<T>(2);;
				temp->right = root->right;
				temp->top = (root->top + root->bottom) / static_cast<T>(2);
				temp->bottom = root->bottom;
				createNodes(temp, level + 1, maxLevel);
				root->forth = temp;
				//LOGD("Create fourth level %d \n", level);
			}
		}

		void clearNodes(Node<T>* root)
		{
			// clear children nodes
			if (root->first) clearNodes(root->first);
			if (root->second) clearNodes(root->second);
			if (root->third) clearNodes(root->third);
			if (root->forth) clearNodes(root->forth);

			delete root;
			root = nullptr;
		}

	private:
		T m_left;
		T m_right;
		T m_top;
		T m_bottom;
		Node<T>* m_root{ nullptr };
		int m_levels{ 1 };
	};
} // namespace GeomSolver

