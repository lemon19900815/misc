#ifndef __State_TcpState_Inc_H__
#define __State_TcpState_Inc_H__

#include "../stdafx.h"

namespace TcpState
{
	class TcpState;

	class TcpContext
	{
	public:
		TcpContext() {
			frame_ = 0;
			tcpstate_ = nullptr;
		}

		TcpState* tcpstate() {
			return tcpstate_;
		}

		void set_tcpstate(TcpState* tcpstate) {
			tcpstate_ = tcpstate;
		}

		int frame() {
			return frame_;
		}

		void update();

	private:
		int			frame_;
		TcpState*	tcpstate_;
	};

	class TcpState
	{
	public:
		virtual ~TcpState() {

		}

		virtual void enter(TcpContext& ctx)  = 0;
		virtual void update(TcpContext& ctx) = 0;
		virtual void exit(TcpContext& ctx)   = 0;
	};

	class ConnectState : public TcpState
	{
	public:
		~ConnectState() override {

		}

		void enter(TcpContext& ctx) override {
			std::cout << "connect." << std::endl;
			ctx.set_tcpstate(this);
		}

		void update(TcpContext& ctx) override {

		}

		void exit(TcpContext& ctx) override {
			ctx.set_tcpstate(nullptr);
		}
	};

	class RunState : public TcpState
	{
	public:
		~RunState() override {

		}

		void enter(TcpContext& ctx) override {
			std::cout << "running." << std::endl;
			ctx.set_tcpstate(this);
		}

		void update(TcpContext& ctx) override {

		}

		void exit(TcpContext& ctx) override {
			ctx.set_tcpstate(nullptr);
		}
	};

	class ReconnectState : public TcpState
	{
	public:
		~ReconnectState() override {

		}

		void enter(TcpContext& ctx) override {
			std::cout << "reconnect." << std::endl;
			ctx.set_tcpstate(this);
		}

		void update(TcpContext& ctx) override {

		}

		void exit(TcpContext& ctx) override {
			ctx.set_tcpstate(nullptr);
		}
	};

	class CloseState : public TcpState
	{
	public:
		~CloseState() override {

		}

		void enter(TcpContext& ctx) override {
			std::cout << "close." << std::endl;
			ctx.set_tcpstate(this);
		}

		void update(TcpContext& ctx) override {

		}

		void exit(TcpContext& ctx) override {
			ctx.set_tcpstate(nullptr);
		}
	};

	void TcpContext::update()
	{
		int loop = 0;

		while (loop++ < 100) {
			++frame_;

			if (tcpstate_) {
				if (frame_ % 100 != 0) {
					tcpstate_->update(*this);
				}
				else {
					tcpstate_->exit(*this);
				}
			}

			_sleep(10);
		}
	}

	namespace TcpStateDemo
	{
		void test()
		{
			TcpContext ctx;

			auto s1 = new ConnectState();
			s1->enter(ctx);

			//ctx.update();

			auto s2 = new RunState();
			s2->enter(ctx);

			//ctx.update();

			auto s3 = new ReconnectState();
			s3->enter(ctx);

			//ctx.update();

			auto s4 = new CloseState();
			s4->enter(ctx);

			//ctx.update();

			Safe_Delete(s1);
			Safe_Delete(s2);
			Safe_Delete(s3);
			Safe_Delete(s4);
		}
	}
}

#endif // !__State_TcpState_Inc_H__
