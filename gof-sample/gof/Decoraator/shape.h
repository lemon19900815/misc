#ifndef __Decorator_Shape_Inc_H__
#define __Decorator_Shape_Inc_H__

#include "../stdafx.h"

namespace Decorator {

class Shape {
public:
    virtual~Shape() { }
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

class Rectangle : public Shape {
public:
    ~Rectangle() { }

    void draw() override {
        std::cout << "rectangle draw." << std::endl;
    }
};

class ShapeDecorator : public Shape {
public:
    ShapeDecorator(Shape::Ptr& shape) {
        shape_ = shape;
    }

protected:
    Shape::Ptr shape_;
};

class RedShapeDecorator : public ShapeDecorator
{
public:
    RedShapeDecorator(Shape::Ptr shape) : ShapeDecorator(shape) { }

    void draw() override {
        shape_->draw();
        std::cout << "red:" << std::endl;
    }
};

void test() {
    std::cout << "\n\n decorator pattern." << std::endl;

    auto sd1 = std::make_shared<RedShapeDecorator>(
        std::make_shared<Circle>());
    sd1->draw();

    auto sd2 = std::make_shared<RedShapeDecorator>(
        std::make_shared<Rectangle>());
    sd2->draw();

    auto sd3 = std::make_shared<RedShapeDecorator>(sd2);
    sd3->draw();
}

}

#endif // __Decorator_Shape_Inc_H__
