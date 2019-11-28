#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVector>
#include <QTime>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    pidcontroller(new torquecontroller),
    filterForAnalog(new filter()),
    eposcontroller(new EposController),
    galilnode(new GalilNode())
{
    ui->setupUi(this);

    eposcontroller->initMotor();
    galilnode->initMotor();

    //3.interface timer initialize
    id1 = startTimer(100);                  // 开启一个1秒定时器，返回其ID

    for(int i=0;i<20;i++)
    {
        num[i] = 0;
        num2[i] = 0;
    }
    for(int i=0;i<count_num;i++)
    {
        analogInput[i] = 0;
    }
    n=0;
    QTimer *timer = new QTimer(this);
    timer->start(100);
    connect(timer,SIGNAL(timeout()),this,SLOT(Graph_Show()));
    connect(timer,SIGNAL(timeout()),this,SLOT(Graph_Show2()));

    biascurrent = -0.025;
    modeChange = 0;  //mode control bit
    motortargetcurrent = 0;
    setmotorcurrent = 0;
    ui->lineEdit_4->setText(QString::number(0));
    ui->lineEdit_5->setText(QString::number(0));
    ui->lineEdit_6->setText(QString::number(0));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    static QString strCPOSA, strCPOSB, strCPOSC;

    eposcontroller->getMotorPosition(1, motorpositon);
    strCPOSA.sprintf("%d",motorpositon);
    ui->lineEdit->setText(strCPOSA);

    galilnode->getAnalogInput(1);
    strCPOSB.sprintf("%f",galilnode->analogValue);
    ui->lineEdit_2->setText(strCPOSB);

    eposcontroller->getMotorCurrent(1, &motorcurrent);
    strCPOSC.sprintf("%d",motorcurrent);
    ui->lineEdit_3->setText(strCPOSC);

    if (modeChange == 3)
    {
        setmotorcurrent = pidcontroller->pidcontrol(num[19]);
        cout << "current torque(mv):" << num[19] << endl;
        cout << "set motor speed(counts/s):" << setmotorcurrent << endl;
        if (setmotorcurrent < -500)
            setmotorcurrent = -500;
        if (setmotorcurrent > 500)
            setmotorcurrent = 500;
        galilnode->setMotorJogSpeed(1, setmotorcurrent);
    }
    else if(modeChange == 4)
    {
        setmotorcurrent = pidcontroller->impedancecontrol(num[19]);
        cout << "current torque(mv):" << num[19] << endl;
        cout << "set motor speed(counts/s):" << setmotorcurrent << endl;
        if (setmotorcurrent < -500)
            setmotorcurrent = -500;
        if (setmotorcurrent > 500)
            setmotorcurrent = 500;
        eposcontroller->setMotorCurrent(1, setmotorcurrent);
    }
    else {
        pidcontroller->Bias = 0;
        pidcontroller->Integral_bias = 0;
        pidcontroller->Last_Bias = 0;
        pidcontroller->Pwm = 0;
    }
}

void MainWindow::on_pushButton_clicked()
{
//    galilnode->movePositionAbsolutely(1,1000);
    modeChange = 1;
}

void MainWindow::on_pushButton_2_clicked()
{
    eposcontroller->setenablestate(1);
    galilnode->setenablestate(1);
}

void MainWindow::on_pushButton_3_clicked()
{
    galilnode->disableMotor(1);
    eposcontroller->setdisablestate(1);
    modeChange = 0;
    motortargetcurrent = 0;
    setmotorcurrent = 0;
}

void MainWindow::on_pushButton_4_clicked()
{
//    eposcontroller->analogConfig(1);
}

void MainWindow::Graph_Show()
{
    QTime t;
    t=QTime::currentTime();
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    n=qrand()%50;
    Graph_Show(ui->widget);
}

