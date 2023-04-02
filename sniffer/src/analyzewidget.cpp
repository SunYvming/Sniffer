#include "analyzewidget.h"
#include "ui_analyzewidget.h"

AnalyzeWidget::AnalyzeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AnalyzeWidget)
{
    ui->setupUi(this);
}

AnalyzeWidget::~AnalyzeWidget()
{
    delete ui;
}
