#ifndef __Builder_Meal_Inc_H__
#define __Builder_Meal_Inc_H__

#include "item.h"
#include "packing.h"
#include "burger.h"
#include "drinking.h"

#include <list>

namespace Builder
{
	class Meal
	{
	public:
		float price() {
			float total = 0.0f;
			for (auto& item : items_) {
				total += item->price();
			}
			return total;
		}

		void show() {
			for (auto& item : items_) {
				std::cout << "item: " << item->name()
					<< ", packing: " << item->packing()->pack()
					<< ", price: " << item->price() << std::endl;
			}

			std::cout << "Total: " << price() << std::endl;
		}

	private:
		friend class MealBuilder;

		~Meal() {
			for (auto& item : items_) {
				Safe_Delete(item);
			}
			items_.clear();
		}

		void add(Item* item) {
			items_.push_back(item);
		}

	private:
		std::list<Item*> items_;
	};

	class MealBuilder
	{
	public:
		static Meal* prepareVegMeal() {
			Meal* meal = new Meal();
			meal->add(new VegBurger());
			meal->add(new Pepsi());
			return meal;
		}

		static Meal* prepareNonVegMeal() {
			Meal* meal = new Meal();
			meal->add(new ChickenBurger());
			meal->add(new Coke());
			return meal;
		}

		static void releaseMeal(Meal* meal) {
			Safe_Delete(meal);
		}
	};

	namespace BuilderPatternDemo
	{
		void test()
		{
			std::cout << "\n\n builder pattern." << std::endl;

			auto meal1 = MealBuilder::prepareNonVegMeal();
			meal1->show();

			auto meal2 = MealBuilder::prepareVegMeal();
			meal2->show();

			MealBuilder::releaseMeal(meal1);
			MealBuilder::releaseMeal(meal2);
		}
	}
}

#endif // __Builder_Meal_Inc_H__
