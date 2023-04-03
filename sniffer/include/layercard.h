#ifndef LAYERCARD_H
#define LAYERCARD_H

#include <QFrame>
#include <dbloader.hpp>

namespace Ui {
class LayerCard;
}

class LayerCard : public QFrame
{
    Q_OBJECT

public:
    explicit LayerCard(DBLoader::layerInfo_t layer, QWidget *parent = nullptr);
    ~LayerCard();
    void updateLayer(int row,int col,DBLoader* db);

private:
    Ui::LayerCard *ui;
};

#endif // LAYERFRAME_H
