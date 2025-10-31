#include "mylistview.h"

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

MyListView::MyListView(QWidget* parent)
    : QWidget(parent)
{
    QStringList data;
    data << "0" << "1" << "2";
    model = new QStringListModel(this);
    model->setStringList(data);

    listView = new QListView(this);
    listView->setModel(model);

    QPushButton *btnShow = new QPushButton(tr("Show Model"), this);
    connect(btnShow, SIGNAL(clicked()),
            this, SLOT(showModel()));
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(btnShow);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(listView);
    layout->addLayout(buttonLayout);
    setLayout(layout);
}

void MyListView::insertData()
{
    bool isOK;
    QString text = QInputDialog::getText(this, "Insert",
                                         "Please input new data:",
                                         QLineEdit::Normal,
                                         "You are inserting new data.",
                                         &isOK);
    if (isOK) {
//        int row = listView->currentIndex().row();
//        model->insertRows(row, 1);
//        QModelIndex index = model->index(row);
//        model->setData(index, text);
//        listView->setCurrentIndex(index);
//        listView->edit(index);

        QModelIndex currIndex = listView->currentIndex();
        model->insertRows(currIndex.row(), 1);
        model->setData(currIndex, text);
//        listView->edit(currIndex);

        auto newIndex = listView->currentIndex();
        qDebug() << "currIndex: " << currIndex;
        qDebug() << "newIndex: " << newIndex;
    }
}

void MyListView::deleteData()
{
    if(model->rowCount() > 1)
    {
        model->removeRow(listView->currentIndex().row());
    }
}

void MyListView::showData()
{
    QString str;
    for(auto& s: model->stringList()) {
        str += s + "\n";
    }
    QMessageBox::information(this, "Data", str);
}
