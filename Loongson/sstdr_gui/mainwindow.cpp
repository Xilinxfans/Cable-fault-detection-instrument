#include "mainwindow.h"

#include<QtCharts/QChartView>
#include<QtCharts/QLineSeries>

 QT_CHARTS_USE_NAMESPACE

#include "ui_mainwindow.h"
//#include <QtMath>

int aaa=0;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    UART =new uart(this);
    SSTDR =new sstdr(this);
    QLineSeries *series = new QLineSeries();
    connect(SSTDR,SIGNAL(sigDone(float,int)),this,SLOT(setjg(float,int)));
    connect(SSTDR,SIGNAL(sigDone1()),this,SLOT(setjg1()));
    ui->JL->setFocusPolicy(Qt::NoFocus);
    ui->LX->setFocusPolicy(Qt::NoFocus);



}

MainWindow::~MainWindow()
{
    delete ui;



}


void MainWindow::closeEvent(QCloseEvent *event){
    UART->CLOSE();
}

void MainWindow::on_start_clicked()
{   aaa=1;
    QThread::sleep(1);
    UART->write();
    SSTDR->start();
}
void MainWindow::setjg(float Ss,int aa){


      QString data= QString("%1 m").arg(Ss);
      ui->JL->setText(data);
    if(aa>2){
      ui->LX->setText("断路");
    }
     else
       {
      ui->LX->setText("短路");
       }


}

void MainWindow::setjg1(){

    UART->write();

}

void MainWindow::on_pushButton_2_clicked()
{
      ui->xs12->setCurrentIndex(1);
      //![1]
          QLineSeries *series = new QLineSeries();
      //![1]

      //![2]
         if(aaa<2){

          for(int i = 0; i < 300; i++)
          {
              series->append(0.033*i, buf[i]);

          }
         }
         else{

             for(int i = 0; i < 50; i++)
             {
                 series->append(0.033*i, buf[i]);

             }

         }
      //![2]

      //![3]
          QChart *chart = new QChart();
          chart->legend()->hide();
          chart->addSeries(series);
          chart->createDefaultAxes();
          chart->setTitle("反射波形图 ");
      //![3]

      //![4]
  //        QChartView *chartView = new QChartView(chart);
  //        chartView->setRenderHint(QPainter::Antialiasing);
          ui->graphicsView->setChart(chart);
          ui->graphicsView->setRenderHint(QPainter::Antialiasing);
      //![4]
          //![1]
              QLineSeries *series1 = new QLineSeries();
          //![1]

          //![2]
            if(aaa<2){
              for(int i = 2800; i < 3200; i++)
              {
                  series1->append(0.033*i, r[i]);

              }
            }

            else{

                for(int i = 2850; i < 3050; i++)
                {
                    series1->append(0.033*i, r[i]);

                }

            }
          //![2]

          //![3]
              QChart *chart1 = new QChart();
              chart1->legend()->hide();
              chart1->addSeries(series1);
              chart1->createDefaultAxes();
              chart1->setTitle("互相关波形图");
          //![3]

          //![4]
      //        QChartView *chartView = new QChartView(chart);
      //        chartView->setRenderHint(QPainter::Antialiasing);
              ui->graphicsView_2->setChart(chart1);
              ui->graphicsView_2->setRenderHint(QPainter::Antialiasing);
          //![4]

}

void MainWindow::on_pushButton_3_clicked()
{
      ui->xs12->setCurrentIndex(0);
}

void MainWindow::on_pushButton_clicked()
{

    UART->write();
    SSTDR->start();
}
