#ifndef __Bridge_Shape_Inc_H__
#define __Bridge_Shape_Inc_H__

#include "drawapi.h"

namespace Bridge
{
	class Shape
	{
	public:
		Shape(DrawApi* drawApi) {
			drawApi_ = drawApi;
		}

		virtual ~Shape() {
			Safe_Delete(drawApi_);
		}

		virtual void draw() = 0;

	protected:
		DrawApi* drawApi_;
	};

	class Circle : public Shape
	{
	public:
		Circle(DrawApi* drawApi)
			: Shape(drawApi) {

		}

		~Circle() override {

		}

		void draw() override {
			drawApi_->draw("circle");
		}
	};

	namespace BridgePatternDemo
	{
		void test()
		{
			std::cout << "\n\nbridge model." << std::endl;

			Shape* shape1 = new Circle(new RedDraw());
			shape1->draw();

			Shape* shape2 = new Circle(new GreenDraw());
			shape2->draw();

			Safe_Delete(shape1);
			Safe_Delete(shape2);
		}
	}
}

#endif // !__Bridge_Shape_Inc_H__
