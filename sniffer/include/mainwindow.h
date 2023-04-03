#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "sniffer.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void addInfoCard(Sniffer* dev);

    void addDBLoader(Sniffer* dev);
    

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
