#pragma once

#include <QWidget>
#include <QStyledItemDelegate>
#include <QDebug>
#include <QSpinBox>

class MyListViewDelegate : public QWidget
{
    Q_OBJECT

public:
    MyListViewDelegate(QWidget* parent = nullptr);

private:
    class QStringListModel* model;
    class QListView* listView;
};

class SpinBoxDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    SpinBoxDelegate(QObject *parent = 0) : QStyledItemDelegate(parent) {
        qDebug() << __FUNCTION__;
    }

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;
};

class MySpinBox : public QSpinBox
{
    Q_OBJECT

public:
    MySpinBox(QWidget* parent = nullptr) : QSpinBox(parent) {
        qDebug() << __FUNCTION__;
    }
    ~MySpinBox() {
        qDebug() << __FUNCTION__;
    }
};
