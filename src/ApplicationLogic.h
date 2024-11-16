#pragma once
#include <map>
#include <string>


#include "Def.h"
#include "ElementFactory.h"
#include "QuadTree.h"


namespace GeomSolver
{
	class ApplicationLogic
	{
	public:
		ApplicationLogic(char* argv);
		~ApplicationLogic();
		void loadElements(std::string path);
		void checkObjectsAtPoint(const RectCoordinate<Decimal>& pt) const;
		void checkObjectsOverlaping() const;
	private:
		std::shared_ptr<ElementFactory<Decimal>> m_elementFactory;
		std::shared_ptr<QuadTree<Decimal>> m_quadTree;

		std::string m_dataPath{"./SampleData/"};
		std::map<ElementId,IElement<Decimal>*> m_elements;
	};
} // namespace GeomSolver

