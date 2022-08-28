#ifndef __Visitor_Computer_Inc_H__
#define __Visitor_Computer_Inc_H__

#include "../stdafx.h"

namespace Visitor {

class Keyboard;
class Mouse;
class Monitor;
class Computer;

class ComputerPartVisiter {
public:
  void Visit(Keyboard* keyboard) {
    std::cout << "keyboard display." << std::endl;
  }

  void Visit(Mouse* mouse) {
    std::cout << "mouse display." << std::endl;
  }

  void Visit(Monitor* monitor) {
    std::cout << "monitor display." << std::endl;
  }

  void Visit(Computer* computer) {
    std::cout << "computer display." << std::endl;
  }
};

class ComputerPart {
public:
  virtual ~ComputerPart() { }
  CLASS_PTR(ComputerPart);

  virtual void Accept(ComputerPartVisiter& visitor) = 0;
};

class Keyboard : public ComputerPart {
public:
  void Accept(ComputerPartVisiter& visitor) override {
    visitor.Visit(this);
  }
};

class Mouse : public ComputerPart
{
public:
  void Accept(ComputerPartVisiter& visitor) override {
    visitor.Visit(this);
  }
};

class Monitor : public ComputerPart
{
public:
  void Accept(ComputerPartVisiter& visitor) override {
    visitor.Visit(this);
  }
};

class Computer : public ComputerPart {
public:
  Computer() {
    parts_.push_back(std::make_shared<Keyboard>());
    parts_.push_back(std::make_shared<Monitor>());
    parts_.push_back(std::make_shared<Mouse>());
  }

  ~Computer() {
  }

  void Accept(ComputerPartVisiter& visitor) override {
    for (auto& ele : parts_) {
      ele->Accept(visitor);
    }
    visitor.Visit(this);
  }

private:
  std::list<ComputerPart::Ptr> parts_;
};

void test() {
  std::cout << "\n\n visitor pattern." << std::endl;

  ComputerPartVisiter visitor;
  Computer computer;

  computer.Accept(visitor);
}

}

#endif // __Visitor_Computer_Inc_H__
