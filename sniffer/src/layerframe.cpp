#include "layerframe.h"
#include "ui_layerframe.h"

LayerFrame::LayerFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::LayerFrame)
{
    ui->setupUi(this);
}

LayerFrame::~LayerFrame()
{
    delete ui;
}
