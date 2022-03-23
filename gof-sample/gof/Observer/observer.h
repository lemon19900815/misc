#ifndef __Observer_Observer_Inc_H__
#define __Observer_Observer_Inc_H__

#include "../stdafx.h"

namespace Observer
{
	class Subject;

	class Observer
	{
	public:
		virtual ~Observer() {

		}

		virtual void update() = 0;

	protected:
		Subject* subject_;
	};

	class BinaryObserver : public Observer
	{
	public:
		BinaryObserver(Subject* subject) {
			subject_ = subject;
		}

		~BinaryObserver() override {
		}

		void update() override {
			std::cout << "BinaryObserver update." << std::endl;
		}
	};

	class OctalObserver : public Observer
	{
	public:
		OctalObserver(Subject* subject) {
			subject_ = subject;
		}

		~OctalObserver() override {
		}

		void update() override {
			std::cout << "OctalObserver update." << std::endl;
		}
	};

	class HexaObserver : public Observer
	{
	public:
		HexaObserver(Subject* subject) {
			subject_ = subject;
		}

		~HexaObserver() override {
		}

		void update() override {
			std::cout << "HexaObserver update." << std::endl;
		}
	};
}

#endif //  __Observer_Observer_Inc_H__
