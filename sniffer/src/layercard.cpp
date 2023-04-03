#include "layercard.h"
#include "ui_layercard.h"
#include "dbloader.hpp"

LayerCard::LayerCard(DBLoader::layerInfo_t layer, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::LayerCard)
{
    ui->setupUi(this);
    ui->layerNem->setText(QString::number(layer.layerNum));
    ui->layerType->setText(QString::fromStdString(layer.layerType));
    ui->type->setText(QString::fromStdString(layer.type));
    ui->src->setText(QString::fromStdString(layer.src));
    ui->dst->setText(QString::fromStdString(layer.dst));
    ui->len->setText(QString::number(layer.len));


    QByteArray data((char*)layer.data,layer.len);
    QString str = QString::fromLatin1(data);
    str = str.split("").join(" ");
    ui->row->setText(data.toHex(' ').toUpper());
    ui->decode->setText(str);

    delete[] layer.data;
}

LayerCard::~LayerCard()
{
    delete ui;
}
