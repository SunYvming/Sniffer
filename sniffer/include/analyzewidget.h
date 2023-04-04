#ifndef ANALYZEWIDGET_H
#define ANALYZEWIDGET_H

#include <QWidget>
#include <QComboBox>

#include "dbloader.hpp"
#include "loginfoheaderview.hpp"
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

    void updateTable(std::string target);

private:
    Ui::AnalyzeWidget *ui;
    std::vector<DBLoader*> dbs;
    std::string lastText;
    std::string localIp;
    bool localOnly;
    LogInfoHeaderView * headerView;
};

#endif // ANALYZEWIDGET_H
