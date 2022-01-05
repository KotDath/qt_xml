#ifndef BOLDDELEGATE_H
#define BOLDDELEGATE_H

#include <QStyledItemDelegate>
#include "treeitem.h"


//!!! Если вы хотите сделать жирным текст в QTreeView, то городить делегат излишне и его нужно убрать.
//!!! Есть Qt::FontRole и QFont, который можно установтить в setData
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
