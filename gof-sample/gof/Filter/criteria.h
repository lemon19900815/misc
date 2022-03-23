#ifndef __Filter_Criteria_Inc_H__
#define __Filter_Criteria_Inc_H__

#include "../stdafx.h"
#include "person.h"

#include <cassert>
#include <list>

namespace Filter
{
	class Criteria
	{
	public:
		virtual ~Criteria() {

		}

		virtual std::list<Person> meetCirteria(std::list<Person>& persons) = 0;
	};

	class AndCriteria : public Criteria
	{
	public:
		AndCriteria(Criteria* lhs, Criteria* rhs) {
			lhs_ = lhs;
			rhs_ = rhs;
		}

		~AndCriteria() override {
			Safe_Delete(lhs_);
			Safe_Delete(rhs_);
		}

		std::list<Person> meetCirteria(std::list<Person>& persons) override {
			assert(lhs_ && rhs_);
			std::list<Person> t1 = lhs_->meetCirteria(persons);
			std::list<Person> t2 = rhs_->meetCirteria(t1);
			return std::move(t2);
		}

	private:
		Criteria* lhs_;
		Criteria* rhs_;
	};

	class OrCriteria : public Criteria
	{
	public:
		OrCriteria(Criteria* lhs, Criteria* rhs) {
			lhs_ = lhs;
			rhs_ = rhs;
		}

		~OrCriteria() override {
			Safe_Delete(lhs_);
			Safe_Delete(rhs_);
		}

		std::list<Person> meetCirteria(std::list<Person>& persons) override {
			assert(lhs_ && rhs_);
			std::list<Person> t1 = lhs_->meetCirteria(persons);
			if (t1.empty()) {
				t1 = rhs_->meetCirteria(persons);
			}
			return std::move(t1);
		}

	private:
		Criteria* lhs_;
		Criteria* rhs_;
	};

	class FemaleCriteria : public Criteria
	{
	public:
		~FemaleCriteria() override {

		}

		std::list<Person> meetCirteria(std::list<Person>& persons) override {
			std::list<Person> t1;
			for (auto& it : persons) {
				if (it.gender() == "female") {
					t1.push_back(it);
				}
			}
			return std::move(t1);
		}
	};

	class MaleCriteria : public Criteria
	{
	public:
		~MaleCriteria() override {

		}

		std::list<Person> meetCirteria(std::list<Person>& persons) override {
			std::list<Person> t1;
			for (auto& it : persons) {
				if (it.gender() == "male") {
					t1.push_back(it);
				}
			}
			return std::move(t1);
		}
	};

	class AgeCriteria : public Criteria
	{
	public:
		AgeCriteria(int age) {
			age_ = age;
		}

		~AgeCriteria() override {

		}

		std::list<Person> meetCirteria(std::list<Person>& persons) override {
			std::list<Person> t1;
			for (auto& it : persons) {
				if (it.age() == age_) {
					t1.push_back(it);
				}
			}
			return std::move(t1);
		}

	private:
		int age_;
	};

	namespace CriteriaPatternDemo
	{
		void test()
		{
			std::cout << "\n\n Filter[Criteria] Pattern" << std::endl;

			Person p1("XieGang", "male", 28);
			Person p2("YuanYaou", "female", 26);
			Person p3("XieSiyuan", "female", 6);
			Person p4("XieYuanhong", "male", 6);

			std::list<Person> persons;
			persons.push_back(std::move(p1));
			persons.push_back(std::move(p2));
			persons.push_back(std::move(p3));
			persons.push_back(std::move(p4));

			std::cout << "male criteria:" << std::endl;

			Criteria* maleCriteria = new MaleCriteria();
			auto maleList = maleCriteria->meetCirteria(persons);

			for (auto& it : maleList) {
				std::cout << it.name() << "," << it.gender() << "," << it.age() << std::endl;
			}

			Safe_Delete(maleCriteria);

			std::cout << "\nfemale criteria:" << std::endl;

			Criteria* femaleCriteria = new FemaleCriteria();
			auto femaleList = femaleCriteria->meetCirteria(persons);

			for (auto& it : femaleList) {
				std::cout << it.name() << "," << it.gender() << "," << it.age() << std::endl;
			}

			Safe_Delete(femaleCriteria);

			std::cout << "\nage criteria:" << std::endl;

			Criteria* ageCriteria = new AgeCriteria(6);
			auto age6 = ageCriteria->meetCirteria(persons);

			for (auto& it : age6) {
				std::cout << it.name() << "," << it.gender() << "," << it.age() << std::endl;
			}

			Safe_Delete(ageCriteria);

			std::cout << "\nand criteria[age6&male]:" << std::endl;

			Criteria* andCriteria = new AndCriteria(new AgeCriteria(6), new MaleCriteria());
			auto maleOfAge6 = andCriteria->meetCirteria(persons);

			for (auto& it : maleOfAge6) {
				std::cout << it.name() << "," << it.gender() << "," << it.age() << std::endl;
			}

			Safe_Delete(andCriteria);

			std::cout << "\nor criteria[age7|female]:" << std::endl;

			Criteria* orCriteria = new OrCriteria(new AgeCriteria(7), new FemaleCriteria());
			auto femaleOfAge7 = orCriteria->meetCirteria(persons);

			for (auto& it : femaleOfAge7) {
				std::cout << it.name() << "," << it.gender() << "," << it.age() << std::endl;
			}

			Safe_Delete(orCriteria);
		}
	}
}

#endif // !__Filter_Criteria_Inc_H__
