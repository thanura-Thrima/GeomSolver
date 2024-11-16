#pragma once
#include <cstdint>
#include <set>
#include <stdio.h>


#define LOGE(message,...) printf("\033[1;31m [Error] : "); printf(message,__VA_ARGS__); printf("\033[0m")
#define LOGD(message,...) printf("\033[1;33m [Debug] : ");printf(message,__VA_ARGS__); printf("\033[0m")
#define LOGI(message,...) printf(message,__VA_ARGS__)

namespace GeomSolver
{
	typedef double Decimal;
	typedef int32_t Integer;
	typedef size_t ElementId;

	const Integer c_quadTreeLevels = 8;
	const Decimal c_quadTreeLimitLeft = -10000.0f;
	const Decimal c_quadTreeLimitRight = 10000.0f;
	const Decimal c_quadTreeLimitTop = 10000.0f;
	const Decimal c_quadTreeLimitBottom = -10000.0f;

	enum class ElementType : int {
		Triangle = 0,
		Rectangle,
		Circle
	};
} // namespace GeomSolver