#ifndef TREEITEM_H
#define TREEITEM_H

#include <QVector>
#include <QVariant>

class TreeItem {
public:
    explicit TreeItem(const QVector<QVariant> &data, TreeItem *parentItem = nullptr);
    ~TreeItem();
    TreeItem* child(int row);
    void appendChild(TreeItem* item);
    int rowCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    TreeItem* parent() const;
    int rowInParent() const;
private:
    QVector<TreeItem*> itemChildren;
    QVector<QVariant> itemData;
    TreeItem* itemParent;
};

#endif // TREEITEM_H
