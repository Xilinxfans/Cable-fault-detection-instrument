#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QThread>
#include <QMainWindow>
#include <sstream>
#include <string>
//#include <QtCharts>
//#include <QValueAxis>
#include "uart.h"
#include "sstdr.h"

 QT_BEGIN_NAMESPACE

namespace Ui {
class MainWindow;
}
 QT_END_NAMESPACE
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    uart *UART;
    sstdr *SSTDR;

private slots:

    void closeEvent(QCloseEvent *event);

    void on_start_clicked();

    void setjg(float Ss,int aa);
    void setjg1();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;


};
extern int aaa;
#endif // MAINWINDOW_H
