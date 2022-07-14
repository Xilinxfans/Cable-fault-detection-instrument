#ifndef SSTDR_H
#define SSTDR_H

#include <QThread>

#include "uart.h"

extern "C"{
         #include <stdio.h>
         #include <stdlib.h>
         #include <math.h>
}
class sstdr : public QThread
{
    Q_OBJECT
public:
    explicit sstdr(QObject *parent = nullptr);

    void xcorr(float *r, float *x, float *y, int N);
    float FindMAX(int m1,int m2,int f);
    int SStdr();

protected:
    void run();

signals:
    void sigDone(float S,int a);
    void sigDone1();


public slots:
};

extern float r[6000];

extern float S;
extern int A;
extern unsigned char buf[3000];
#endif // SSTDR_H
