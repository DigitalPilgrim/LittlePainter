#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QList>
#include <QMainWindow>

class Little_Painter;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void newImage();
    void clearImage();
    void open();
    void save();
    void saveAs();
    void changePenColor();
    void changePenWidth();


private:
    Ui::MainWindow *ui;

    void createActions();
    void createMenus();
    bool maybeSave();
    bool saveFile(const QByteArray &fileFormat);

    Little_Painter * m_cPainter;

};
#endif // MAINWINDOW_H
