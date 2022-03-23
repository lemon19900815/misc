#ifndef __Facade_ShapeMaker_Inc_H__
#define __Facade_ShapeMaker_Inc_H__

#include "../stdafx.h"
#include "shape.h"

namespace Facade
{
	class ShapeMaker
	{
	public:
		ShapeMaker() {
			circle_ = nullptr;
			square_ = nullptr;
			rectangle_ = nullptr;
		}

		~ShapeMaker() {
			Safe_Delete(circle_);
			Safe_Delete(square_);
			Safe_Delete(rectangle_);
		}

		void drawCircle() {
			if (!circle_) {
				circle_ = new Circle();
			}

			circle_->draw();
		}

		void drawSquare() {
			if (!square_) {
				square_ = new Square();
			}

			square_->draw();
		}

		void drawRectangle() {
			if (!rectangle_) {
				rectangle_ = new Rectangle();
			}

			rectangle_->draw();
		}

	private:
		Shape* circle_;
		Shape* square_;
		Shape* rectangle_;
	};

	namespace FacadePatternDemo
	{
		void test()
		{
			std::cout << "\n\n facade pattern." << std::endl;

			ShapeMaker* sm = new ShapeMaker();
			sm->drawCircle();
			sm->drawSquare();
			sm->drawRectangle();

			Safe_Delete(sm);
		}
	}
}

#endif // !__Facade_ShapeMaker_Inc_H__
