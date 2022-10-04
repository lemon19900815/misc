#ifndef __Memento_Memento_Inc_H__
#define __Memento_Memento_Inc_H__

#include "../stdafx.h"
#include <vector>

namespace Memento {

class Memento {
public:
  Memento(std::string state) {
    state_ = state;
  }
  virtual ~Memento() { }
  CLASS_PTR(Memento);

private:
  const std::string& state() {
    return state_;
  }

private:
  friend class Originator;
  std::string state_;
};

class CareTaker {
public:
  ~CareTaker() {
    hist_memento_.clear();
  }

  void Add(Memento::Ptr mt) {
    hist_memento_.push_back(mt);
  }

  Memento::Ptr Get(size_t index) {
    return index < hist_memento_.size() ? hist_memento_[index] : nullptr;
  }

private:
  std::vector<Memento::Ptr> hist_memento_;
};

class Originator {
public:
  Originator() {
    state_ = "default";
  }

  void SetState(std::string state) {
    state_ = state;
    LOG("originator cur state: " << state_);
  }

  const std::string& GetState() {
    return state_;
  }

  Memento::Ptr CreateMemento() {
    return std::make_shared<Memento>(state_);
  }

  void Reset(Memento::Ptr memento) {
    LOG("Originator::Reset memento state...");
    SetState(memento->state());
  }

private:
  std::string state_;
};

void test() {
  LOG("\n\n memento pattern.");

  Originator originator;
  CareTaker care_taker;

  originator.SetState("state #0");
  care_taker.Add(originator.CreateMemento());

  originator.SetState("state #1");
  care_taker.Add(originator.CreateMemento());

  originator.SetState("state #2");
  care_taker.Add(originator.CreateMemento());

  originator.SetState("state #3");
  care_taker.Add(originator.CreateMemento());

  auto memento0 = care_taker.Get(0);

  originator.Reset(memento0);
  LOG("originator recover to state: " << originator.GetState());
}

}

#endif // !__Memento_Memento_Inc_H__
