#ifndef __Decorator_ShapeDecorator_Inc_H__
#define __Decorator_ShapeDecorator_Inc_H__

#include "shape.h"

namespace Decorator
{
	class ShapeDecorator
	{
	public:
		ShapeDecorator(Shape* shape) {
			shape_ = shape;
		}

		virtual ~ShapeDecorator() {
			Safe_Delete(shape_);
		}

		virtual void draw() = 0;

	protected:
		Shape* shape_;
	};

	class RedShapeDecorator : public ShapeDecorator
	{
	public:
		RedShapeDecorator(Shape* shape) : ShapeDecorator(shape){

		}

		void draw() override {
			shape_->draw();
			std::cout << "red:" << std::endl;
		}
	};

	namespace DecoratorPatternDemo
	{
		void test()
		{
			std::cout << "\n\n decorator pattern." << std::endl;

			ShapeDecorator* sd1 = new RedShapeDecorator(new Circle());
			sd1->draw();

			ShapeDecorator* sd2 = new RedShapeDecorator(new Rectangle());
			sd2->draw();

			Safe_Delete(sd1);
			Safe_Delete(sd2);
		}
	}
}

#endif // !__Decorator_ShapeDecorator_Inc_H__