void MainWindow::Graph_Show(QCustomPlot *CustomPlot)
{

    QVector<double> temp(20);
    QVector<double> temp1(20);  //analog input

    for(int i=0; i<19; i++)
    {
        num[i]=num[i+1];
    }
//    filterForAnalog->buffer_left_move(analogInput, count_num);
//    analogInput[count_num-1] = galilnode->analogValue;
//    num[19] = filterForAnalog->smooth_aver_filter(analogInput, count_num);
    num[19] = galilnode->analogValue;
//    cout << analogInput[count_num-1] << "," << num[19] << endl;
    for(int i=0;i<20;i++)
    {
        temp[i] = i;
        temp1[i] = num[i];
    }
    CustomPlot->addGraph();
    CustomPlot->graph(0)->setPen(QPen(Qt::red));
    CustomPlot->graph(0)->setData(temp,temp1);

    CustomPlot->xAxis->setLabel("t");
    CustomPlot->yAxis->setLabel("V");

    CustomPlot->xAxis->setRange(0,20);
    CustomPlot->yAxis->setRange(-5,5);
    CustomPlot->replot();
}

void MainWindow::Graph_Show2()
{
    QTime t;
    t=QTime::currentTime();
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    n=qrand()%50;
    Graph_Show2(ui->widget_2);
//    cout << "yes" << endl;
}

void MainWindow::Graph_Show2(QCustomPlot *CustomPlot)
{

    QVector<double> temp(20);
    QVector<double> temp2(20);  //set current for motor
    QVector<double> temp3(20);  //set current for motor

    for(int i=0; i<19; i++)
    {
        num2[i]=num2[i+1];
        num3[i]=num3[i+1];
    }
    num2[19] = setmotorcurrent;
    num3[19] = motorcurrent;
    for(int i=0;i<20;i++)
    {
        temp[i] = i;
        temp2[i] = num2[i];
        temp3[i] = num3[i];
    }
    CustomPlot->addGraph();
    CustomPlot->graph(0)->setPen(QPen(Qt::blue));
    CustomPlot->graph(0)->setData(temp,temp2);
    CustomPlot->addGraph();
    CustomPlot->graph(1)->setPen(QPen(Qt::green));
    CustomPlot->graph(1)->setData(temp,temp3);

    CustomPlot->xAxis->setLabel("t");
    CustomPlot->yAxis->setLabel("mA");

    CustomPlot->xAxis->setRange(0,20);
    CustomPlot->yAxis->setRange(-1000,1000);
    CustomPlot->replot();
}


void MainWindow::on_pushButton_5_clicked()
{
    eposcontroller->currentConfig(1);
    modeChange = 2;
}

void MainWindow::on_pushButton_6_clicked()
{
    if (modeChange == 2)
      eposcontroller->setMotorCurrent(1, setmotorcurrent);
    cout << setmotorcurrent << endl;
}

void MainWindow::on_lineEdit_4_textChanged(const QString &arg1)
{
    setmotorcurrent = ui->lineEdit_4->text().toShort();
    if (setmotorcurrent < -500)
        setmotorcurrent = -500;
    if (setmotorcurrent > 500)
        setmotorcurrent = 500;
}

void MainWindow::on_pushButton_7_clicked()
{
    //not use now
//    modeChange = 3;  //pid control mode
//    motortargetcurrent = ui->lineEdit_5->text().toDouble();
//    pidcontroller->controltargetcurrent = motortargetcurrent;
//    cout << "Set target torque:" << pidcontroller->controltargetcurrent << endl;
}

void MainWindow::on_pushButton_8_clicked()
{
    modeChange = 4;  //impedance control mode
    motortargetcurrent = ui->lineEdit_5->text().toDouble();
    pidcontroller->controltargetcurrent = motortargetcurrent;
    cout << "Set target torque:" << pidcontroller->controltargetcurrent << endl;

    pidcontroller->impedanceFactor = ui->lineEdit_6->text().toDouble();
    cout << "Impedance factor:" << pidcontroller->impedanceFactor << endl;
}
