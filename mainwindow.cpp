#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    auto menuFile = new QMenu{tr("Файл")};

    auto actionOpen = new QAction{tr("Открыть"), this};
    actionOpen->setShortcut(QKeySequence::Open);
    menuFile->addAction(actionOpen);
    connect(actionOpen, SIGNAL(triggered()), this, SLOT(openFile()));

    auto actionCloseAll = new QAction{tr("Закрыть все"), this};
    actionCloseAll->setShortcut(QKeySequence::Close);
    menuFile->addAction(actionCloseAll);
    connect(actionCloseAll, SIGNAL(triggered()), this, SLOT(closeAll()));

    auto actionQuit = new QAction{tr("Выход"), this};
    actionQuit->setShortcut(QKeySequence::Quit);
    menuFile->addAction(actionQuit);
    connect(actionQuit, SIGNAL(triggered()), this, SLOT(quit()));

    menuBar()->addMenu(menuFile);

    model = new XMLModel{};
    treeView = new QTreeView{};
    treeView->setModel(model);
    treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(treeView, SIGNAL(customContextMenuRequested(QPoint)), SLOT(customMenuRequested(QPoint)));
    setCentralWidget(treeView);
}

MainWindow::~MainWindow() {
    delete model;
    delete treeView;
}

void MainWindow::openFile() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Выбор файла для открытия"), QDir::homePath(), tr("XML файлы (*.xml)"));
    model->LoadFile(fileName);
    treeView->reset();
    model->setData(model->firstIndex(), QVariant(), Qt::FontRole);
}

void MainWindow::closeAll() {
    model->closeAll();
    treeView->reset();
}

void MainWindow::quit() {
    closeAll();
    close();
}


void MainWindow::customMenuRequested(QPoint pos) {
    QMenu *menu=new QMenu(this);
    auto actionActive = new QAction{tr("Сделать активным"), this};
    menu->addAction(actionActive);
    connect(actionActive, &QAction::triggered, this, &MainWindow::makeActive);
    menu->popup(treeView->viewport()->mapToGlobal(pos));
}

void MainWindow::makeActive() {
    auto currentIndex = treeView->currentIndex();
    model->setData(currentIndex, QVariant(), Qt::FontRole);
}
