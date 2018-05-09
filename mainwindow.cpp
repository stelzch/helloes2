#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    glWidget = new MOpenGLWidget();

    ui->verticalLayout->addWidget(glWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}
