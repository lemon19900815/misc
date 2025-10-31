#include "currencymodel.h"

CurrencyModel::CurrencyModel(QObject *parent)
    : QAbstractTableModel{parent}
{
}

int CurrencyModel::rowCount(const QModelIndex & parent) const
{
    return currencyMap.count();
}

int CurrencyModel::columnCount(const QModelIndex & parent) const
{
    return currencyMap.count();
}

QVariant CurrencyModel::headerData(int section, Qt::Orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
    return currencyAt(section);
}

QString CurrencyModel::currencyAt(int offset) const
{
    return (currencyMap.begin() + offset).key();
}

void CurrencyModel::setCurrencyMap(const QMap<QString, double> &map)
{
    beginResetModel();
    currencyMap = map;
    endResetModel();
}

QVariant CurrencyModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::TextAlignmentRole) {
        return int(Qt::AlignRight | Qt::AlignVCenter);
    } else if (role == Qt::DisplayRole) {
        QString rowCurrency = currencyAt(index.row());
        QString columnCurrency = currencyAt(index.column());
        if (currencyMap.value(rowCurrency) == 0.0) {
            return "####";
        }
        double amount = currencyMap.value(columnCurrency)
                        / currencyMap.value(rowCurrency);
        return QString("%1").arg(amount, 0, 'f', 4);
    }
    return QVariant();
}

/////////////////////////////////////////////////////////////////////////////////
CurrencyView::CurrencyView(QWidget *parent)
    : QTableView(parent)
{
    QMap<QString, double> data;
    data["USD"] = 1.0000;
    data["CNY"] = 0.1628;
    data["GBP"] = 1.5361;
    data["EUR"] = 1.2992;
    data["HKD"] = 0.1289;

    auto model = new CurrencyModel(this);
    model->setCurrencyMap(data);

    setModel(model);
}
