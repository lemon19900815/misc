#ifndef __Factory_Shape_Inc_H__
#define __Factory_Shape_Inc_H__

#include <iostream>
#include <cstring>
#include <string>

namespace Factory
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

	class ShapeFactory_
	{
	public:
		static Shape* createShape(const std::string& name) {
			Shape* shape = nullptr;

			if (name == "circle") {
				shape = new Circle();
			}
			else if (name == "square") {
				shape = new Square();
			}
			else if (name == "rectangle") {
				shape = new Rectangle();
			}

			return shape;
		}

		static void releaseShape(Shape* shape) {
			if (shape) {
				delete shape;
				shape = nullptr;
			}
		}
	};

	namespace FactoryPatternDemo
	{
		void test()
		{
			std::cout << "\n\n factory pattern." << std::endl;

			auto shape1 = ShapeFactory_::createShape("circle");
			shape1->draw();

			auto shape2 = ShapeFactory_::createShape("square");
			shape2->draw();

			auto shape3 = ShapeFactory_::createShape("rectangle");
			shape3->draw();

			ShapeFactory_::releaseShape(shape1);
			ShapeFactory_::releaseShape(shape2);
			ShapeFactory_::releaseShape(shape3);
		}
	}
}

#endif // __Factory_Shape_Inc_H__
