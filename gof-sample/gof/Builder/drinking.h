#ifndef __Builder_Drinking_Inc_H__
#define __Builder_Drinking_Inc_H__

#include "item.h"
#include "packing.h"

namespace Builder
{
	class ColdDrink : public Item
	{
	public:
		ColdDrink() {
			packing_ = nullptr;
		}
		~ColdDrink() override {
			Safe_Delete(packing_);
		}

		Packing* packing() override {
			if (!packing_) {
				packing_ = new Bottle();
			}
			return packing_;
		}

	private:
		Packing* packing_;
	};

	class Pepsi : public ColdDrink
	{
	public:
		~Pepsi() override {

		}

		const std::string name() override {
			return "Pepsi";
		}

		float price() override {
			return 3.0f;
		}
	};

	class Coke : public ColdDrink
	{
	public:
		~Coke() override {

		}

		const std::string name() override {
			return "Coke";
		}

		float price() override {
			return 4.0f;
		}
	};
}

#endif //  __Builder_Drinking_Inc_H__
