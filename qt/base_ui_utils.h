#pragma once

#include <QWidget>
#include <QScreen>
#include <QGuiApplication>
#include <QFontMetrics>
#include <QString>
#include <QFont>
#include <QPushButton>

// Linux 下使用cmake编译时
// set(CMAKE_AUTOMOC ON)
// set(CMAKE_AUTORCC ON)
// set(CMAKE_AUTOUIC ON)
// 注意：不要使用ui_base_utils命名格式，moc会自动去查找base_utils.ui文件
namespace base_ui_utils
{

static void set_window_style(QWidget *widget)
{
    // 去掉窗口默认的title栏，并设置背景透明
    widget->setWindowFlags(Qt::FramelessWindowHint);
    widget->setAttribute(Qt::WA_TranslucentBackground);
}

// 设置窗口居中显示
static void set_window_center(QWidget *widget)
{
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry();
    widget->move((screenGeometry.width() - widget->width()) / 2,
                 (screenGeometry.height() - widget->height()) / 2);
}

// 设置按钮最大文本宽度，超出宽度使用"..."替代，并设置tooltip显示
static void set_button_text_max_width(QPushButton &btn, int32_t max_width)
{
    auto text = btn.text();
    btn.setToolTip(text);

    QFontMetrics fontMetrics(btn.font());
    int textWidth = fontMetrics.width(btn.text()); // 计算文本的宽度
    if (textWidth > max_width)
    {
        // 使用省略号替代超出部分
        QString ellipsizedText = fontMetrics.elidedText(text, Qt::ElideRight,
                                                        max_width);
        btn.setText(ellipsizedText); // 更新QLabel的文本内容
    }
}

// 计算文本在字体font下显示需要占用的宽度
static int32_t calc_text_width(const QFont &font, const QString &text)
{
    return QFontMetrics(font).width(text);
}

// 计算QPushButton按钮文本显示宽度
static int32_t calc_btn_text_width(QPushButton *btn)
{
    return calc_text_width(btn->font(), btn->text());
}

// 窗口下的按钮根据翻译内容自适应
// space: 2个按钮之间的空闲区域 [btn1]--space--[btn2]
// margin: 按钮文本到边缘的留白区域，[--margin--text--margin--]
static void adjust_btn_width(QWidget *parent,
                             int32_t space, int32_t margin,
                             const std::vector<QPushButton *> &btns)
{
    if (btns.empty() || !parent)
    {
        return;
    }

    auto btn_width = 0;
    for (auto &btn : btns)
    {
        btn->adjustSize();
        btn_width = std::max<int32_t>(btn_width, calc_btn_text_width(btn));
    }

    auto btn_count = static_cast<int32_t>(btns.size());

    auto space_width = (btn_count - 1) * space;
    auto margin_width = btn_count * margin * 2; // 2侧margin
    auto win_width = btn_width * btn_count + space_width + margin_width;

    parent->resize(win_width, parent->height());
}

} // namespace base_ui_utils
