#ifndef INFOCARD_H
#define INFOCARD_H

#include <QFrame>
#include "sniffer.hpp"

namespace Ui {
class InfoCard;
}

class InfoCard : public QFrame
{
    Q_OBJECT

public:
    explicit InfoCard(QWidget *parent = nullptr, Sniffer* sniffer = nullptr);
    ~InfoCard();
    void setStartInfo();
    void setStopInfo();

private:
    Ui::InfoCard *ui;
    Sniffer* sniffer;
};

#endif // INFOCARD_H
