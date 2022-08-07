#ifndef __Strategy_Stategy_Inc_H__
#define __Strategy_Stategy_Inc_H__

#include "../stdafx.h"

namespace Strategy {

class AbstractStategy {
public:
  virtual ~AbstractStategy() { }
  CLASS_PTR(AbstractStategy);

  virtual int32_t Operate(int32_t num1, int32_t num2) = 0;
};

class OperationAdd : public AbstractStategy {
public:
  int32_t Operate(int32_t num1, int32_t num2) override {
    return num1 + num2;
  }
};

class OperationSub : public AbstractStategy {
public:
  int32_t Operate(int32_t num1, int32_t num2) override {
    return num1 - num2;
  }
};

class OperationMultiply : public AbstractStategy {
public:
  int32_t Operate(int32_t num1, int32_t num2) override {
    return num1 * num2;
  }
};

class OperationDivide : public AbstractStategy {
public:
  int32_t Operate(int32_t num1, int32_t num2) override {
    if (num2 == 0) {
      std::cerr << "num2 is zero." << std::endl;
      assert(false);
      return -1;
    }
    return num1 / num2;
  }
};

class Context {
public:
  Context(AbstractStategy::Ptr strategy) {
    strategy_ = strategy;
  }

  virtual ~Context() {
  }

  int32_t Calculate(int32_t num1, int32_t num2) {
    return strategy_->Operate(num1, num2);
  }

private:
  AbstractStategy::Ptr strategy_;
};

void test() {
  std::cout << "\n\n strategy pattern." << std::endl;

  Context add(std::make_shared<OperationAdd>());
  Context sub(std::make_shared<OperationSub>());
  Context multiply(std::make_shared<OperationMultiply>());
  Context divide(std::make_shared<OperationDivide>());

  std::cout << "3 + 5 = " << add.Calculate(3, 5) << std::endl;
  std::cout << "1 - 7 = " << sub.Calculate(1, 7) << std::endl;
  std::cout << "3 * 5 = " << multiply.Calculate(3, 5) << std::endl;
  std::cout << "8 / 2 = " << divide.Calculate(8, 2) << std::endl;
}

}

#endif // __Strategy_Stategy_Inc_H__
