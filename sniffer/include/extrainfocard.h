#ifndef EXTRAINFOCARD_H
#define EXTRAINFOCARD_H

#include <QFrame>
#include <string>

#include "dbloader.hpp"

namespace Ui {
class TcpInfo;
class HttpInfo;
class DefaultInfo;
}

class ExtraInfoCard : public QFrame
{
    Q_OBJECT

public:
    ExtraInfoCard(std::string type,QWidget *parent = nullptr);
    ExtraInfoCard(DBLoader::tcpInfo_t info,QWidget *parent = nullptr);

    ~ExtraInfoCard();

private:
    void *ui;
    std::string type;
};

#endif // EXTRAINFOCARD_H
