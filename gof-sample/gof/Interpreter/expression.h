#ifndef __Interpreter_Expression_Inc_H__
#define __Interpreter_Expression_Inc_H__

#include "../stdafx.h"

namespace Interpreter {

class Expression {
public:
  virtual ~Expression() { }
  CLASS_PTR(Expression);

  virtual bool Interpret(std::string context) = 0;
};

class TerminalExpression : public Expression {
public:
  TerminalExpression(std::string data) {
    data_ = data;
  }

  bool Interpret(std::string context) override {
    return context.find(data_) != std::string::npos;
  }

private:
  std::string data_;
};

class AndExpression : public Expression {
public:
  AndExpression(Expression::Ptr lhs, Expression::Ptr rhs) {
    lhs_ = lhs;
    rhs_ = rhs;
  }

  bool Interpret(std::string context) override {
    return lhs_->Interpret(context) && rhs_->Interpret(context);
  }

private:
  Expression::Ptr lhs_;
  Expression::Ptr rhs_;
};

class OrExpression : public Expression {
public:
  OrExpression(Expression::Ptr lhs, Expression::Ptr rhs) {
    lhs_ = lhs;
    rhs_ = rhs;
  }

  bool Interpret(std::string context) override {
    return lhs_->Interpret(context) || rhs_->Interpret(context);
  }

private:
  Expression::Ptr lhs_;
  Expression::Ptr rhs_;
};

void test() {
  LOG("\n\n interpreter pattern.");

  std::shared_ptr<Expression> male = std::make_shared<TerminalExpression>("male");
  std::shared_ptr<Expression>  female = std::make_shared<TerminalExpression>("female");
  std::shared_ptr<Expression>  married = std::make_shared<TerminalExpression>("married");

  std::shared_ptr<Expression>  femaleAndMarried = std::make_shared<AndExpression>(female, married);
  std::shared_ptr<Expression>  maleOrMarried = std::make_shared<OrExpression>(male, married);

  LOG("John is a male ? " << (male->Interpret("male") ? "Yes" : "No"));
  LOG("Julie is a married women ? " << (femaleAndMarried->Interpret("female married") ? "Yes" : "No"));
  LOG("Jack is a male or married ? " << (maleOrMarried->Interpret("married") ? "Yes" : "No"));
}

}

#endif //  __Interpreter_Expression_Inc_H__
