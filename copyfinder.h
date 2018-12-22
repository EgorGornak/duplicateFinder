#ifndef COPYFINDER_H
#define COPYFINDER_H

#include <QObject>
#include <QFileInfo>
#include <QFileInfoList>
#include <QTreeWidget>

class CopyFinder : public QObject {
    Q_OBJECT

signals:
    void addDuplicateGroup(QTreeWidgetItem *group);
    void setProgressBar(int value);

public slots:
    void addNewFile(QFileInfo const &fileInfo);
    void resetFiles();
    void startFind();
    void setCancel(bool value);
    void setCurrentDirectory(QString const &directory);

private:
    QString sizeHuman(qint64 size);
    void findDuplicateWithEqualSize(QFileInfoList const &filesInfo);
    QFileInfoList filesInfo;
    QString currentDirectory;
    qint64 totalSize = 0;
    bool isCancel;
};


#endif // COPYFINDER_H
