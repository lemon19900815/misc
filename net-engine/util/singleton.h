#ifndef __Singleton_Inc_H__
#define __Singleton_Inc_H__

#include <memory>

namespace util
{
  template<typename cls>
  class singleton
  {
  protected:
    singleton() {
      instance_ = nullptr;
    }
    virtual ~singleton() {

    }

  public:
    static std::shared_ptr<cls> instance() {
      if (!instance_) {
        instance_ = std::make_shared<cls>();
      }
      return instance_;
    }

  private:
    static std::shared_ptr<cls> instance_;
  };
}

template<typename cls>
std::shared_ptr<cls> util::singleton<cls>::instance_ = nullptr;

#endif // __Singleton_Inc_H__
