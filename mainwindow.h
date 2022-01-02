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
#include "bolddelegate.h"

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
    void customMenuRequested(QPoint pos);
    void makeActive();

private:
    XMLModel* model;
    QTreeView* treeView;
    BoldDelegate* del;
};
#endif // MAINWINDOW_H
