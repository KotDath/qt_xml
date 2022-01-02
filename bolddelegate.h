#ifndef BOLDDELEGATE_H
#define BOLDDELEGATE_H

#include <QStyledItemDelegate>
#include "treeitem.h"

class BoldDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
   explicit BoldDelegate(QObject* parent = nullptr);

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
private:
    bool isBold(const QModelIndex& index) const;
    QModelIndex focused;

public slots:
    void setFocus(const QModelIndex& focus);
};

#endif // BOLDDELEGATE_H
