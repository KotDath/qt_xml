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
    setCentralWidget(treeView);
}

MainWindow::~MainWindow() {
    delete model;
}

void MainWindow::openFile() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Выбор файла для открытия"), "C://", tr("XML файлы (*.xml)"));
    QMessageBox::information(this, "..", fileName);
    model->LoadFile(fileName);
    treeView->reset();
}

void MainWindow::closeAll() {
    model->closeAll();
    treeView->reset();
}

void MainWindow::quit() {
    closeAll();
    close();
}

