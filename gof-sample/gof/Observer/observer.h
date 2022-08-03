#ifndef __Observer_Observer_Inc_H__
#define __Observer_Observer_Inc_H__

#include "../stdafx.h"

namespace Observer {

class Subject;

class Observer {
public:
  virtual ~Observer() { }
  CLASS_PTR(Observer);

  virtual void Update(std::shared_ptr<Subject> subject) = 0;
};

class BinaryObserver : public Observer {
public:
  void Update(std::shared_ptr<Subject> subject) override {
    std::cout << "BinaryObserver update." << std::endl;
  }
};

class OctalObserver : public Observer {
public:
  void Update(std::shared_ptr<Subject> subject) override {
    std::cout << "OctalObserver update." << std::endl;
  }
};

class HexaObserver : public Observer {
public:
  void Update(std::shared_ptr<Subject> subject) override {
    std::cout << "HexaObserver update." << std::endl;
  }
};

}

#endif //  __Observer_Observer_Inc_H__
