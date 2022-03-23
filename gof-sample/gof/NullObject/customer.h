#ifndef __NullObject_Customer_Inc_H__
#define __NullObject_Customer_Inc_H__

#include "../stdafx.h"
#include <unordered_map>

namespace NullObject
{
	class AbstractCustomer
	{
	public:
		virtual ~AbstractCustomer() {

		}

		virtual bool isNil() = 0;
		virtual std::string name() = 0;

	protected:
		std::string name_;
	};

	class RealCustomer : public AbstractCustomer
	{
	public:
		RealCustomer(std::string& name) {
			name_ = name;
		}

		~RealCustomer() override {

		}

		bool isNil() override {
			return false;
		}

		std::string name() override {
			return name_;
		}
	};

	class NullCustomer : public AbstractCustomer
	{
	public:
		~NullCustomer() override {

		}

		bool isNil() override {
			return true;
		}

		std::string name() override {
			return "Not Available in Customer Database.";
		}
	};

	class CustomerFactory
	{
	public:
		static AbstractCustomer* createCustomer(std::string name) {
			for (int i = 0; i < customer_count; ++i) {
				if (name == available_customer[i])
					return new RealCustomer(name);
			}

			return new NullCustomer();
		}

		static void realeaseCustomer(AbstractCustomer* customer) {
			Safe_Delete(customer);
		}

	private:
		static constexpr int customer_count = 5;
		static std::string available_customer[customer_count];
	};

	std::string CustomerFactory::available_customer[customer_count] = { "Joe", "Jack", "Json", "Python", "Java" };

	namespace NullObjectPatternDemo
	{
		void test()
		{
			std::cout << "\n\n nullobject pattern." << std::endl;

			auto c1 = CustomerFactory::createCustomer("Joe");
			auto c2 = CustomerFactory::createCustomer("Jack");
			auto c3 = CustomerFactory::createCustomer("C++");

			std::cout << "c1: " << c1->name() << ", isNil ? "
				<< (c1->isNil() ? "Yes" : "No") << std::endl;

			std::cout << "c2: " << c2->name() << ", isNil ? "
				<< (c2->isNil() ? "Yes" : "No") << std::endl;

			std::cout << "c3: " << c3->name() << ", isNil ? "
				<< (c3->isNil() ? "Yes" : "No") << std::endl;

			CustomerFactory::realeaseCustomer(c1);
			CustomerFactory::realeaseCustomer(c2);
			CustomerFactory::realeaseCustomer(c3);
		}
	}
}

#endif //  __NullObject_Customer_Inc_H__
