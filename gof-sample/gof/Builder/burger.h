#ifndef __Builder_Burger_Inc_H__
#define __Builder_Burger_Inc_H__

#include "item.h"
#include "packing.h"

namespace Builder
{
	class Burger : public Item
	{
	public:
		Burger() {
			packing_ = nullptr;
		}
		~Burger() override {
			Safe_Delete(packing_);
		}

		Packing* packing() override {
			if (!packing_) {
				packing_ = new Wrapper();
			}
			return packing_;
		}

	private:
		Packing* packing_;
	};

	class VegBurger : public Burger
	{
	public:
		~VegBurger() override {

		}

		const std::string name() override {
			return "VegBurger";
		}

		float price() override {
			return 5.0f;
		}
	};

	class ChickenBurger : public Burger
	{
	public:
		~ChickenBurger() override {

		}

		const std::string name() override {
			return "ChickenBurger";
		}

		float price() override {
			return 8.0f;
		}
	};
}

#endif // __Builder_Burger_Inc_H__
