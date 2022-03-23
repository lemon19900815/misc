#ifndef __Command_Stock_Inc_H__
#define __Command_Stock_Inc_H__

#include "../stdafx.h"

namespace Command
{
	class Stock
	{
	public:
		Stock(std::string name, int amount) {
			name_ = name;
			amount_ = amount;
		}

		~Stock() {

		}

		void buy() {
			std::cout << "buy stock[name: " << name_
				<< ", amount: " << amount_ << "]" << std::endl;
		}

		void sell() {
			std::cout << "sell stock[name: " << name_
				<< ", amount: " << amount_ << "]" << std::endl;
		}

	private:
		std::string name_;
		int			amount_;
	};
}

#endif //  __Command_Stock_Inc_H__
