#ifndef __Bridge_Drawapi_Inc_H__
#define __Bridge_Drawapi_Inc_H__

#include "../stdafx.h"

namespace Bridge {
class DrawApi
{
public:
	virtual ~DrawApi() { }
    CLASS_PTR(DrawApi);

	virtual void draw(const std::string& shape) = 0;
};

class RedDraw : public DrawApi {
public:
	void draw(const std::string& shape) override {
		std::cout << "draw red " << shape << std::endl;
	}
};

class GreenDraw : public DrawApi {
public:
	void draw(const std::string& shape) override {
		std::cout << "draw green " << shape << std::endl;
	}
};

}

#endif // !__Bridge_Drawapi_Inc_H__
