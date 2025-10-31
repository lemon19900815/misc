#pragma once

#include <QWidget>

class MyListView : public QWidget
{
    Q_OBJECT

public:
    MyListView(QWidget* parent = nullptr);

private slots:
    void insertData();
    void deleteData();
    void showData();

private:
    class QStringListModel* model;
    class QListView* listView;
};
