#ifndef __Memento_Originator_Inc_H__
#define __Memento_Originator_Inc_H__

#include "../stdafx.h"
#include "memento.h"

namespace Memento
{
	class Originator
	{
	public:
		Originator() {
			state_ = "default";
		}

		void set_state(std::string state) {
			state_ = state;
		}

		const std::string& state() {
			return state_;
		}

		Memento* saveStateToMemento() {
			return new Memento(state_);
		}

		void getStateFromMemento(Memento* memento) {
			set_state(memento->state());
		}

	private:
		std::string state_;
	};

	namespace MementoPatternDemo
	{
		void test()
		{
			std::cout << "\n\n memento pattern." << std::endl;

			auto originator = new Originator();
			auto careTaker = new CareTaker();

			originator->set_state("state #0");
			careTaker->add(originator->saveStateToMemento());

			originator->set_state("state #1");
			careTaker->add(originator->saveStateToMemento());

			originator->set_state("state #2");
			careTaker->add(originator->saveStateToMemento());

			originator->set_state("state #3");
			careTaker->add(originator->saveStateToMemento());

			auto memento0 = careTaker->get(0);
			std::cout << memento0->state() << std::endl;

			originator->getStateFromMemento(memento0);
			std::cout << "originator recover to state: " << originator->state() << std::endl;

			Safe_Delete(originator);
			Safe_Delete(careTaker);
		}
	}
}

#endif // ! __Memento_Originator_Inc_H__
