#ifndef INFOWIDGET_H
#define INFOWIDGET_H

#include <QScrollArea>
#include <QWidget>

#include "infocard.h"

namespace Ui {
class InfoWidget;
}

class InfoWidget : public QScrollArea
{
    Q_OBJECT

public:
    explicit InfoWidget(QWidget *parent = nullptr);
    ~InfoWidget();

    void addInfoCard(Sniffer* dev);

private:
    Ui::InfoWidget *ui;
    QWidget *widget;
};

#endif // INFOWIDGET_H
