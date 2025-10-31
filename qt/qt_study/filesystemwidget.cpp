#include "filesystemwidget.h"

#include <QFileSystemModel>
#include <QTreeView>
#include <QPushButton>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QMessageBox>
#include <QHeaderView>
#include <QDebug>

FileSystemWidget::FileSystemWidget(QWidget *parent) :
    QWidget(parent)
{
    model = new QFileSystemModel(this);
    model->setRootPath(QDir::currentPath());

    // 底层加载扫描文件夹是异步进行的，这里直接获取是为0
    // model->rowCount(parentIndex);

    connect(model, &QFileSystemModel::directoryLoaded, this, [&](const QString& path) {
        qDebug() << "directoryLoaded: " << path;

        auto parentIndex = model->index(QDir::currentPath());
        qDebug() << parentIndex;

        auto numRows = model->rowCount(parentIndex);
        for(auto i = 0; i < numRows; ++i)
        {
            auto index = model->index(i, 0, parentIndex);
            auto text = model->data(index).toString();
            qDebug() << index << "->" << text;
        }
    });

    treeView = new QTreeView(this);
    treeView->setModel(model);
    treeView->setRootIndex(model->index(QDir::currentPath()));

    // 激活排序
    treeView->setSortingEnabled(true);
    // 数据view设置数据排序列和规则（升序/降序）
    treeView->sortByColumn(0, Qt::SortOrder::AscendingOrder);

    // @Note：下面的代码暂时不知道有什么作用
//    auto headerView = treeView->header();
//    headerView->setStretchLastSection(true);
//    headerView->setSortIndicator(0, Qt::DescendingOrder);
//    headerView->setSortIndicatorShown(true);
//    headerView->setSectionsClickable(true);

    QPushButton *mkdirButton = new QPushButton(tr("Make Directory..."), this);
    QPushButton *rmButton = new QPushButton(tr("Remove"), this);
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(mkdirButton);
    buttonLayout->addWidget(rmButton);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(treeView);
    layout->addLayout(buttonLayout);

    setLayout(layout);
    setWindowTitle("File System Model");

    connect(mkdirButton, SIGNAL(clicked()),
            this, SLOT(mkdir()));
    connect(rmButton, SIGNAL(clicked()),
            this, SLOT(rm()));
}

void FileSystemWidget::mkdir()
{
    QModelIndex index = treeView->currentIndex();//.parent();
    if (!index.isValid()) {
        return;
    }
    qDebug() << index.data();
    index.model();

    QString dirName = QInputDialog::getText(this,
                                            tr("Create Directory"),
                                            tr("Directory name"));
    if (!dirName.isEmpty()) {
        if (!model->mkdir(index, dirName).isValid()) {
            QMessageBox::information(this,
                                     tr("Create Directory"),
                                     tr("Failed to create the directory"));
        }
    }
}

void FileSystemWidget::rm()
{
    QModelIndex index = treeView->currentIndex();
    if (!index.isValid()) {
        return;
    }
    bool ok;
    if (model->fileInfo(index).isDir()) {
        ok = model->rmdir(index);
    } else {
        ok = model->remove(index);
    }
    if (!ok) {
        QMessageBox::information(this,
                                 tr("Remove"),
                                 tr("Failed to remove %1").arg(model->fileName(index)));
    }
}
