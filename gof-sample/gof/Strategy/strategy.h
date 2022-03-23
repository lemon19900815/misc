#ifndef __Strategy_Stategy_Inc_H__
#define __Strategy_Stategy_Inc_H__

#include "../stdafx.h"

namespace Strategy
{
	class AbstractStategy
	{
	public:
		virtual ~AbstractStategy() {

		}

		virtual int operate(int num1, int num2) = 0;
	};

	class OperationAdd : public AbstractStategy
	{
	public:
		~OperationAdd() override {

		}

		int operate(int num1, int num2) override {
			return num1 + num2;
		}
	};

	class OperationSub : public AbstractStategy
	{
	public:
		~OperationSub() override {

		}

		int operate(int num1, int num2) override {
			return num1 - num2;
		}
	};

	class OperationMultiply : public AbstractStategy
	{
	public:
		~OperationMultiply() override {

		}

		int operate(int num1, int num2) override {
			return num1 * num2;
		}
	};

	class OperationDivide : public AbstractStategy
	{
	public:
		~OperationDivide() override {

		}

		int operate(int num1, int num2) override {
			if (num2 == 0) {
				std::cerr << "num2 is zero." << std::endl;
				assert(false);
				return -1;
			}
			return num1 / num2;
		}
	};

	class Context
	{
	public:
		Context(AbstractStategy* strategy) {
			strategy_ = strategy;
		}

		~Context() {
			Safe_Delete(strategy_);
		}

		int calculate(int num1, int num2) {
			return strategy_->operate(num1, num2);
		}

	private:
		AbstractStategy* strategy_;
	};

	namespace StragegyPatternDemo
	{
		void test()
		{
			std::cout << "\n\n strategy pattern." << std::endl;

			Context add(new OperationAdd());
			Context sub(new OperationSub());
			Context multiply(new OperationMultiply());
			Context divide(new OperationDivide());

			std::cout << "3 + 5 = " << add.calculate(3, 5) << std::endl;
			std::cout << "1 - 7 = " << sub.calculate(1, 7) << std::endl;
			std::cout << "3 * 5 = " << multiply.calculate(3, 5) << std::endl;
			std::cout << "8 / 2 = " << divide.calculate(8, 2) << std::endl;
		}
	}
}

#endif // __Strategy_Stategy_Inc_H__
