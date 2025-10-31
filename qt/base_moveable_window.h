#pragma once

#include <cstdint>

#include <QMainWindow>
#include <QDialog>
#include <QWidget>
#include <QString>
#include <QMouseEvent>

enum moveable_area_dir {
    kDirX,  // 可移动区域位于窗口左侧
    kDirY,  // 可移动区域位于窗口顶部
};

template<typename widget>
class base_movable_window : public widget
{
    static_assert(std::is_base_of<QWidget, widget>::value,
                  "widget derived by QWidget");

public:
    base_movable_window(int32_t restrict_value = 50,
                        moveable_area_dir dir = kDirY,
                        QWidget *parent = nullptr)
        : widget(parent)
    {
        dir_ = dir;
        restrict_value_ = restrict_value;
    }

protected:
    virtual void mouseMoveEvent(QMouseEvent *event) override
    {
        if (enable_move_)
        {
            auto dx = event->globalX() - prev_pos_.x();
            auto dy = event->globalY() - prev_pos_.y();
            this->move(this->x() + dx, this->y() + dy);

            prev_pos_ = event->globalPos();
        }
    }

    virtual void mousePressEvent(QMouseEvent *event) override
    {
        auto v = dir_ == moveable_area_dir::kDirY ? event->y() : event->x();
        if (v < restrict_value_ && event->button() == Qt::LeftButton)
        {
            enable_move_ = true;
            prev_pos_ = event->globalPos();
        }
    }

    virtual void mouseReleaseEvent(QMouseEvent *event) override
    {
        enable_move_ = false;
    }

protected:
    // 窗口顶部或左侧可以拖拽的区域高度、宽度
    int32_t restrict_value_{ 0 };
    moveable_area_dir dir_{ moveable_area_dir::kDirY };
    bool enable_move_{ false };
    QPoint prev_pos_;
};