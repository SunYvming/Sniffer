#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sniffer.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->setTabText(0,"概述");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addInfoCard(Sniffer* dev){
    ui->info_tab->addInfoCard(dev);
}
