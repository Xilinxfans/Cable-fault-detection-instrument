#include "uart.h"
#include "mainwindow.h"



unsigned char w_buf[1] = {0xFF};
typedef struct uart_hardware_cfg {
    unsigned int baudrate;      /* 波特率 */
    unsigned char dbit;         /* 数据位 */
    char parity;                /* 奇偶校验 */
    unsigned char sbit;         /* 停止位 */
} uart_cfg_t;

static struct termios old_cfg;  //用于保存终端的配置参数
int fd;    //串口终端对应的文件描述符

int uart::uart_init(const char *device)
{
    /* 打开串口终端 */
    fd = ::open(device, O_RDWR | O_NOCTTY);
    if (0 > fd) {
        ::fprintf(stderr, "open error: %s: %s\n", device, strerror(errno));
        ::close(fd);
        return -1;

    }

    /* 获取串口当前的配置参数 */
    if (0 > ::tcgetattr(fd, &old_cfg)) {
        ::fprintf(stderr, "tcgetattr error: %s\n", strerror(errno));
        ::close(fd);
        return -1;
    }

    return 0;
}
int uart::uart_cfg(const uart_cfg_t *cfg)
{
    struct termios new_cfg = {0};   //将new_cfg对象清零
    speed_t speed;

    /* 设置为原始模式 */
    ::cfmakeraw(&new_cfg);

    /* 使能接收 */
    new_cfg.c_cflag |= CREAD;

    /* 设置波特率 */
    switch (cfg->baudrate) {
    case 1200: speed = B1200;
        break;
    case 1800: speed = B1800;
        break;
    case 2400: speed = B2400;
        break;
    case 4800: speed = B4800;
        break;
    case 9600: speed = B9600;
        break;
    case 19200: speed = B19200;
        break;
    case 38400: speed = B38400;
        break;
    case 57600: speed = B57600;
        break;
    case 115200: speed = B115200;
        qDebug("default baud rate: 115200\n");
        break;
    case 230400: speed = B230400;
        break;
    case 460800: speed = B460800;
        break;
    case 500000: speed = B500000;
        break;
    default:    //默认配置为115200
        speed = B115200;
        qDebug("default baud rate: 115200\n");
        break;
    }

    if (0 > cfsetspeed(&new_cfg, speed)) {
        fprintf(stderr, "cfsetspeed error: %s\n", strerror(errno));
        return -1;
    }

    /* 设置数据位大小 */
    new_cfg.c_cflag &= ~CSIZE;  //将数据位相关的比特位清零
    switch (cfg->dbit) {
    case 5:
        new_cfg.c_cflag |= CS5;
        break;
    case 6:
        new_cfg.c_cflag |= CS6;
        break;
    case 7:
        new_cfg.c_cflag |= CS7;
        break;
    case 8:
        new_cfg.c_cflag |= CS8;
        qDebug("default data bit size: 8\n");
        break;
    default:    //默认数据位大小为8
        new_cfg.c_cflag |= CS8;
        qDebug("default data bit size: 8\n");
        break;
    }

    /* 设置奇偶校验 */
    switch (cfg->parity) {
    case 'N':       //无校验
        new_cfg.c_cflag &= ~PARENB;
        new_cfg.c_iflag &= ~INPCK;
        qDebug("default parity: N\n");
        break;
    case 'O':       //奇校验
        new_cfg.c_cflag |= (PARODD | PARENB);
        new_cfg.c_iflag |= INPCK;
        break;
    case 'E':       //偶校验
        new_cfg.c_cflag |= PARENB;
        new_cfg.c_cflag &= ~PARODD; /* 清除PARODD标志，配置为偶校验 */
        new_cfg.c_iflag |= INPCK;
        break;
    default:    //默认配置为无校验
        new_cfg.c_cflag &= ~PARENB;
        new_cfg.c_iflag &= ~INPCK;
        qDebug("default parity: N\n");
        break;
    }

    /* 设置停止位 */
    switch (cfg->sbit) {
    case 1:     //1个停止位
        new_cfg.c_cflag &= ~CSTOPB;
        qDebug("default stop bit size: 1\n");
        break;
    case 2:     //2个停止位
        new_cfg.c_cflag |= CSTOPB;
        break;
    default:    //默认配置为1个停止位
        new_cfg.c_cflag &= ~CSTOPB;
        qDebug("default stop bit size: 1\n");
        break;
    }

    /* 将MIN和TIME设置为0 */
    new_cfg.c_cc[VTIME] = 0;
    new_cfg.c_cc[VMIN] = 0;

    /* 清空缓冲区 */
    if (0 > ::tcflush(fd, TCIOFLUSH)) {
        fprintf(stderr, "tcflush error: %s\n", strerror(errno));
        return -1;
    }

    /* 写入配置、使配置生效 */
    if (0 > ::tcsetattr(fd, TCSANOW, &new_cfg)) {
        fprintf(stderr, "tcsetattr error: %s\n", strerror(errno));
        return -1;
    }

    /* 配置OK 退出 */
    return 0;
}



uart::uart(QObject *parent) : QObject(parent)
{
    uart_cfg_t cfg = {0};
    char *device = NULL;
    int rw_flag = -1;
    //        unsigned char w_buf[10] = {0x11, 0x22, 0x33, 0x44,
    //                0x55, 0x66, 0x77, 0x88};
    device = "/dev/ttyS1";
    rw_flag = 1;
    cfg.baudrate=115200;
    cfg.dbit =8;
    cfg.parity='N';
    cfg.sbit =1;
    uart::uart_init(device);


    /* 串口配置 */
    uart::uart_cfg(&cfg);
}



void uart::write(){
    unsigned char  w_buff[1]={0x55};
    unsigned char  w_buff1[1]={0xef};
    int a;
    if (aaa<2){


    //向串口写入数据
    a= ::write(fd, w_buff, 1);
    QThread::sleep(1);
    a=::write(fd, w_buf, 1);
     }
    else{
        a= ::write(fd, w_buff, 1);
        QThread::sleep(1);
        a=::write(fd, w_buff1, 1);

    }

    if(a <0){

        ::tcsetattr(fd, TCSANOW, &old_cfg);   //恢复到之前的配置
        ::close(fd);
        ::exit(EXIT_SUCCESS);

    }



}


void uart::CLOSE(){
    ::tcsetattr(fd, TCSANOW, &old_cfg);   //恢复到之前的配置
    ::close(fd);
    ::exit(EXIT_SUCCESS);
}
void uart::Read(){





    //   rs::async_io_init();	//我们使用异步I/O方式读取串口的数据，调用该函数去初始化串口的异步I/O
    //    for ( ; ; ){
    //    ::sleep(1);
    //    }
    //    ::tcsetattr(fd, TCSANOW, &old_cfg);   //恢复到之前的配置
    //    ::close(fd);
    //    ::exit(EXIT_SUCCESS);

}
