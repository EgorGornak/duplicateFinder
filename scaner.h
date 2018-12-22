#ifndef SCAN_H
#define SCAN_H

#include <QObject>
#include <QFileInfo>


class Scaner : public QObject {
    Q_OBJECT

signals:
    void addNewFile(QFileInfo const &fileInfo);
    void resetFiles();


public slots:
    void scan_directory(QString const &dir);
    void setCancel(bool value);

private:
    bool isCancel;
};


#endif // SCAN_H
