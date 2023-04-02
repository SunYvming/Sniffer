#include "logframe.h"
#include "ui_logframe.h"

LogFrame::LogFrame(QWidget *parent) :
    QScrollArea(parent),
    ui(new Ui::LogFrame)
{
    ui->setupUi(this);
}

LogFrame::~LogFrame()
{
    delete ui;
}
