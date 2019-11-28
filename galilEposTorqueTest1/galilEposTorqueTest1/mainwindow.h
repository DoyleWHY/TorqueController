#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "galilnode.h"
#include "eposcontroller.h"
#include "qcustomplot.h"
#include "torquecontroller.h"
#include "filter.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    double num[20];
    double num2[20];
    double num3[20];
    int n;
    void Graph_Show(QCustomPlot *customPlot);
    void Graph_Show2(QCustomPlot *customPlot);

private slots:
  void on_pushButton_clicked();

  void on_pushButton_2_clicked();

  void on_pushButton_3_clicked();

  void on_pushButton_4_clicked();

  void on_pushButton_5_clicked();

  void on_pushButton_6_clicked();

  void on_lineEdit_4_textChanged(const QString &arg1);

  void on_pushButton_7_clicked();

  void on_pushButton_8_clicked();

public slots:
    void Graph_Show();
    void Graph_Show2();

private:
    Ui::MainWindow *ui;
    GalilNode *galilnode;
    EposController *eposcontroller;
    torquecontroller *pidcontroller;
    filter *filterForAnalog;

    int id1;   //timer

    int count_num = 5;
    int motorpositon;
    short int motorcurrent;
    short int setmotorcurrent;
    double motortargetcurrent;
    double analogInput[5];
    double biascurrent;
    unsigned int modeChange;


protected:
    void timerEvent(QTimerEvent *event);
};

#endif // MAINWINDOW_H
