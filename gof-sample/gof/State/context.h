#ifndef __State_Context_Inc_H__
#define __State_Context_Inc_H__

#include "../stdafx.h"

namespace State
{
	class State;

	class Context
	{
	public:
		Context() {
			state_ = nullptr;
		}

		~Context() {
		}

		State* state() {
			return state_;
		}

		void set_state(State* state) {
			state_ = state;
		}

	private:
		State* state_;
	};

	class State
	{
	public:
		virtual ~State() {

		}

		virtual void doAction(Context& ctx) = 0;
		virtual std::string toString() = 0;
	};

	class StartState : public State
	{
	public:
		~StartState() override {

		}

		void doAction(Context& ctx) override {
			std::cout << "start." << std::endl;
			ctx.set_state(this);
		}

		std::string toString() override {
			return "state: start";
		}
	};

	class StopState : public State
	{
	public:
		~StopState() override {

		}

		void doAction(Context& ctx) override {
			std::cout << "stop." << std::endl;
			ctx.set_state(this);
		}

		std::string toString() override {
			return "state: stop";
		}
	};

	namespace StatePatternDemo
	{
		void test()
		{
			std::cout << "\n\n state pattern." << std::endl;

			Context ctx;

			auto s1 = new StartState();
			s1->doAction(ctx);
			std::cout << ctx.state()->toString() << std::endl;

			auto s2 = new StopState();
			s2->doAction(ctx);
			std::cout << ctx.state()->toString() << std::endl;

			Safe_Delete(s1);
			Safe_Delete(s2);
		}
	}
}

#endif // !__State_Context_Inc_H__
