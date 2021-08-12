#include "file_manager.h"
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>

// ====================================================================================================

QString file_manager::currentFile;

// ====================================================================================================

file_manager::file_manager()
{

}

// ====================================================================================================

void file_manager::clearFileName()
{
    currentFile = "";
}

// ====================================================================================================

e_file_state file_manager::saveImage(QImage &image, QWidget *parent)
{
    return saveImage(image, false, parent);
}

// ====================================================================================================

e_file_state file_manager::saveImage(QImage &image, bool saveAs, QWidget * parent)
{
    QString fileName;
    if (currentFile.isEmpty() /*|| currentFile != fileName*/ || saveAs)
    {
        //fileName = QFileDialog::getSaveFileName(parent, "Save Image");
        QFileDialog dialog(parent, "Save Image");
        dialog.setAcceptMode(QFileDialog::AcceptSave);
        dialog.setFileMode(QFileDialog::AnyFile);
        dialog.setNameFilter("*.png");

        if (dialog.exec())
        {
            QStringList fileNames = dialog.selectedFiles();
            if (fileNames.size() > 0)
            {
                for (QString t : fileNames)
                {
                    fileName = t;
                    break;
                }
            }
            if (!fileName.isEmpty())
            {
                if (!fileName.contains(".png", Qt::CaseSensitivity::CaseInsensitive))
                {
                    fileName += ".png";
                }
                if (image.save(fileName))
                {
                    currentFile = fileName;
                    //qInfo() << "SAVE AS | currentFile = " << currentFile;
                    return e_file_state::saved;
                }
            }
            return e_file_state::error;
        }
        return e_file_state::closed;
    }
    else { fileName = currentFile; }

    if (!fileName.isEmpty())
    {
        if (image.save(fileName))
        {
            return e_file_state::saved;
        }
    }
    return e_file_state::error;

}

// ====================================================================================================

e_file_state file_manager::openImage(QImage &image, QWidget * parent)
{
    QFileDialog dialog(parent, "Open Image");
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilter("*.png");

    QString fileName;
    if (dialog.exec())
    {
        QStringList fileNames = dialog.selectedFiles();
        if (fileNames.size() > 0)
        {
            for (QString t : fileNames)
            {
                fileName = t;
                break;
            }
        }
        if (!fileName.isEmpty())
        {
            if (image.load(fileName))
            {
                currentFile = fileName;
                return e_file_state::opened;
            }
        }
        return e_file_state::error;
    }

    // vracia true, len z dovodu, ze uzivatel mohol zrusit vyber obrazka a nie je dovood vracat false
    return e_file_state::closed;
}

// ====================================================================================================

e_file_state file_manager::openImage(QImage &image, QString path)
{
    QFileDialog dialog(nullptr, "Open Image");
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilter("*.png");
    dialog.setDirectory(path);

    QString fileName;
    if (dialog.exec())
    {
        QStringList fileNames = dialog.selectedFiles();
        if (fileNames.size() > 0)
        {
            for (QString t : fileNames)
            {
                fileName = t;
                break;
            }
        }
        if (!fileName.isEmpty())
        {
            if (image.load(fileName))
            {
                return e_file_state::opened;
            }
        }
        return e_file_state::error;
    }

    // vracia true, len z dovodu, ze uzivatel mohol zrusit vyber obrazka a nie je dovood vracat false
    return e_file_state::closed;
}

// ====================================================================================================

