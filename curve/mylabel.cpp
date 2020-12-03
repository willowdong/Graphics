#include "mylabel.h"

MyLabel::MyLabel(QWidget *parent):QLabel(parent)
{

}

void MyLabel::paintEvent(QPaintEvent *){
    QPainter painter(this);
    QPen pen(Qt::red, 3);
    painter.setPen(pen);

    /*绘制矩形*/
    if(drawRect==1){
        painter.drawRect(QRect(rect[0],rect[1],rect[2]-rect[0],rect[3]-rect[1]));
    }

    /*绘制多边形*/
    if(drawPoint==1){
        QPen pen(Qt::blue, 3);
        painter.setPen(pen);
        for(int i=0;i<numP-1;i++){
            painter.drawLine(QPoint(pointx[i],pointy[i]),QPoint(pointx[i+1],pointy[i+1]));
//            qDebug()<<i;
        }
        painter.drawLine(QPoint(pointx[numP-1],pointy[numP-1]),QPoint(pointx[0],pointy[0]));
    }

    /*绘制裁剪结果*/
    if(drawCurve==1){
        QPen pen(Qt::yellow, 3);
        painter.setPen(pen);
        for(int i=0;i<nums;i++){
            int j=i+1;
            if(i==nums-1){
                j=0;
            }
//            painter.drawLine(QPoint(pointx_curve[3][i],pointy_curve[3][i]),QPoint(pointx_curve[3][j],pointy_curve[3][j]));
            int n=3;
            painter.drawLine(QPoint(pointx_curve[n][i],pointy_curve[n][i]),QPoint(pointx_curve[n][j],pointy_curve[n][j]));
        }
    }
}

void MyLabel::mousePressEvent(QMouseEvent *ev){
    //记录矩形坐标
    if(drawRect==1&&getPoint==0){
        rect[0]=ev->x();
        rect[1]=ev->y();
    }
    //记录多边形顶点
    if(getPoint==1){
        pointx[numP]=ev->x();
        pointy[numP]=ev->y();
        numP=numP+1;
    }
}

void MyLabel::mouseReleaseEvent(QMouseEvent *ev){
    //矩形终点
    if(drawRect==1&&getPoint==0){
        rect[2]=ev->x();
        rect[3]=ev->y();
    }

    repaint();
}

/*求交点的函数*/
QPointF MyLabel::intersect(int x1, int y1, int x2, int y2, int sig, int base){
    double x=0,y=0;
    //sig=0,求左右边界 sig=1，求上下边界
    double k=(y2-y1)*1.0/(x2-x1)*1.0;
    double b=y1*1.0-k*x1;
    if(sig==0){
      x=base;
      y=k*base+b;
      if(y2==y1){           //水平直线
          y=y1;
      }
    }
    if(sig==1){
        y=base;
        x=(y-b)*1.0/k;
        if(x2==x1){         //竖直直线，斜率∞
            x=x1;
        }
    }
    return QPointF(x,y);
}

