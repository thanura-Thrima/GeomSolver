#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "Def.h"

namespace GeomSolver
{
	class CsvParser
	{
	public:
		CsvParser() = delete;
		template<std::size_t T>
		static std::vector<std::array<std::string,T>> getRows(std::string& fileName)
		{
			std::vector<std::array<std::string,T>> csvRows;
			std::ifstream input{ fileName };
			if (!input.is_open())
			{
				LOGE("Error opening %s", fileName.c_str());
				return csvRows;
			}
			for (std::string line; std::getline(input, line);) {
				std::istringstream ss(std::move(line));
				std::array<std::string,T> row;
				int i = 0;
				for (std::string value; std::getline(ss, value, ',');)
				{
					row[i] = std::move(value);
					i++;
				}
				csvRows.push_back(std::move(row));
			}
			return csvRows;

		}
	};
} // namespace GeomSolver

