#ifndef LOGFRAME_H
#define LOGFRAME_H

#include <QScrollArea>

namespace Ui {
class LogFrame;
}

class LogFrame : public QScrollArea
{
    Q_OBJECT

public:
    explicit LogFrame(QWidget *parent = nullptr);
    ~LogFrame();

private:
    Ui::LogFrame *ui;
};

#endif // LOGFRAME_H
