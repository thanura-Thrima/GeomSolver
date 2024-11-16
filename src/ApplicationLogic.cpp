#include "ApplicationLogic.h"

#include <algorithm>
#include <filesystem>
#include <iostream>

#include "CsvParser.h"
#include "Node.h"

namespace fs = std::filesystem;

namespace GeomSolver
{

	ApplicationLogic::ApplicationLogic(char* argv):m_dataPath(std::string(argv))
		,m_elementFactory(ElementFactory<Decimal>::getFactory())
		,m_quadTree(std::make_shared<QuadTree<Decimal>>(c_quadTreeLimitLeft, c_quadTreeLimitRight, 
			c_quadTreeLimitTop, c_quadTreeLimitBottom, c_quadTreeLevels))
	{
		fs::path p = fs::absolute(m_dataPath);
		struct stat sb;

		// Looping until all the items of the directory are
		// exhausted
		for (const auto& entry : fs::directory_iterator(p)) {

			// Converting the path to const char * in the
			// subsequent lines
			std::filesystem::path outfilename = entry.path();
			std::string outfilename_str = outfilename.string();
			const char* path = outfilename_str.c_str();

			// Testing whether the path points to a
			// non-directory or not If it does, displays path
			if (stat(path, &sb) == 0 && !(sb.st_mode & S_IFDIR))
				LOGD("path %s \n", path);
			loadElements(std::string(path));
		}
	}
	ApplicationLogic::~ApplicationLogic()
	{
		for (auto it = m_elements.begin(); it != m_elements.end(); it++)
		{
			delete it->second;
		}
		m_elements.clear();
	}

	void ApplicationLogic::loadElements(std::string path)
	{
		auto rowArray = CsvParser::getRows<8>(path);
		for (int i = 1; i < rowArray.size(); i++)
		{
			LOGD("Read row %s %s %s %s %s %s %s %s \n", rowArray[i][0].c_str(), rowArray[i][1].c_str(), rowArray[i][2].c_str(), 
				rowArray[i][3].c_str(), rowArray[i][4].c_str(), rowArray[i][5].c_str(), rowArray[i][6].c_str() , rowArray[i][7].c_str());
			auto* element = m_elementFactory->createElementFromRawData(rowArray[i]);
			if (element == nullptr) continue;
			m_elements[element->getId()] = element;
			m_quadTree->addElement(element);
		}
	}

	void ApplicationLogic::checkObjectsAtPoint(const RectCoordinate<Decimal>& pt) const
	{
		LOGI("\nObjects at point (%f,%f): ",pt.x,pt.y);
		Node<Decimal>* currentNode = m_quadTree->findNodeAt(pt);
		if (currentNode == nullptr)
		{
			LOGE("No existing node foud for pt (%f,%f) \n", pt.x, pt.y);
			return;
		}
		std::vector<ElementId> elements;
		currentNode->findPotentialOverlappingElementsCurrentLevelAndDownwards(-1, elements);
		currentNode->findPotentialOverlappingElementsUpward(elements);
		for (auto itElmId = elements.begin(); itElmId != elements.end(); itElmId++)
		{
			//auto a = *itElmId;
			//auto b = itElmId;
			auto elmIt = m_elements.find(*itElmId);
			if (elmIt == m_elements.end()) continue;
			auto element = elmIt->second;
			if (element->isInterset(pt, 0.0005))
			{
				LOGI(" %s", element->getName().c_str());
			}
		}
		LOGI(" \n");
	}
	void ApplicationLogic::checkObjectsOverlaping() const
	{
		LOGI("\nCheck object overlap(s) :\n");
		for (auto it = m_elements.begin(); it != m_elements.end(); it++)
		{
			IElement<Decimal>* selectElement =  it->second;

			Node<Decimal>* currentNode = selectElement->getNode();

			// check overlapping
			std::vector<ElementId> elements;
			currentNode->findPotentialOverlappingElementsCurrentLevelAndDownwards(selectElement->getId(), elements);
			currentNode->findPotentialOverlappingElementsUpward(elements);
			LOGI(" %s is overlapping:",selectElement->getName().c_str());
			for (auto itElmId = elements.begin(); itElmId != elements.end(); itElmId++)
			{
				auto elmIt = m_elements.find(*itElmId);
				if (elmIt == m_elements.end()) { continue; }
				
				auto secondElement = elmIt->second;
				if (selectElement->isOverlap(secondElement, 0.0005))
				{
					LOGI(" %s", secondElement->getName().c_str());
				}
			}
			LOGI(" \n");
		}
	}
} // namespace GeomSolver
