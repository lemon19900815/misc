#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "mylistview.h"
#include "filesystemwidget.h"
#include "mylistviewdelegate.h"
#include "sortview.h"
#include "currencymodel.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    setCentralWidget(new MyListView(this));
//    setCentralWidget(new FileSystemWidget(this));
//    setCentralWidget(new MyListViewDelegate(this));
//    setCentralWidget(new SortView(this));
    setCentralWidget(new CurrencyView(this));
}

MainWindow::~MainWindow()
{
    delete ui;
}

