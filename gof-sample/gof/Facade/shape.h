#ifndef __Facade_Shape_Inc_H__
#define __Facade_Shape_Inc_H__

#include "../stdafx.h"

namespace Facade
{
	class Shape
	{
	public:
		virtual~Shape() {

		}

		virtual void draw() = 0;
	};

	class Circle : public Shape
	{
	public:
		~Circle() override {

		}

		void draw() override {
			std::cout << "circle draw." << std::endl;
		}
	};

	class Square : public Shape
	{
	public:
		~Square() override {

		}

		void draw() override {
			std::cout << "square draw." << std::endl;
		}
	};

	class Rectangle : public Shape
	{
	public:
		~Rectangle() override {

		}

		void draw() override {
			std::cout << "rectangle draw." << std::endl;
		}
	};
}

#endif // __Facade_Shape_Inc_H__
