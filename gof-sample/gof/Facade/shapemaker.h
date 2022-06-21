#ifndef __Facade_ShapeMaker_Inc_H__
#define __Facade_ShapeMaker_Inc_H__

#include "../stdafx.h"

namespace Facade {

class Shape {
public:
    virtual ~Shape() { }
    CLASS_PTR(Shape);

    virtual void draw() = 0;
};

class Circle : public Shape {
public:
    ~Circle() { }

    void draw() override {
        std::cout << "circle draw." << std::endl;
    }
};

class Square : public Shape {
public:
    ~Square() { }
    void draw() override {
        std::cout << "square draw." << std::endl;
    }
};

class Rectangle : public Shape
{
public:
    ~Rectangle() { }

    void draw() override {
        std::cout << "rectangle draw." << std::endl;
    }
};

class ShapeFacade {
public:
    ShapeFacade() {
        circle_ = std::make_shared<Circle>();
        square_ = std::make_shared<Square>();
        rectangle_ = std::make_shared<Rectangle>();
    }

    ~ShapeFacade() {
    }

    void drawCircle() {
        circle_->draw();
    }

    void drawSquare() {
        square_->draw();
    }

    void drawRectangle() {
        rectangle_->draw();
    }

private:
    std::shared_ptr<Shape> circle_;
    std::shared_ptr<Shape> square_;
    std::shared_ptr<Shape> rectangle_;
};

void test() {
	std::cout << "\n\n facade pattern." << std::endl;

	auto sm = std::make_shared<ShapeFacade>();
	sm->drawCircle();
	sm->drawSquare();
	sm->drawRectangle();
}

}

#endif // !__Facade_ShapeMaker_Inc_H__