/*裁剪函数*/
void MyLabel::curve(){
    qDebug()<<"curve";
    QPointF temp;

    //左边界
    int i,j,index;
    for(i=0;i<numP;i++){
        j=i+1;
        if(i==numP-1){
            j=0;
        }
        if(pointx[i]<rect[0]&&pointx[j]<rect[0]){               //out，不记录点
            continue;
        }
        else if(pointx[i]>=rect[0]&&pointx[j]>=rect[0]){        //in，记录终点
            pointx_curve[0][nums]=pointx[j];
            pointy_curve[0][nums]=pointy[j];
            nums++;
        }
        else if(pointx[i]<rect[0]&&pointx[j]>=rect[0]){         //out->in，记录交点和终点
            temp=intersect(pointx[i],pointy[i],pointx[j],pointy[j],0,rect[0]);
            pointx_curve[0][nums]=temp.rx();
            pointy_curve[0][nums]=temp.ry();
            nums++;
            pointx_curve[0][nums]=pointx[j];
            pointy_curve[0][nums]=pointy[j];
            nums++;
        }
        else if (pointx[i]>=rect[0]&&pointx[j]<rect[0]) {       //in->out，记录交点
            temp=intersect(pointx[i],pointy[i],pointx[j],pointy[j],0,rect[0]);
            pointx_curve[0][nums]=temp.rx();
            pointy_curve[0][nums]=temp.ry();
            nums++;
        }
    }
    qDebug()<<"curve1 nums: "<<nums;

    //右边界
    index=1;
    _nums=nums;                             //nums为当前边产生的顶点数，_nums为上一条边产生的顶点数
    //如果上一条边的裁剪没有产生顶点，则把上上条边结果复制过去
    if(nums==0){
        for(int i=0;i<numP;i++){
            pointx_curve[0][i]=pointx[i];
            pointy_curve[0][i]=pointy[i];
        }
        _nums=numP;
    }
    nums=0;
    for(i=0;i<_nums;i++){
        j=i+1;
        if(i==_nums-1){
            j=0;
        }
        if(pointx_curve[index-1][i]<=rect[2]&&pointx_curve[index-1][j]<=rect[2]){   //in
           pointx_curve[index][nums]=pointx_curve[index-1][j];
           pointy_curve[index][nums]=pointy_curve[index-1][j];
           nums++;
           qDebug()<<"in";
        }
        else if(pointx_curve[index-1][i]>rect[2]&&pointx_curve[index-1][j]>rect[2]){        //out
            qDebug()<<"out";
            continue;
        }
        else if(pointx_curve[index-1][i]>rect[2]&&pointx_curve[index-1][j]<=rect[2]){  //out->in
            temp=intersect(pointx_curve[index-1][i],pointy_curve[index-1][i],
                    pointx_curve[index-1][j],pointy_curve[index-1][j],0,rect[2]);
            pointx_curve[index][nums]=temp.rx();
            pointy_curve[index][nums]=temp.ry();
            nums++;
            pointx_curve[index][nums]=pointx_curve[index-1][j];
            pointy_curve[index][nums]=pointy_curve[index-1][j];
            nums++;
            qDebug()<<"out->in";
        }
        else if(pointx_curve[index-1][i]<=rect[2]&&pointx_curve[index-1][j]>rect[2]){ //in->out
            temp=intersect(pointx_curve[index-1][i],pointy_curve[index-1][i],
                    pointx_curve[index-1][j],pointy_curve[index-1][j],0,rect[2]);
            pointx_curve[index][nums]=temp.rx();
            pointy_curve[index][nums]=temp.ry();
            nums++;
            qDebug()<<"in->out";
        }
    }
    qDebug()<<"curve2 nums: "<<nums;

    /*除判断条件外与前两个计算过程相同*/

    //下边界
    index=2;
    if(nums==0){
        for(int i=0;i<_nums;i++){
            pointx_curve[index-1][i]=pointx_curve[index-2][i];
            pointy_curve[index-1][i]=pointy_curve[index-2][i];
        }
    }
    if(nums!=0){
      _nums=nums;
    }
    nums=0;
    for(i=0;i<_nums;i++){
        j=i+1;
        if(i==_nums-1){
            j=0;
        }
        if(pointy_curve[index-1][i]>rect[3]&&pointy_curve[index-1][j]>rect[3]){     //out  (0,0)左上角
            qDebug()<<"out";
            continue;
        }
        else if (pointy_curve[index-1][i]<=rect[3]&&pointy_curve[index-1][j]<=rect[3]) {  //in
            pointx_curve[index][nums]=pointx_curve[index-1][j];
            pointy_curve[index][nums]=pointy_curve[index-1][j];
            nums++;
            qDebug()<<"in";
        }
        else if (pointy_curve[index-1][i]>rect[3]&&pointy_curve[index-1][j]<=rect[3]) {   //out->in
            temp=intersect(pointx_curve[index-1][i],pointy_curve[index-1][i],
                                                                           pointx_curve[index-1][j],pointy_curve[index-1][j],1,rect[3]);
            pointx_curve[index][nums]=temp.rx();
            pointy_curve[index][nums]=temp.ry();
            nums++;
            pointx_curve[index][nums]=pointx_curve[index-1][j];
            pointy_curve[index][nums]=pointy_curve[index-1][j];
            nums++;
            qDebug()<<"out->in";
        }
        else if (pointy_curve[index-1][i]<=rect[3]&&pointy_curve[index-1][j]>rect[3]) {      //in->out
            temp=intersect(pointx_curve[index-1][i],pointy_curve[index-1][i],
                                                                           pointx_curve[index-1][j],pointy_curve[index-1][j],1,rect[3]);
            pointx_curve[index][nums]=temp.rx();
            pointy_curve[index][nums]=temp.ry();
            nums++;
            qDebug()<<"in->out";
        }
    }
    qDebug()<<"curve3 nums: "<<nums;

    //上边界
    index=3;
    if(nums==0){
        for(int i=0;i<_nums;i++){
            pointx_curve[index-1][i]=pointx_curve[index-2][i];
            pointy_curve[index-1][i]=pointy_curve[index-2][i];
        }
    }
    if(nums!=0){
      _nums=nums;
    }
    nums=0;
    for(i=0;i<_nums;i++){
        j=i+1;
        if(i==_nums-1){
            j=0;
        }
        if(pointy_curve[index-1][i]<rect[1]&&pointy_curve[index-1][j]<rect[1]){  //out
            qDebug()<<"out";
            continue;
        }
        else if (pointy_curve[index-1][i]>=rect[1]&&pointy_curve[index-1][j]>=rect[1]) {      //in
            pointx_curve[index][nums]=pointx_curve[index-1][j];
            pointy_curve[index][nums]=pointy_curve[index-1][j];
            nums++;
            qDebug()<<"in";
        }
        else if (pointy_curve[index-1][i]>=rect[1]&&pointy_curve[index-1][j]<rect[1]) {  //in->out
            temp=intersect(pointx_curve[index-1][i],pointy_curve[index-1][i],
                                                                          pointx_curve[index-1][j],pointy_curve[index-1][j],1,rect[1]);
            pointx_curve[index][nums]=temp.rx();
            pointy_curve[index][nums]=temp.ry();
            nums++;
            qDebug()<<"in->out";
        }
        else if (pointy_curve[index-1][i]<rect[1]&&pointy_curve[index-1][j]>=rect[1]) {  //out->in
            temp=intersect(pointx_curve[index-1][i],pointy_curve[index-1][i],
                                                                          pointx_curve[index-1][j],pointy_curve[index-1][j],1,rect[1]);
            pointx_curve[index][nums]=temp.rx();
            pointy_curve[index][nums]=temp.ry();
            nums++;
            pointx_curve[index][nums]=pointx_curve[index-1][j];
            pointy_curve[index][nums]=pointy_curve[index-1][j];
            nums++;
            qDebug()<<"out->in";
        }
    }
    /*如果最后一次裁剪没有结果，则把下面边的结果复制过来*/
    if(nums==0){
        nums=_nums;
        for(int i=0;i<_nums;i++){
            pointx_curve[index][i]=pointx_curve[index-1][i];
            pointy_curve[index][i]=pointy_curve[index-1][i];
        }
    }
    qDebug()<<"curve4 nums: "<<nums;
}

/*重绘函数，清除所有结果*/
void MyLabel::fresh(){
    drawRect=0;
    getPoint=0;
    drawCurve=0;
    drawPoint=0;
    memset(rect,0,4);
    memset(pointx,0,1024);
    memset(pointy,0,1024);
    memset(pointx_curve,0,10*100);
    memset(pointy_curve,0,10*100);
    nums=numP=_nums=0;
    repaint();
}
