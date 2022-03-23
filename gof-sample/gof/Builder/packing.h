#ifndef __Builder_Packing_Inc_H__
#define __Builder_Packing_Inc_H__

#include "../stdafx.h"

namespace Builder
{
	class Packing
	{
	public:
		virtual ~Packing() {

		}

		virtual const std::string pack() = 0;
	};

	class Wrapper : public Packing
	{
	public:
		~Wrapper() override {

		}

		const std::string pack() override {
			return "Wrapper";
		}
	};

	class Bottle : public Packing
	{
	public:
		~Bottle() override {

		}

		const std::string pack() override {
			return "Bottle";
		}
	};
}

#endif // __Builder_Packing_Inc_H__
