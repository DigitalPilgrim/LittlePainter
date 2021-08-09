#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "little_painter.h"

#include <QColorDialog>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_cPainter(new Little_Painter(this))
{
    ui->setupUi(this);
    createActions();
    createMenus();
    setCentralWidget(m_cPainter);

    setWindowTitle("Little Painter");
    resize(500, 500);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_cPainter;
}

void MainWindow::newImage()
{

}

void MainWindow::clearImage()
{
    m_cPainter->ClearImage();
}

void MainWindow::open()
{

}

void MainWindow::save()
{

}

void MainWindow::saveAs()
{

}

void MainWindow::changePenColor()
{
    QColor ncolor = QColorDialog::getColor(m_cPainter->penColor());
    if (ncolor.isValid())
    {
        m_cPainter->setPenColor(ncolor);
    }
}

void MainWindow::changePenWidth()
{
    bool ok = false;
    int nw = QInputDialog::getInt(this, "Little Painter", "Select pen widht: ", m_cPainter->penWidth(), 1, 50, 1, &ok);
    if (ok)
    {
        m_cPainter->setPenWidth(nw);
    }
}

void MainWindow::createActions()
{
    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::newImage);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::open);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::save);
    connect(ui->actionSave_As, &QAction::triggered, this, &MainWindow::saveAs);
    connect(ui->actionChange_a_Pen_Size, &QAction::triggered, this, &MainWindow::changePenWidth);
    connect(ui->actionPen_Color, &QAction::triggered, this, &MainWindow::changePenColor);
    connect(ui->actionClear_Image, &QAction::triggered, m_cPainter, &Little_Painter::ClearImage);
}

void MainWindow::createMenus()
{

}

bool MainWindow::maybeSave()
{
    return false;
}

bool MainWindow::saveFile(const QByteArray &fileFormat)
{
    return false;
}



