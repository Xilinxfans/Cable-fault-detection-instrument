#ifndef UART_H
#define UART_H

#include <QObject>
#include <QThread>

extern "C"{
  #define _GNU_SOURCE     //在源文件开头定义_GNU_SOURCE宏
  #include <stdio.h>
  #include <stdlib.h>
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <fcntl.h>
  #include <unistd.h>
  #include <sys/ioctl.h>
  #include <errno.h>
  #include <string.h>
  #include <signal.h>
  #include <termios.h>
}

class uart : public QObject
{
    Q_OBJECT
public:
    explicit uart(QObject *parent = nullptr);

    typedef struct uart_hardware_cfg {
        unsigned int baudrate;      /* 波特率 */
        unsigned char dbit;         /* 数据位 */
        char parity;                /* 奇偶校验 */
        unsigned char sbit;         /* 停止位 */
    } uart_cfg_t;


    static int uart_init(const char *device);
    static int uart_cfg(const uart_cfg_t *cfg);


    void write();
    void Read();
    void CLOSE();

signals:

public slots:
};
extern int fd;


#endif // UART_H
