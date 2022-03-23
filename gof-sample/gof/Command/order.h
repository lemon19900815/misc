#ifndef __Command_Order_Inc_H__
#define __Command_Order_Inc_H__

#include "../stdafx.h"
#include "stock.h"
#include <list>

namespace Command
{
	class Order
	{
	public:
		virtual ~Order() {

		}

		virtual void execute() = 0;
	};

	class BuyStock : public Order
	{
	public:
		BuyStock(Stock* stock) {
			stock_ = stock;
		}

		void execute() {
			stock_->buy();
		}

	private:
		Stock* stock_;
	};

	class SellStock : public Order
	{
	public:
		SellStock(Stock* stock) {
			stock_ = stock;
		}

		void execute() {
			stock_->sell();
		}

	private:
		Stock* stock_;
	};

	class Broker
	{
	public:
		Broker() {

		}

		~Broker() {
			for (auto& it : orderList_) {
				Safe_Delete(it);
			}
			orderList_.clear();
		}

		void takeOrder(Order* order) {
			orderList_.push_back(order);
		}

		void placeOrders() {
			for (auto& it : orderList_) {
				it->execute();
			}
		}

	private:
		std::list<Order*> orderList_;
	};

	namespace CommandPatternDemo
	{
		void test()
		{
			std::cout << "\n\n command pattern." << std::endl;

			Stock* qq = new Stock("qq", 100);
			Stock* alibaba = new Stock("alibaba", 400);

			Broker* broker = new Broker();
			broker->takeOrder(new BuyStock(qq));
			broker->takeOrder(new SellStock(qq));

			broker->takeOrder(new BuyStock(alibaba));
			broker->takeOrder(new SellStock(alibaba));

			broker->placeOrders();

			Safe_Delete(qq);
			Safe_Delete(alibaba);
			Safe_Delete(broker);
		}
	}
}

#endif //  __Command_Order_Inc_H__
