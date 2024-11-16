#pragma once
#include <memory>
#include <mutex>
#include <utility>

#include "Trangle.h"
#include "Circle.h"
#include "Rectangle.h"
#include "Def.h"

namespace GeomSolver
{
	template<class S>
	class IElement;

	template <class T>
	class ElementFactory
	{
	public:


		ElementFactory(const ElementFactory& factory) = delete;
		ElementFactory& operator=(const ElementFactory& factory) = delete;

		static std::shared_ptr<ElementFactory<T>> getFactory()
		{
			if (!g_factory)
			{
				std::lock_guard<std::mutex> guard(g_mutex);
				g_factory = std::shared_ptr<ElementFactory<T>>(new ElementFactory<T>());
			}
			return g_factory;

		}
		~ElementFactory() {}

		template<class... V,std::size_t N>
		IElement<T>* createElementFromRawData(const std::array<std::string, N>& raw)
		{
			if (raw.size() < 2)
			{
				LOGE("Not enough data to create element.");
				return nullptr;
			}
			if(raw[0].compare("CIRCLE") == 0)
			{ 
				if (raw.size() < 5) {
					LOGE("Not enough data to create CIRCLE.");
					return nullptr;
				}
				return createElement(ElementType::Circle, raw[1], std::make_tuple(std::atof(raw[2].c_str()), std::atof(raw[3].c_str()), std::atof(raw[4].c_str()),0.0f,0.0f,0.0f));

			}else if(raw[0].compare("RECTANGLE") == 0)
			{
				if (raw.size() < 6) {
					LOGE("Not enough data to create RECTANGLE.");
					return nullptr;
				}
				// swap 3<->5 since Rectnagle code writen assuming topLeft and bottomRight order
				return createElement(ElementType::Rectangle, raw[1], std::make_tuple(std::atof(raw[2].c_str()), std::atof(raw[5].c_str()),
					std::atof(raw[4].c_str()), std::atof(raw[3].c_str()), 0.0f, 0.0f));

			}else if (raw[0].compare("TRIANGLE") == 0)
			{
				if (raw.size() < 8) {
					LOGE("Not enough data to create TRIANGLE.");
					return nullptr;
				}
				return createElement(ElementType::Triangle, raw[1], std::make_tuple(std::atof(raw[2].c_str()), std::atof(raw[3].c_str()), std::atof(raw[4].c_str()),
					std::atof(raw[5].c_str()), std::atof(raw[6].c_str()), std::atof(raw[7].c_str())));

			}else
			{
				LOGE("Unknow element %s", raw[0].c_str());
				return nullptr;
			}
			return nullptr;
		}

		template<class... V>
		IElement<T>* createElement(const ElementType type,std::string name,const std::tuple <V...>& args)
		{
			switch (type)
			{
			case ElementType::Triangle: {
				return createTrangle(args, name);
			}
			case ElementType::Rectangle: {
				return createRectangle(args, name);
			}
			case ElementType::Circle: {
				return createCircle(args, name);
			}
			default: {
				return nullptr;
			}
			}
			return nullptr;

		}

	private:
		ElementFactory(){}

		template<class... V>
		Triangle<T>* createTrangle(const std::tuple <V...>& args, std::string& name){
			ElementId id = 0;
			{
				std::lock_guard<std::mutex> guard(g_mutex);
				id = m_Id;
				m_Id++;
			}
			Triangle<T>* ret =  new Triangle<T>(id, name, args);

			return ret;
		}

		template<class... V>
		Circle<T>* createCircle(const std::tuple <V...>& args, std::string& name) {
			ElementId id = 0;
			{
				std::lock_guard<std::mutex> guard(g_mutex);
				id = m_Id;
				m_Id++;
			}
			Circle<T>* ret = new Circle<T>(id,name, args);

			return ret;
		}

		template<class... V>
		Rectangle<T>* createRectangle(const std::tuple <V...>& args, std::string& name) {
			ElementId id = 0;
			{
				std::lock_guard<std::mutex> guard(g_mutex);
				id = m_Id;
				m_Id++;
			}
			Rectangle<T>* ret = new Rectangle<T>(id, name, args);

			return ret;
		}
	private:
		static std::shared_ptr<ElementFactory<T>> g_factory;
		static ElementId m_Id;
		static std::mutex g_mutex;

	};
	template<class T>
	std::shared_ptr <ElementFactory<T>> ElementFactory<T>::g_factory = nullptr;

	template<class T>
	ElementId ElementFactory<T>::m_Id = 0;

	template<class T>
	std::mutex ElementFactory<T>::g_mutex;
} // namespace GeomSolver

