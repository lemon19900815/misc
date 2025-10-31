#include "mainwindow.h"

#include <QApplication>
#include <QTreeWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    QTreeWidget treeWidget;
//    treeWidget.setColumnCount(1);
//    treeWidget.setHeaderHidden(true);

//    QTreeWidgetItem *root = new QTreeWidgetItem(&treeWidget,
//                                                QStringList(QString("Root")));
//    new QTreeWidgetItem(root, QStringList(QString("Leaf 1")));
//    QTreeWidgetItem *leaf2 = new QTreeWidgetItem(root, QStringList(QString("Leaf 2")));
//    leaf2->setCheckState(0, Qt::Checked);

//    QList<QTreeWidgetItem *> rootList;
//    rootList << root;
//    treeWidget.insertTopLevelItems(0, rootList);

//    treeWidget.show();

//    QTreeWidget treeWidget;

//    QStringList headers;
//    headers << "Name" << "Number";
//    treeWidget.setHeaderLabels(headers);

//    QStringList rootTextList;
//    rootTextList << "Root" << "0";
//    QTreeWidgetItem *root = new QTreeWidgetItem(&treeWidget, rootTextList);

//    new QTreeWidgetItem(root, QStringList() << QString("Leaf 1") << "1");
//    QTreeWidgetItem *leaf2 = new QTreeWidgetItem(root,
//                                                 QStringList() << QString("Leaf 2") << "2");
//    leaf2->setCheckState(0, Qt::Checked);

//    QList<QTreeWidgetItem *> rootList;
//    rootList << root;
//    treeWidget.insertTopLevelItems(0, rootList);

//    treeWidget.show();

    MainWindow w;
    w.show();
    return a.exec();
}
