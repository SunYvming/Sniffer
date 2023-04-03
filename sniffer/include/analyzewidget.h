#ifndef ANALYZEWIDGET_H
#define ANALYZEWIDGET_H

#include <QWidget>

#include "dbloader.hpp"
#include <vector>

namespace Ui {
class AnalyzeWidget;
}

class AnalyzeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AnalyzeWidget(QWidget *parent = nullptr);
    ~AnalyzeWidget();

    void addDBLoader(DBLoader* db);

private:
    Ui::AnalyzeWidget *ui;
    std::vector<DBLoader*> dbs;
};

#endif // ANALYZEWIDGET_H
