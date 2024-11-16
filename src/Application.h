#pragma once
#include <memory>

namespace GeomSolver {

	class ApplicationLogic;
	class Application
	{
	public:
		Application(char* args);
		~Application();
		void run(int argCount, char** args);
	private:
		std::unique_ptr<ApplicationLogic> m_ApplicationLogic{ nullptr };
	};
} // namespace GeomSolver

