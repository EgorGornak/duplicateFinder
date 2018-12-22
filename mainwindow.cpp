#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCommonStyle>
#include <QDesktopWidget>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QLabel>
#include <QDirIterator>
#include <QDebug>
#include <QtConcurrent/QtConcurrent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->treeWidget->setUniformRowHeights(true);

    connect(ui->actionScan, &QAction::triggered, this, &MainWindow::selectDirectory);
    connect(ui->actionFindDuplicate, &QAction::triggered, this, &MainWindow::findCopy);
    connect(ui->actionCancel, &QAction::triggered, this, &MainWindow::cancelFindDuplicate);
    connect(ui->actionDeleteFiles, &QAction::triggered, this, &MainWindow::deleteFiles);
    connect(&scaner, &Scaner::addNewFile, &copyFinder, &CopyFinder::addNewFile);
    connect(&scaner, &Scaner::resetFiles, &copyFinder, &CopyFinder::resetFiles);
    connect(&copyFinder, &CopyFinder::addDuplicateGroup, this, &MainWindow::addDuplicateGroup);
    connect(&copyFinder, &CopyFinder::setProgressBar, this, &MainWindow::setProgressBar);
}

void MainWindow::findCopy() {
    ui->treeWidget->clear();

    scaner.setCancel(false);
    copyFinder.setCancel(false);
    ui->progressBar->setValue(0);
    scaner.scan_directory(currentDirectory);
    QFuture <void> future = QtConcurrent::run(&copyFinder, &CopyFinder::startFind);
}


void MainWindow::selectDirectory() {
    QString dir = QFileDialog::getExistingDirectory(this, "Select Directory for Scanning",
                                                    QString(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    setCurrentDirectory(dir);
    copyFinder.setCurrentDirectory(dir);
    ui->statusBar->showMessage("Current directory = " + dir);
    scanDirectory(dir);
}



void MainWindow::scanDirectory(QString const& dir) {
    ui->treeWidget->clear();
    setWindowTitle(QString("Directory Content - %1").arg(dir));
    QDir d(dir);
    QFileInfoList list = d.entryInfoList();
    for (QFileInfo file_info : list)
    {
        QTreeWidgetItem* item = new QTreeWidgetItem(ui->treeWidget);
        item->setText(0, file_info.fileName());
        ui->treeWidget->addTopLevelItem(item);
    }
}

void MainWindow::setCurrentDirectory(QString const& dir) {
    currentDirectory = dir;
}

void MainWindow::setProgressBar(int value) {
    ui->progressBar->setValue(value);
}

void MainWindow::addDuplicateGroup(QTreeWidgetItem *group) {
    ui->treeWidget->addTopLevelItem(group);
}

void MainWindow::cancelFindDuplicate(){
    copyFinder.setCancel(true);
    scaner.setCancel(true);
}

void MainWindow::deleteFiles() {
    QList<QTreeWidgetItem *> selectedItems = ui->treeWidget->selectedItems();
    QMessageBox::StandardButton dialog = QMessageBox::question(this, "Delete", "Are you sure?");
    if (dialog == QMessageBox::Yes) {
        for (auto item: selectedItems) {
            if (item->parent() == nullptr) {
                continue;
            }
            QString path = currentDirectory + '/' + item->text(0);
            QFile file(path);
            qint64 fileSize = file.size();
            if (file.remove()) {
                item->parent()->setText(0, QString::number(item->parent()->childCount() - 1) + " files, size = " + QString::number(fileSize));
                item->parent()->removeChild(item);

            }
        }
    }
}



MainWindow::~MainWindow() {
    delete ui;
}


