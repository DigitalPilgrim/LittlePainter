#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "little_painter.h"
#include "painter_canvas.h"
#include "file_manager.h"

#include <QColorDialog>
#include <QInputDialog>
#include <QDebug>
#include <QLayout>
#include <QMessageBox>

// ------------------------------------------------------------------------------------------------------------

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    //, m_cPainterCanvas(new painter_canvas(this))
    , m_cPainter(new Little_Painter(this))
    //, m_cPainterCanvas(new painter_canvas(this))
{
    ui->setupUi(this);
    createActions();
    createMenus();
    setCentralWidget(m_cPainter);
    //setCentralWidget(m_cPainterCanvas);


    setWindowTitle("Little Painter");
    resize(500, 500);
}

// ------------------------------------------------------------------------------------------------------------

MainWindow::~MainWindow()
{
    delete ui;
    delete m_cPainter;
    //delete m_cPainterCanvas;
}

// ------------------------------------------------------------------------------------------------------------

void MainWindow::newImage()
{

}

// ------------------------------------------------------------------------------------------------------------

void MainWindow::newImage500()
{
    file_manager::clearFileName();
    m_cPainter->setImageSizeAndClear(QSize(500, 500));
}

// ------------------------------------------------------------------------------------------------------------

void MainWindow::newImage1000()
{
    file_manager::clearFileName();
    m_cPainter->setImageSizeAndClear(QSize(1000, 1000));
}

// ------------------------------------------------------------------------------------------------------------

void MainWindow::newImage4000()
{
    file_manager::clearFileName();
    m_cPainter->setImageSizeAndClear(QSize(4000, 4000));
}

// ------------------------------------------------------------------------------------------------------------

void MainWindow::clearImage()
{
    m_cPainter->ClearImage();
}

// ------------------------------------------------------------------------------------------------------------

void MainWindow::open()
{
    if (!m_cPainter->openImage())
    {
        QMessageBox::warning(this, "Error", "Chyba pri Open.\nDaco nie je v poriadku.");
    }
}

// ------------------------------------------------------------------------------------------------------------

void MainWindow::save()
{
    if (!m_cPainter->saveImage())
    {
        QMessageBox::warning(this, "Error", "Chyba pri Save.\nDaco nie je v poriadku.");
    }
}

// ------------------------------------------------------------------------------------------------------------

void MainWindow::saveAs()
{
    if (!m_cPainter->saveImageAs())
    {
        QMessageBox::warning(this, "Error", "Chyba pri Save As.\nDaco nie je v poriadku.");
    }
}

// ------------------------------------------------------------------------------------------------------------

void MainWindow::changePenColor()
{
    QColor ncolor = QColorDialog::getColor(m_cPainter->penColor());
    if (ncolor.isValid())
    {
        m_cPainter->setPenColor(ncolor);
    }
}

// ------------------------------------------------------------------------------------------------------------

void MainWindow::changePenWidth()
{
    bool ok = false;
    int nw = QInputDialog::getInt(this, "Little Painter", "Select pen widht: ", m_cPainter->penWidth(), 10, 500, 10, &ok);
    if (ok)
    {
        m_cPainter->setPenWidth(nw);
    }
}

// ------------------------------------------------------------------------------------------------------------

void MainWindow::changeBrushTransparency()
{
    bool ok = false;
    int nw = QInputDialog::getInt(this
                                   , "Little Painter", "Select Brush Transparency: "
                                   , 100 * m_cPainter->BrushTransparency()
                                   , 1, 100, 1, &ok);
    if (ok)
    {
        m_cPainter->BrushTransparency(static_cast<float>(nw) / 100.0f);
    }
}

// ------------------------------------------------------------------------------------------------------------

void MainWindow::createActions()
{

    // ------------------------------------------------------------------------------------------------------
    // new
    //connect(ui->actionNew, &QAction::triggered, this, &MainWindow::newImage);
    connect(ui->actionNew500, &QAction::triggered, this, &MainWindow::newImage500);
    connect(ui->actionNew1000, &QAction::triggered, this, &MainWindow::newImage1000);
    connect(ui->actionNew4000, &QAction::triggered, this, &MainWindow::newImage4000);
    // ------------------------------------------------------------------------------------------------------
    // open save
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::open);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::save);
    connect(ui->actionSave_As, &QAction::triggered, this, &MainWindow::saveAs);
    // -----------------------------------------------------------------------------------------------------
    connect(ui->actionClear_Image, &QAction::triggered, m_cPainter, &Little_Painter::ClearImage);
    connect(ui->actionClear_Color, &QAction::triggered, m_cPainter, &Little_Painter::ClearColor);
    // -----------------------------------------------------------------------------------------------------
    // /////////////////////////////////////////////////////////////////////////////////////////////////////
    // ------------------------------------------------------------------------------------------------------
    // EDIT
    // ------------------------------------------------------------------------------------------------------
    // Undo Redo
    connect(ui->actionUndo, &QAction::triggered, m_cPainter, &Little_Painter::Undo);
    connect(ui->actionRedo, &QAction::triggered, m_cPainter, &Little_Painter::Redo);
    // ------------------------------------------------------------------------------------------------------
    // Brush properties
    connect(ui->actionChange_a_Pen_Size, &QAction::triggered, this, &MainWindow::changePenWidth);
    connect(ui->actionPen_Color, &QAction::triggered, this, &MainWindow::changePenColor);
    connect(ui->actionBrush_Transparency, &QAction::triggered, this, &MainWindow::changeBrushTransparency);
    // ------------------------------------------------------------------------------------------------------
    // select brush circle
    connect(ui->actionBrush_Circle, &QAction::triggered, m_cPainter, &Little_Painter::SetBrushCircle);
    // ------------------------------------------------------------------------------------------------------
    // select brush texture
    connect(ui->actionBrush_Texture_1, &QAction::triggered, m_cPainter, &Little_Painter::SetBrushTexture);
    connect(ui->actionBrush_Texture_2, &QAction::triggered, m_cPainter, &Little_Painter::SetBrushTexture2);
    connect(ui->actionBrushCustom, &QAction::triggered, m_cPainter, &Little_Painter::SetBrushCustom);
    // ------------------------------------------------------------------------------------------------------
}

// ------------------------------------------------------------------------------------------------------------

void MainWindow::createMenus()
{

}

// ------------------------------------------------------------------------------------------------------------

bool MainWindow::maybeSave()
{
    return false;
}

// ------------------------------------------------------------------------------------------------------------

bool MainWindow::saveFile(const QByteArray &fileFormat)
{
    return false;
}

// ------------------------------------------------------------------------------------------------------------



