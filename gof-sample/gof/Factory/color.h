#ifndef __Factory_Color_Inc_H__
#define __Factory_Color_Inc_H__

#include <iostream>

class Color
{
public:
	virtual~Color() {

	}

	virtual void fill() = 0;
};

class Red : public Color
{
public:
	~Red() override {

	}

	void fill() override {
		std::cout << "red fill." << std::endl;
	}
};

class Green : public Color
{
public:
	~Green() override {

	}

	void fill() override {
		std::cout << "green fill." << std::endl;
	}
};

class Blue : public Color
{
public:
	~Blue() override {

	}

	void fill() override {
		std::cout << "blue fill." << std::endl;
	}
};

class ColorFactory_
{
public:
	static Color* createColor(const std::string& name) {
		Color* color = nullptr;

		if (name == "red") {
			color = new Red();
		}
		else if (name == "green") {
			color = new Green();
		}
		else if (name == "blue") {
			color = new Blue();
		}

		return color;
	}

	static void releaseColor(Color* color) {
		if (color) {
			delete color;
			color = nullptr;
		}
	}
};

#endif // __Factory_Color_Inc_H__
