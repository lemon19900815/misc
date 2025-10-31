#ifndef SORTVIEW_H
#define SORTVIEW_H

#include <QWidget>
#include <QListView>
#include <QStringListModel>
#include <QSortFilterProxyModel>
#include <QComboBox>

class SortView : public QWidget
{
    Q_OBJECT

public:
    SortView(QWidget* parent = nullptr);

private:
    QListView *view;
    QStringListModel *model;
    QSortFilterProxyModel *modelProxy;
    QComboBox *syntaxBox;

private slots:
    void filterChanged(const QString &text);
};

#endif // SORTVIEW_H
