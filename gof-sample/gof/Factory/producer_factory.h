#ifndef __Factory_Producer_Factory_Inc_H__
#define __Factory_Producer_Factory_Inc_H__

#include "abstract_factory.h"

namespace AbstractFactory
{
	class FactoryProducer
	{
	public:
		static AbstractFactory* createFactory(const std::string& choice) {
			AbstractFactory* factory = nullptr;

			if (choice == "shape") {
				factory = new ShapeFactory();
			}
			else if (choice == "color") {
				factory = new ColorFactory();
			}

			return factory;
		}

		static void releaseFactory(AbstractFactory* factory) {
			if (factory) {
				delete factory;
				factory = nullptr;
			}
		}
	};

	namespace AbstractFactoryPatternDemo
	{
		void test()
		{
			std::cout << "\n\n abstract factory." << std::endl;

			std::cout << "shape factory." << std::endl;
			auto shapeFactory = FactoryProducer::createFactory("shape");

			auto shape1 = shapeFactory->createShape("circle");
			shape1->draw();

			auto shape2 = shapeFactory->createShape("square");
			shape2->draw();

			auto shape3 = shapeFactory->createShape("rectangle");
			shape3->draw();

			shapeFactory->releaseShape(shape1);
			shapeFactory->releaseShape(shape2);
			shapeFactory->releaseShape(shape3);

			std::cout << std::endl << "color factory." << std::endl;
			auto colorFactory = FactoryProducer::createFactory("color");

			auto color1 = colorFactory->createColor("red");
			color1->fill();

			auto color2 = colorFactory->createColor("green");
			color2->fill();

			auto color3 = colorFactory->createColor("blue");
			color3->fill();

			colorFactory->releaseColor(color1);
			colorFactory->releaseColor(color2);
			colorFactory->releaseColor(color3);

			FactoryProducer::releaseFactory(shapeFactory);
			FactoryProducer::releaseFactory(colorFactory);
		}
	}
}

#endif //  __Factory_Producer_Factory_Inc_H__
