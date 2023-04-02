#ifndef LAYERFRAME_H
#define LAYERFRAME_H

#include <QFrame>

namespace Ui {
class LayerFrame;
}

class LayerFrame : public QFrame
{
    Q_OBJECT

public:
    explicit LayerFrame(QWidget *parent = nullptr);
    ~LayerFrame();

private:
    Ui::LayerFrame *ui;
};

#endif // LAYERFRAME_H
