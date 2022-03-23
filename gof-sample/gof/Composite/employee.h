#ifndef __Composite_Employee_Inc_H__
#define __Composite_Employee_Inc_H__

#include "../stdafx.h"
#include <list>

namespace Composite
{
	class Employee
	{
	public:
		Employee(const std::string& name, const std::string& dept, int salary) {
			name_ = name;
			dept_ = dept;
			salary_ = salary;
		}

		~Employee() {
			for (auto& it : subordinates_) {
				Safe_Delete(it);
			}
			subordinates_.clear();
		}

		const std::list<Employee*>& subordinates() {
			return subordinates_;
		}

		const std::string& name() {
			return name_;
		}

		const std::string& dept() {
			return dept_;
		}

		int salary() {
			return salary_;
		}

		void add(Employee* e) {
			subordinates_.push_back(e);
		}

		void remove(Employee* e) {
			for (auto it = subordinates_.begin();
				it != subordinates_.end(); ++it) {
				if (e == *it) {
					subordinates_.erase(it);
					break;
				}
			}
		}
		
		void process() {
			std::cout << name() << " have remain subordinates:" << std::endl;
			
			for (auto& it : subordinates()) {
				std::cout << "[name: " << it->name() << ", dept: "
					<< it->dept() << ", salary: " << it->salary() << "]" << std::endl;
			}
		}

	private:
		std::string name_;
		std::string dept_;
		int salary_;

		std::list<Employee*> subordinates_;
	};

	namespace CompositePatternDemo
	{
		void test()
		{
			std::cout << "\n\n Composite Pattern." << std::endl;

			Employee* cto = new Employee("A", "CTO", 30000);
			Employee* pro1 = new Employee("B", "Programer", 23000);
			Employee* pro2 = new Employee("C", "Programer", 21000);
			Employee* pro3 = new Employee("D", "Programer", 10000);

			cto->add(pro1);
			cto->add(pro2);
			cto->add(pro3);
			
			cto->process();

			std::cout << "remove subordinate: " << pro3->name() << std::endl;
			cto->remove(pro3);
			
			cto->process();
		}
	}
}

#endif // __Composite_Employee_Inc_H__
