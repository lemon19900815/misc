#pragma once

#include <QWidget>

class FileSystemWidget : public QWidget
{
    Q_OBJECT

public:
    FileSystemWidget(QWidget* parent = nullptr);


private slots:
    void mkdir();
    void rm();

private:
    class QFileSystemModel* model;
    class QTreeView* treeView;
};
