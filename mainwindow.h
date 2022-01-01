#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QtXml>
#include <QTreeView>

#include "xmlmodel.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void openFile();
    void closeAll();
    void quit();

private:
    XMLModel* model;
    QTreeView* treeView;
};
#endif // MAINWINDOW_H
