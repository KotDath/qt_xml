#include "xmlmodel.h"

XMLModel::XMLModel(QObject *parent) : QAbstractItemModel(parent), rootItem(new TreeItem{{tr("")}}),
                                      xmlReader(new QXmlStreamReader{}), transformText(), focused(), focusedFont() {
    QFile file{":/src/config.json"};
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Fail to open";
    }
    QByteArray data = file.readAll();
    transformText = QJsonDocument::fromJson(data).object();
    file.close();

}

QVariant XMLModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }
    if (role == Qt::FontRole && index == focused) { // First column items are bold.
        return focusedFont;
    }
    if (role != Qt::DisplayRole) {
        return QVariant();
    }
    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    return item->data(index.column());
}

bool XMLModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!index.isValid()) {
        return false;
    }
    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    if (role == Qt::EditRole) {
        return item->setData(index.column(), value);
    }

    if (role == Qt::FontRole) {
        focused = index;
        focusedFont = value.value<QFont>();
        return true;
    }
    return false;
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
    if (parentItem == rootItem || childItem == rootItem) {
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
    QFile file{fileName};
    
    
    QStack<int> counterStack;
    QStack<int> layerStack;


    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Fail to open";
    }
    xmlReader->setDevice(&file);

    auto fileNode = new TreeItem{{fileName}, rootItem};
    rootItem->appendChild(fileNode);
    TreeItem* currentNode = fileNode;

    int currentLayer = 0;
    while (!xmlReader->atEnd() && !xmlReader->hasError()) {
        QXmlStreamReader::TokenType token = xmlReader->readNext();

        if (token == QXmlStreamReader::StartElement) {
            ++currentLayer;

            auto text = xmlReader->name().toString();

            auto formatText = transformText.value(text);
            if (!formatText.isUndefined()) {
                text = formatText.toString();
            }

            if (text == "array") {
                if (layerStack.isEmpty() || currentLayer != layerStack.top()) {
                    counterStack.append(0);
                    layerStack.append(currentLayer);
                }
                text = QString("Элемент %1").arg(counterStack.top() + 1);
                ++counterStack.top();


            }

            auto node = new TreeItem{{text}, currentNode};
            currentNode->appendChild(node);
            currentNode = node;
        }

        if (token == QXmlStreamReader::Characters) {
            auto node = new TreeItem{{xmlReader->text().toString()}, currentNode};
            currentNode->appendChild(node);
        }

        if (token == QXmlStreamReader::EndElement) {
            --currentLayer;
            currentNode = currentNode->parent();
            auto text = xmlReader->name().toString();
            if (text == "array" && currentLayer + 1 != layerStack.top()) {
                counterStack.pop();
                layerStack.pop();
            }
        }
    }

    if (xmlReader->hasError()) {
        rootItem->removeChild();
        delete fileNode;
        QMessageBox message(QMessageBox::Critical, tr("Ошибка парсинга!"),
                            tr("Данный XML документ невозможно отобразить!"),
                            QMessageBox::Ok);
        message.exec();
    } else {
        QMessageBox message(QMessageBox::Information, tr("Документ добавлен!"),
                            tr("Документ был успешно добавлен в модель!"),
                            QMessageBox::Ok);
        message.exec();
    }

    file.close();
}

void XMLModel::closeAll() { 
    removeRows(0, rootItem->rowCount(), createIndex(-1, -1, nullptr));
}

void XMLModel::close() {
    auto fileTreeNode = static_cast<TreeItem*>(focused.internalPointer());
    if (fileTreeNode) {
        removeRow(fileTreeNode->rowInParent(), focused.parent());
    }
}

XMLModel::~XMLModel() {
    delete rootItem;
    delete xmlReader;
}

QModelIndex XMLModel::firstIndex() {
    return index(0, 0);
}

bool XMLModel::removeRows(int row, int count, const QModelIndex &parent) {
    TreeItem* node = nullptr;
    QModelIndex correctParent;
    if (parent.isValid()) {
        node = static_cast<TreeItem*>(parent.internalPointer());
        correctParent = parent;
    } else {
        node = rootItem;
        correctParent = QModelIndex();
    }
    beginRemoveRows(correctParent, row, row + count - 1);
    bool success = node->removeChilds(row, row + count);
    endRemoveRows();
}
