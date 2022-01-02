#include "bolddelegate.h"

BoldDelegate::BoldDelegate(QObject* parent) : QStyledItemDelegate(parent), focused() {

}

void BoldDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);
    opt.font.setBold(isBold(index));
    QStyledItemDelegate::paint(painter, opt, index);
}

bool BoldDelegate::isBold(const QModelIndex& index) const {
    return index == focused;
}

void BoldDelegate::setFocus(const QModelIndex& focus) {
    qDebug() << focus.row() << focus.column() << focus.data().toString();
    focused = focus;
}
