#ifndef __Interpreter_Expression_Inc_H__
#define __Interpreter_Expression_Inc_H__

#include "../stdafx.h"

namespace Interpreter
{
	class Expression
	{
	public:
		virtual ~Expression() {

		}

		virtual bool interpret(std::string context) = 0;
	};

	class TerminalExpression : public Expression
	{
	public:
		TerminalExpression(std::string data) {
			data_ = data;
		}

		bool interpret(std::string context) override {
			return context.find(data_) != std::string::npos;
		}

	private:
		std::string data_;
	};

	class AndExpression : public Expression
	{
	public:
		AndExpression(Expression* lhs, Expression* rhs) {
			lhs_ = lhs;
			rhs_ = rhs;
		}

		bool interpret(std::string context) override {
			return lhs_->interpret(context) && rhs_->interpret(context);
		}

	private:
		Expression* lhs_;
		Expression* rhs_;
	};

	class OrExpression : public Expression
	{
	public:
		OrExpression(Expression* lhs, Expression* rhs) {
			lhs_ = lhs;
			rhs_ = rhs;
		}

		bool interpret(std::string context) override {
			return lhs_->interpret(context) || rhs_->interpret(context);
		}

	private:
		Expression* lhs_;
		Expression* rhs_;
	};

	namespace InterpreterPatternDemo
	{
		void test()
		{
			std::cout << "\n\n interpreter pattern." << std::endl;

			Expression* male = new TerminalExpression("male");
			Expression* female = new TerminalExpression("female");
			Expression* married = new TerminalExpression("married");

			Expression* femaleAndMarried = new AndExpression(female, married);
			Expression* maleOrMarried = new OrExpression(male, married);

			std::cout << "John is a male ? " << (male->interpret("male") ? "Yes" : "No") << std::endl;
			std::cout << "Julie is a married women ? " << (femaleAndMarried->interpret("female married") ? "Yes" : "No") << std::endl;
			std::cout << "Jack is a male or married ? " << (maleOrMarried->interpret("married") ? "Yes" : "No") << std::endl;

			Safe_Delete(male);
			Safe_Delete(female);
			Safe_Delete(married);
			Safe_Delete(femaleAndMarried);
			Safe_Delete(maleOrMarried);
		}
	}
}

#endif //  __Interpreter_Expression_Inc_H__
