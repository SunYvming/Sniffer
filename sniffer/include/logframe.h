#ifndef LOGFRAME_H
#define LOGFRAME_H

#include <QScrollArea>
#include "dbloader.hpp"
#include "layercard.h"
#include "extrainfocard.h"
#include <vector>

namespace Ui {
class LogFrame;
}

class LogFrame : public QScrollArea
{
    Q_OBJECT

public:
    explicit LogFrame(QWidget *parent = nullptr);
    ~LogFrame();
    void updateLayer(uint64_t sec, uint64_t nsec,DBLoader* db);

private:
    Ui::LogFrame *ui;
    std::vector<LayerCard*> cards;
    std::vector<ExtraInfoCard*> extraCards;
    QWidget *widget;
};

#endif // LOGFRAME_H
