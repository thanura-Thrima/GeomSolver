#include "Application.h"

#include <sstream>

#include "ApplicationLogic.h"
#include "Def.h"

namespace GeomSolver
{
	Application::Application(char* arg) :m_ApplicationLogic(std::make_unique<ApplicationLogic>(arg)) {}

	void Application::run(int argCount, char** args) {
		std::string command(args[2]);
		if (command.compare("OBJECTS_AT_POINT") == 0)
		{
			if (argCount < 4)
			{
				LOGE("Not enough args for OBJECTS_AT_POINT command.");
				return;
			}
			std::string pt(args[3]);
			pt = pt.substr(1, pt.length() - 2);
			std::istringstream ss(std::move(pt));
			std::array<std::string, 10> row;
			Decimal val1, val2;

			try {
				int i = 0;
				for (std::string value; std::getline(ss, value, ',');)
				{
					row[i] = std::move(value);
					i++;
				}
				val1 = std::atof(row[0].c_str());
				val2 = std::atof(row[1].c_str());
			}
			catch (std::exception& e) {
				LOGE("Error loading pt %s", pt.c_str());
				return;
			}
			m_ApplicationLogic->checkObjectsAtPoint(RectCoordinate<Decimal>(val1, val2));
		}
		else if (command.compare("OVERLAPPING_OBJECTS") == 0) {
			m_ApplicationLogic->checkObjectsOverlaping();
		}
		else {
			LOGE("Error command %s", args[2]);
		}
	}

	Application::~Application() {}
} // namespace GeomSolver