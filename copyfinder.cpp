#include <copyfinder.h>
#include <QFileInfo>
#include <QDebug>
#include <QCryptographicHash>



void CopyFinder::addNewFile(QFileInfo const &fileInfo) {
    if (fileInfo.isFile()) {
        filesInfo.push_back(fileInfo);
        totalSize += fileInfo.size();
    }
}


void CopyFinder::startFind() {
    qint64 currSize = 0;
    qint64 finishedFiles = 0;
    QMap<qint64, QFileInfoList> filesBySize;

    for (auto file: filesInfo) {
        if (isCancel) {
            emit setProgressBar(100);
            return;
        }
        filesBySize[file.size()].push_back(file);
    }


    for (auto const &listFiles: filesBySize) {
        if (isCancel) {
            emit setProgressBar(100);
            return;
        }
        if (listFiles.size() > 1) {
            findDuplicateWithEqualSize(listFiles);
        }

        for (auto file: listFiles) {
            currSize += file.size();
        }
        finishedFiles += listFiles.size();


        int setProgressBarValue = static_cast<int>(30 * currSize / totalSize) + static_cast<int>(70 * finishedFiles / filesInfo.size());
        emit setProgressBar(setProgressBarValue);
    }
}

void CopyFinder::findDuplicateWithEqualSize(QFileInfoList const &listFiles) {
    QCryptographicHash hash(QCryptographicHash::Sha3_256);
    QMap<QByteArray, QFileInfoList> filesByHash;

    for (auto const &fileInfo: listFiles) {
        hash.reset();

        QFile file(fileInfo.filePath());
        if (file.open(QIODevice::ReadOnly)) {
            hash.addData(&file);
            filesByHash[hash.result()].push_back(fileInfo);
        }
        if (isCancel) {
            emit setProgressBar(100);
            return;
        }
    }

    for (auto const &fileList: filesByHash) {
        if (isCancel) {
            emit setProgressBar(100);
            return;
        }
        if (fileList.size() > 1) {
            QTreeWidgetItem* group = new QTreeWidgetItem();
            group->setText(0, QString::number(fileList.size()) + " files, each size = " + sizeHuman(fileList.back().size()));

            for (auto const &file: fileList) {

                QTreeWidgetItem* childItem = new QTreeWidgetItem();

                QString path;
                for (int i = currentDirectory.size() + 1; i < file.filePath().size(); i++) {
                    path += file.filePath()[i];
                }
                childItem->setText(0, path);

                group->addChild(childItem);
            }
            emit addDuplicateGroup(group);
        }
    }

}

void CopyFinder::resetFiles() {
   filesInfo.clear();
   totalSize = 0;
}

void CopyFinder::setCancel(bool value) {
    isCancel = value;
}

QString CopyFinder::sizeHuman(qint64 size) {
    float num = static_cast<float>(size);
    QStringList list;
    list << "KB" << "MB" << "GB" << "TB";

    QStringListIterator i(list);
    QString unit("bytes");

    while(num >= 1024.f && i.hasNext())
     {
        unit = i.next();
        num /= 1024.f;
    }
    return QString().setNum(num,'f',2) + " " + unit;
}

void CopyFinder::setCurrentDirectory(QString const &dir) {
    currentDirectory = dir;
}
