#include "sstdr.h"
#include "mainwindow.h"

#define NN 3000
sstdr::sstdr(QObject *parent) : QThread(parent)
{

}



float r[6000]={0};


float S;
int A=1;
unsigned char buf[3000] = {0};
void sstdr::run(){

    int ret=0;

//    sstdr::SStdr();





//    if((fd_txt=::fopen("/home/loongson/rsin.txt","wt"))  == NULL){
//        ::printf("Cannot open \n");
//        ::fclose(fd_txt);
//        ::exit(0);

//    }



//    if((fd_txt=::fopen("/home/loongson/rsin.txt","wt"))  == NULL){
//        ::printf("Cannot open \n");
//        ::fclose(fd_txt);
//        ::exit(0);

//    }




    while(1){
          QThread::usleep(500000);
        ret = ::read(fd, buf, 3000);     //一次最多读3000个字节数据


         if(ret==518){

            sstdr::SStdr();
             }

        for (int n = 0; n < ret; n++){

         ::printf("0x%hhx ", buf[n]);

             }
    }
}





void sstdr::xcorr(float *r, float *x, float *y, int N)
{
    float sxy;
    int    delay,i,j;

    for(int delay = -N + 1; delay < N; delay++)
    {

        sxy = 0;
        for(i=0; i<N; i++)
        {
            j = i + delay;
            if((j < 0) || (j >= N))
                continue;
            else
                sxy += (x[i] * y[j]);
        }


        r[delay + N - 1] = sxy;
    }
}
/***************测距函数***************************
*****          m1    原始信号长度；            *****
*****          m2    互相关函数长度；         ******
*****          f     采样频率；                ****/
float sstdr::FindMAX(int m1,int m2,int f)
{   int M1=0;
    int M2=0;
    float r1[6000]={0};
    for (int i1=0;i1<6000;i1++){
        r1[i1]=::abs(r[i1]);
    }
    int Max=r1[0];
    for (int MM=0;MM<m2;MM++)
    {
        if (r1[MM]>Max){
            Max=r1[MM];
            M1=MM;
        }
    }
  for (int jj=M1-m1;jj<M1+m1;jj++)
  {   r1[jj]=0;
  }
    int MM1=M1+m1;
    int Max1=r1[0];
    for (int ii=0;ii<m2;ii++){

        if (r1[ii]>Max1){
            Max1=r1[ii];
            M2=ii;
          }
    }
    float t=0;
    float s=0;
            t=::abs((M2-M1))*(((double)1/f)*1000000);
            s=95*t;
            if (r[M2]<0){
                A=1;
            }
            else{
            A=3;
                }
    return s;

}


int sstdr::SStdr(){
    float x[3000];
    float y[3000];
    float x1[3000];
    float y1[3000];
    int aa[3000]={0};
    FILE  *fp_out;
    int delay;


    for(int a=0;a<3000;a++){
        aa[a]=(int)buf[a];
        x[a]=(float)aa[a];


    }

    float xxx[518]={0};
    for(int i=0;i<518;i++){
        xxx[i]=x[i];

    }
    int MAXX=0;
    for(int i1=130;i1<518;i1++){
        if(::abs(xxx[i1]-128)>5){
           MAXX=i1;
        }

    }
 if(aaa<2){
    if (MAXX==0){
        aaa=3;
        emit  sigDone1();

        return 1;
    }

    else{
        aaa=1;
    }
          }
//   FILE *fp1;
//    if (!(fp1 = ::fopen("/home/loongson/rsin.txt","r+")))			//fp_a = fopen("D:/DulMZData/4a.dat","r+");
//    {
//        ::printf("Cannot open file. ");
//    }

//    int i = 0;
//    float temp1 = 0;
//    while ((::fscanf(fp1, "%f", &temp1)) != EOF) //以浮点型格式写入temp
//   ::fclose(fp1);
//    {
  //      x= buf; //再将temp赋给要存放的数组
//        i++;
//    }
//   ::fclose(fp1);
    //-------------------------------------------------------------------
    if(aaa<2){
    FILE *fp2;

    if (!(fp2 = ::fopen("/home/loongson/S2.txt","r+")))
    {
        ::printf("Cannot open file. ");
    }

    int j = 0;
    float temp2 = 0;
    while ((::fscanf(fp2, "%f", &temp2)) != EOF) //以浮点型格式写入temp
    {
        y[j] = temp2; //再将temp赋给要存放的数组
        j++;
    }
   ::fclose(fp2);
         }

    else{
        FILE *fp2;

        if (!(fp2 = ::fopen("/home/loongson/S3.txt","r+")))
        {
            ::printf("Cannot open file. ");
        }

        int j = 0;
        float temp2 = 0;
        while ((::fscanf(fp2, "%f", &temp2)) != EOF) //以浮点型格式写入temp
        {
            y[j] = temp2; //再将temp赋给要存放的数组
            j++;
        }
       ::fclose(fp2);


    }
    for (int i=0;i<320;i++){
        x1[i]=x[i]-128;
        y1[i]=y[i]-128;

    }
     sstdr::xcorr(r, x1, y1, NN);

    //Open the file to write
    if((fp_out=::fopen("/home/loongson/out_xcorr.txt","wt")) == NULL)
    {
        ::printf("Cannot open this file!\n");
        ::exit(0);
    }

    for(delay=0; delay < 2*NN; delay++)
    {
        ::fprintf(fp_out,"%f\n",r[delay]);
    }
    ::fclose(fp_out);
    int a1;
if (aaa<2){
a1=60;

}
else{
a1=10;
}
       //
    int a2=2*NN;     //
    int f=30000000;   //
    S=sstdr::FindMAX(a1,a2,f);
    ::printf("\n***************%f******************\n", S);



    emit  sigDone(S,A);
    return 0;



}
