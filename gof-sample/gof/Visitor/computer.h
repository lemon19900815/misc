#ifndef __Visitor_Computer_Inc_H__
#define __Visitor_Computer_Inc_H__

#include "../stdafx.h"

namespace Visitor
{
	class Keyboard;
	class Mouse;
	class Monitor;
	class Computer;

	class ComputerPartVisiter
	{
	public:
		void visit(Keyboard* keyboard) {
			std::cout << "keyboard display." << std::endl;
		}

		void visit(Mouse* mouse) {
			std::cout << "mouse display." << std::endl;
		}

		void visit(Monitor* monitor) {
			std::cout << "monitor display." << std::endl;
		}

		void visit(Computer* computer) {
			std::cout << "computer display." << std::endl;
		}
	};

	class ComputerPart
	{
	public:
		virtual ~ComputerPart() {

		}

		virtual void accept(ComputerPartVisiter& visitor) = 0;
	};

	class Keyboard : public ComputerPart
	{
	public:
		~Keyboard() override {

		}

		void accept(ComputerPartVisiter& visitor) override {
			visitor.visit(this);
		}
	};

	class Mouse : public ComputerPart
	{
	public:
		~Mouse() override {

		}

		void accept(ComputerPartVisiter& visitor) override {
			visitor.visit(this);
		}
	};

	class Monitor : public ComputerPart
	{
	public:
		~Monitor() override {

		}

		void accept(ComputerPartVisiter& visitor) override {
			visitor.visit(this);
		}
	};

	class Computer : public ComputerPart
	{
	public:
		Computer() {
			parts_[0] = new Keyboard();
			parts_[1] = new Monitor();
			parts_[2] = new Mouse();
		}

		~Computer() override {
			for (int i = 0; i < 3; ++i) {
				Safe_Delete(parts_[i]);
			}
		}

		void accept(ComputerPartVisiter& visitor) override {
			for (int i = 0; i < 3; ++i) {
				parts_[i]->accept(visitor);
			}
			visitor.visit(this);
		}

	private:
		ComputerPart* parts_[3];
	};

	namespace VisitorPatternDemo
	{
		void test()
		{
			std::cout << "\n\n visitor pattern." << std::endl;

			ComputerPartVisiter visitor;
			Computer computer;

			computer.accept(visitor);
		}
	}
}

#endif // __Visitor_Computer_Inc_H__
