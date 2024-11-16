#pragma once
namespace GeomSolver
{
	template<class T>
	class ICoordinate
	{
	public:
		virtual ~ICoordinate() = 0;
	};

	template<class T>
	ICoordinate<T>::~ICoordinate() {}

} // namespace GeomSolver

