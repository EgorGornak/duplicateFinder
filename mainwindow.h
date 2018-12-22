#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <scaner.h>
#include <copyfinder.h>
#include <QTreeWidget>
#include <QFuture>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void setCurrentDirectory(QString const &dir);
    ~MainWindow();

private slots:
    void selectDirectory();
    void findCopy();
    void scanDirectory(QString const& dir);
    void addDuplicateGroup(QTreeWidgetItem *group);
    void setProgressBar(int value);
    void cancelFindDuplicate();
    void deleteFiles();

private:
    Ui::MainWindow *ui;
    Scaner scaner;
    CopyFinder copyFinder;
    QString currentDirectory;
};

#endif // MAINWINDOW_H
