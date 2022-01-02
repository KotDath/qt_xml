#include "xmlmodel.h"

XMLModel::XMLModel(QObject *parent) : QAbstractItemModel(parent), rootItem(new TreeItem{{tr("")}}), xmlReader(new QXmlStreamReader{}) {
}

QVariant XMLModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }
    if (role != Qt::DisplayRole) {
        return QVariant();
    }
    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    return item->data(index.column());
}

Qt::ItemFlags XMLModel::flags(const QModelIndex& index) const {
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }
    return QAbstractItemModel::flags(index);
}

QVariant XMLModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return rootItem->data(section);
    }

    return QVariant();

}


QModelIndex XMLModel::index(int row, int column, const QModelIndex& parent) const {
    TreeItem* parentItem;
    if (!parent.isValid()) {
        parentItem = rootItem;
    } else {
        parentItem = static_cast<TreeItem*>(parent.internalPointer());
    }
    TreeItem *childItem = parentItem->child(row);
    if (childItem) {
        return createIndex(row, column, childItem);
    }
    return QModelIndex();
}

QModelIndex XMLModel::parent(const QModelIndex& child) const {
    if (!child.isValid()) {
        return QModelIndex{};
    }
    TreeItem *childItem = static_cast<TreeItem*>(child.internalPointer());
    TreeItem *parentItem = childItem->parent();
    if (parentItem == rootItem) {
        return QModelIndex();
    }
    return createIndex(parentItem->rowInParent(), 0, parentItem);
}


int XMLModel::rowCount(const QModelIndex& parent) const {
    TreeItem *parentItem;
    if (parent.column() > 0) {
        return 0;
    }
    if (!parent.isValid()) {
        parentItem = rootItem;
    }else {
        parentItem = static_cast<TreeItem*>(parent.internalPointer());
    }
    return parentItem->rowCount();

}

int XMLModel::columnCount(const QModelIndex& parent) const {
    if (parent.isValid()) {
        return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
    }
    return rootItem->columnCount();
}


void XMLModel::LoadFile(const QString& fileName) {
    QFile* file = new QFile{fileName};

    if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Fail to open";
    }
    xmlReader->setDevice(file);

    auto fileNode = new TreeItem{{fileName}, rootItem};
    rootItem->appendChild(fileNode);
    TreeItem* currentNode = fileNode;

    while (!xmlReader->atEnd() && !xmlReader->hasError()) {
        QXmlStreamReader::TokenType token = xmlReader->readNext();

        if (token == QXmlStreamReader::StartElement) {
            auto node = new TreeItem{{xmlReader->name().toString()}, currentNode};
            currentNode->appendChild(node);
            currentNode = node;
        }

        if (token == QXmlStreamReader::Characters) {
            auto node = new TreeItem{{xmlReader->text().toString()}, currentNode};
            currentNode->appendChild(node);
        }

        if (token == QXmlStreamReader::EndElement) {
            currentNode = currentNode->parent();
        }
    }

    if (xmlReader->hasError()) {
        qDebug() << xmlReader->error();
    }

    file->close();
}

void XMLModel::closeAll() {
    delete rootItem;
    rootItem = new TreeItem{{tr("")}};

}

XMLModel::~XMLModel() {
    delete rootItem;
    delete xmlReader;
}

QModelIndex XMLModel::firstIndex() {
    return index(0, 0);
}
