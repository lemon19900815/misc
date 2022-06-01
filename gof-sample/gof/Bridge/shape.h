#ifndef __Bridge_Shape_Inc_H__
#define __Bridge_Shape_Inc_H__

#include "drawapi.h"

namespace Bridge {

class Shape {
public:
    Shape(DrawApi::Ptr drawApi) {
        drawApi_ = drawApi;
    }

    virtual ~Shape() { }
    CLASS_PTR(Shape);

    virtual void draw() = 0;

protected:
    DrawApi::Ptr drawApi_;
};

class Circle : public Shape {
public:
    Circle(DrawApi::Ptr drawApi)
        : Shape(drawApi) {
    }

    ~Circle() {
    }

    void draw() override {
        drawApi_->draw("circle");
    }
};

void test() {
    std::cout << "\n\nbridge model." << std::endl;

    auto shape1 = std::make_shared<Circle>(std::make_shared<RedDraw>());
    shape1->draw();

    auto shape2 = std::make_shared<Circle>(std::make_shared<GreenDraw>());
    shape2->draw();
}

}

#endif // !__Bridge_Shape_Inc_H__
