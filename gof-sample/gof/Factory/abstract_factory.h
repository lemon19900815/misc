#ifndef __Factory_Abstract_Factory_Inc_H__
#define __Factory_Abstract_Factory_Inc_H__

#include "shape.h"
#include "color.h"

#include <iostream>
#include <cstring>

namespace AbstractFactory
{
	class AbstractFactory
	{
	public:
		virtual ~AbstractFactory() {

		}

		virtual Factory::Shape*	createShape(const std::string& name) = 0;
		virtual void	releaseShape(Factory::Shape* shape) = 0;

		virtual Color*	createColor(const std::string& name) = 0;
		virtual void	releaseColor(Color* color) = 0;
	};

	class ShapeFactory : public AbstractFactory
	{
	public:
		~ShapeFactory() override {

		}

		Factory::Shape*	createShape(const std::string& name) override {
			return Factory::ShapeFactory_::createShape(name);
		}
		void	releaseShape(Factory::Shape* shape) override {
			if (shape) {
				delete shape;
				shape = nullptr;
			}
		}

		Color*	createColor(const std::string& name) override {
			return nullptr;
		}
		void	releaseColor(Color* color) override {

		}
	};

	class ColorFactory : public AbstractFactory
	{
	public:
		~ColorFactory() override {

		}

		Factory::Shape*	createShape(const std::string& name) override {
			return nullptr;
		}
		void	releaseShape(Factory::Shape* shape) override {
		}

		Color*	createColor(const std::string& name) override {
			return ColorFactory_::createColor(name);
		}
		void	releaseColor(Color* color) override {
			if (color) {
				delete color;
				color = nullptr;
			}
		}
	};
}

#endif //  __Factory_Abstract_Factory_Inc_H__
