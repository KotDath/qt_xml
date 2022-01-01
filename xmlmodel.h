#ifndef XMLMODEL_H
#define XMLMODEL_H

#include <QAbstractItemModel>
#include <QFile>
#include <QXmlStreamReader>
#include <QMessageBox>
#include <QtCore5Compat/QTextCodec>
#include <QModelIndex>

#include "treeitem.h"

class XMLModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit XMLModel(QObject *parent = nullptr);

    QVariant data(const QModelIndex& index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex& p_idx) const override;
    QVariant headerData(int section, Qt::Orientation p_orient, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex& child) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    void LoadFile(const QString& fileName);
    void closeAll();

    ~XMLModel();
private:

    QXmlStreamReader* xmlReader;
    TreeItem* rootItem;
};

#endif // XMLMODEL_H
