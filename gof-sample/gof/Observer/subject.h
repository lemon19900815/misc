#ifndef __Observer_Subject_Inc_H__
#define __Observer_Subject_Inc_H__

#include "../stdafx.h"
#include "observer.h"
#include <list>

namespace Observer {

class Subject : std::enable_shared_from_this<Subject> {
public:
  Subject() {
    state_ = 0;
  }
  CLASS_PTR(Subject);

  int GetState() {
    return state_;
  }

  void SetState(int state) {
    state_ = state;
    NotifyAll();
  }

  void Attach(Observer::Ptr observer) {
    observers_.push_back(observer);
  }

  void NotifyAll() {
    auto this_sub = shared_from_this();
    for (auto& it : observers_) {
      it->Update(this_sub);
    }
  }

private:
  int32_t state_;
  std::list<Observer::Ptr> observers_;
};

void test() {
  std::cout << "\n\n observer pattern." << std::endl;

  auto subject = std::make_shared<Subject>();
  auto observer1 = std::make_shared<BinaryObserver>();
  auto observer2 = std::make_shared<OctalObserver>();
  auto observer3 = std::make_shared<HexaObserver>();

  subject->Attach(observer1);
  subject->Attach(observer2);
  subject->Attach(observer3);

  subject->SetState(1);
}

}

#endif // !__Observer_Subject_Inc_H__
