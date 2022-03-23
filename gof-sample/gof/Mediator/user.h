#ifndef __Mediator_User_Inc_H__
#define __Mediator_User_Inc_H__

#include "../stdafx.h"

namespace Mediator
{
	class ChatRoom
	{
	public:
		static void showMessage(std::string& name, std::string& message) {
			std::cout << name << ": " << message << std::endl;
		}
	};

	class User
	{
	public:
		User(std::string name) {
			set_name(name);
		}

		const std::string& name() {
			return name_;
		}

		void set_name(std::string& name) {
			name_ = name;
		}

		void sendMessage(std::string message) {
			ChatRoom::showMessage(name_, message);
		}

	private:
		std::string name_;
	};

	namespace MediatorPatternDemo
	{
		void test()
		{
			std::cout << "\n\n mediator pattern." << std::endl;

			User* user = new User("Jack");
			user->sendMessage("This is a simple string.");
			Safe_Delete(user);
		}
	}
}

#endif //  __Mediator_User_Inc_H__
