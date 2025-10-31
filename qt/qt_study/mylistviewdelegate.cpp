#include "mylistviewdelegate.h"

#include <QListView>
#include <QStringList>
#include <QStringListModel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QInputDialog>
#include <QLineEdit>
#include <QMessageBox>
#include <QDebug>
#include <QSpinBox>
#include <QApplication>

MyListViewDelegate::MyListViewDelegate(QWidget* parent)
    : QWidget(parent)
{
    QStringList data;
    data << "0" << "1" << "2";
    model = new QStringListModel(this);
    model->setStringList(data);

    listView = new QListView(this);
    listView->setModel(model);
    listView->setItemDelegate(new SpinBoxDelegate(listView));

    QPushButton *btnShow = new QPushButton(tr("Show Model"), this);
    connect(btnShow, SIGNAL(clicked()), this, SLOT(showModel()));
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(btnShow);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(listView);
    layout->addLayout(buttonLayout);
    setLayout(layout);
}

QWidget *SpinBoxDelegate::createEditor(QWidget *parent,
                                       const QStyleOptionViewItem & /* option */,
                                       const QModelIndex & /* index */) const
{
//    QApplication::style()->drawControl()
    qDebug() << __FUNCTION__;
    QSpinBox *editor = new MySpinBox(parent);
    editor->setMinimum(0);
    editor->setMaximum(100);
    return editor;
}

void SpinBoxDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    int value = index.model()->data(index, Qt::EditRole).toInt();
    QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
    spinBox->setValue(value);
    qDebug() << __FUNCTION__ << "->value: " << value;
}

void SpinBoxDelegate::setModelData(QWidget *editor,
                                   QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
//    spinBox->interpretText();
    int value = spinBox->value();
    model->setData(index, value, Qt::EditRole);
    qDebug() << __FUNCTION__ << "->value: " << value;
//    emit this->closeEditor();
}

void SpinBoxDelegate::updateEditorGeometry(QWidget *editor,
                                           const QStyleOptionViewItem &option,
                                           const QModelIndex &index) const
{
    qDebug() << __FUNCTION__;
    editor->setGeometry(option.rect);
}
