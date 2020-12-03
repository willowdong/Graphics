#include "mylabel.h"

MyLabel::MyLabel(QWidget *parent):QLabel(parent)
{

}

void MyLabel::paintEvent(QPaintEvent *)
{
        QPainter painter(this);

        /*绘制网格 10*10像素 */
        for(int i=0;i<this->height();i+=10){
            painter.drawLine(QPoint(i,0),QPoint(i,this->width()));
        }
        for(int i=0;i<this->width();i+=10){
            painter.drawLine(QPoint(0,i),QPoint(this->height(),i));
        }

        QPen pen(Qt::red, 1);
        painter.setPen(pen);

        /*DDA算法*/
        if(DDA==1){
            double m=(end.ry()-start.ry())*1.0/(end.rx()-start.rx())*1.0;      //计算斜率
            qDebug()<<end.ry()<<" "<<start.ry()<<" "<<end.rx()<<" "<<start.rx();
            /*记录起始点坐标*/
            int x=start.rx();
            int y=start.ry();
            /*取整运算*/
            x=x/10*10;
            y=y/10*10;
            qDebug()<<m;
            /*斜率在0-1之间的情况*/
            if(m>0&&m<1){
                int y0=y;
                while(x<end.rx()){         //在起始终止点之间循环
                    painter.fillRect(QRect(x,y,10,10),QBrush(QColor(255,0,0)));   //填充方格，表示绘制这个像素点
                    y0=y0+10*m;            //计算下一个y_(k+1)的坐标
                    y=(int)(y0+5);
                    /*实现原算法中的取整运算*/
                    if(y%10>5){
                        y=y/10*10+10;
                    }
                    else{
                      y=y/10*10;
                    }
                    x+=10;               //计算x_(k+1)
                }
            }
            /*斜率大于1之间的情况*/
            else if(m>1){
                int x0=x;
                m=1/m;          //斜率取倒数，变为0-1  上面的xy位置对调 运算过程相同
                while(y<end.ry()){  //在起始终止点之间循环
                    qDebug()<<"::"<<m;
                    painter.fillRect(QRect(x,y,10,10),QBrush(QColor(255,0,0)));
                    x0=x0+10*m;
                    x=(int)(x0+5);
                    if(x%10>5){
                        x=x/10*10+10;
                    }
                    else{
                        x=x/10*10;
                    }
                    y+=10;
                }
            }
            /*斜率在-1 - 0*/
            else if(m>-1){
                int y0=y;
                while(x<end.rx()){         //在起始终止点之间循环
                    painter.fillRect(QRect(x,y,10,10),QBrush(QColor(255,0,0)));   //填充方格，表示绘制这个像素点
                    y0=y0+10*m;            //计算下一个y_(k+1)的坐标
                    y=(int)(y0-5);
                    /*实现原算法中的取整运算，取整方式发生了改变，变为向下取整，向下判断*/
                    if(y%10<5){
                        y=y/10*10-10;
                    }
                    else{
                      y=y/10*10;
                    }
                    x+=10;               //计算x_(k+1)
                }
            }
            /*斜率小于-1*/
            else if(m<-1){
                qDebug()<<"!!";
                int x0=x;
                m=1/m;          //斜率取倒数，变为0-1  上面的xy位置对调 运算过程相同
                while(y>end.ry()){  //在起始终止点之间循环
                    qDebug()<<"::"<<m;
                    painter.fillRect(QRect(x,y,10,10),QBrush(QColor(255,0,0)));
                    x0=x0-10*m;
                    x=(int)(x0+5);
                    if(x%10>5){
                        x=x/10*10+10;
                    }
                    else{
                        x=x/10*10;
                    }
                    y-=10;           //y需要减小
                }
            }
        }

        /*Bresenham算法*/
        if(Bresen==1){
            /*计算Δx和Δy*/
            int delta_x=end.rx()-start.rx();
            int delta_y=end.ry()-start.ry();
            /*记录xy起始点坐标并取整*/
            int x=start.rx();
            int y=start.ry();
            x=x/10*10;
            y=y/10*10;

            int p=2*delta_y-delta_x;          //计算p0
            float k=(end.ry()-start.ry())*1.0/(end.rx()-start.rx())*1.0;   //计算斜率
            qDebug()<<k;
            /*斜率在0-1之间的情况*/
            if(k>=0&&k<=1){
                while(x<end.rx()){                //在终止点之前循环计算
                    painter.fillRect(QRect(x,y,10,10),QBrush(QColor(0,255,0)));    //填充坐标点对应的正方形方格
                    x+=10;                          //计算x_(k+1)
                    /*根据p_k的值判断计算p_(k+1)*/
                    if(p<0){
                        p=p+2*delta_y;           //y_(k+1)不变
                    }
                    else if(p>0){
                        p=p+2*delta_y-2*delta_x;
                        y+=10;                  //y_(k+1)=//y_k+1
                    }
                }
            }
            /*斜率大于1的情况 计算与上面相同，但xy需要对调*/
            else if(k>1){
                while(y<end.ry()){
                    painter.fillRect(QRect(x,y,10,10),QBrush(QColor(0,255,0)));
                    y+=10;
                    if(p<0){
                        p=p+2*delta_x;
                    }
                    else if(p>0){
                        p=p+2*delta_x-2*delta_y;
                        x+=10;
                    }
                }
            }
            /*斜率在-1 - 0*/
            else if(k<0&&k>-1){
                while(x<end.rx()){                //在终止点之前循环计算
                    painter.fillRect(QRect(x,y,10,10),QBrush(QColor(0,255,0)));    //填充坐标点对应的正方形方格
                    x+=10;                          //计算x_(k+1)
                    /*根据p_k的值判断计算p_(k+1)*/
                    //需要重新推导公式，变为了靠近lower时y减小，并且y_(k+1)-y_k为-1，公式变为+Δx
                    if(p<0){
                        p=p+2*delta_y+2*delta_x;
                        y-=10;
                    }
                    else if(p>0){
                        p=p+2*delta_y;
                    }
                }
            }
            /*斜率<-1 xy对调，同斜率为整数的情况*/
            else if(k<-1){
                while(y>end.ry()){
                    painter.fillRect(QRect(x,y,10,10),QBrush(QColor(0,255,0)));
                    y-=10;                 //每一步y需要减小
                    if(p<0){
                        p=p+2*delta_x;
                    }
                    else if(p>0){
                        p=p+2*delta_x+2*delta_y;        //Δy为负值，所以变为+Δy
                        x+=10;
                    }
                }
            }
        }

}

/*鼠标事件，记录起始点和终止点的坐标*/
void MyLabel::mousePressEvent(QMouseEvent *event){
    start=event->globalPos();
    start=this->mapFromGlobal(start);
}

void MyLabel::mouseReleaseEvent(QMouseEvent *event){
    end=event->globalPos();
    repaint();          //释放鼠标后需要重绘label
}

void MyLabel::mouseMoveEvent(QMouseEvent *event){
    end=event->globalPos();
    end=this->mapFromGlobal(end);
}
