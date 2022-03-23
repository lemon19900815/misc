#ifndef __Observer_Subject_Inc_H__
#define __Observer_Subject_Inc_H__

#include "../stdafx.h"
#include "observer.h"
#include <list>

namespace Observer
{
	class Subject
	{
	public:
		Subject() {
			state_ = 0;
		}

		int state() {
			return state_;
		}

		void set_state(int state) {
			state_ = state;
		}

		void attach(Observer* observer) {
			observers_.push_back(observer);
		}

		void notifyAll() {
			for (auto& it : observers_) {
				it->update();
			}
		}

	private:
		int state_;
		std::list<Observer*> observers_;
	};

	namespace ObserverPatternDemo
	{
		void test()
		{
			std::cout << "\n\n observer pattern." << std::endl;

			Subject* subject = new Subject();
			auto observer1 = new BinaryObserver(subject);
			auto observer2 = new OctalObserver(subject);
			auto observer3 = new HexaObserver(subject);

			subject->attach(observer1);
			subject->attach(observer2);
			subject->attach(observer3);

			subject->set_state(1);
			subject->notifyAll();
		}
	}
}

#endif // !__Observer_Subject_Inc_H__
