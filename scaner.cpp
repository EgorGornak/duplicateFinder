#include <scaner.h>

#include <QDirIterator>
#include <QDebug>



void Scaner::scan_directory(QString const &directory) {
    emit resetFiles();
    QDirIterator it(directory, QDirIterator::Subdirectories);

    while(it.hasNext()) {
        if (isCancel) {
            return;
        }
        QFileInfo tmp = it.next();
        emit addNewFile(tmp);
    }
}

void Scaner::setCancel(bool value) {
    isCancel = value;
}
