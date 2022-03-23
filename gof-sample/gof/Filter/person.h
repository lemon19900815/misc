#ifndef __Filter_Person_Inc_H__
#define __Filter_Person_Inc_H__

namespace Filter
{
	class Person
	{
	public:
		Person(const std::string& name, const std::string& gender, int age) {
			name_ = name;
			gender_ = gender;
			age_ = age;
		}

		const std::string& name() {
			return name_;
		}

		const std::string& gender() {
			return gender_;
		}

		const int age() {
			return age_;
		}


	private:
		std::string name_;
		std::string gender_;
		int         age_;
	};
}

#endif // !__Filter_Person_Inc_H__
