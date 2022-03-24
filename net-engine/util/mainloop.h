#ifndef __MAIN_LOOP_INC_H__
#define __MAIN_LOOP_INC_H__

#include "timehelp.h"

#include <functional>
#include <cassert>
#include <algorithm>

extern util::Time gtime;

namespace util
{
  // Ĭ��Ϊ10ms
  constexpr uint64_t kDefaultFrameTime = 10;

  // ��ѭ�����ƣ�����ִ�м����ʱ��
  class MainLoop
  {
  public:
    MainLoop() {
      frame_count_ = 0;
      last_ms_ = 0;
      schema_ = nullptr;
      frame_time_ = kDefaultFrameTime;
    }

    void registerSchema(uint64_t frame_time, std::function<void()> schema) {
      assert(schema);
      schema_ = schema;

      // ÿ֡����ʱ��
      frame_time_ = std::max<uint64_t>(frame_time, kDefaultFrameTime);
    }

    void update() {
      auto elapsed = gtime.current() - last_ms_;
      if (elapsed >= frame_time_)
      {
        if (schema_) schema_();

        ++frame_count_;
        last_ms_ = gtime.current();
      }
    }

    uint64_t frame_count() {
      return frame_count_;
    }

  private:
    // ÿ֡���м��ʱ��
    uint64_t frame_time_;
    // ��ѭ��������
    std::function<void()> schema_;

    // ���������е�֡��
    uint64_t frame_count_;
    // �ϴμ�¼��ʱ��
    uint64_t last_ms_;
  };
}

#endif // !__MAIN_LOOP_INC_H__
