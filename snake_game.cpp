#include "snake_game.h"
#include "ui_snake_game.h"

snake_game::snake_game(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::snake_game)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0,0,500,500);
    ui->centralWidget->grabKeyboard();
    ui->centralWidget->setFocus();
    ui->canvas->setScene(scene);
    ui->canvas->setAlignment(Qt::AlignCenter);
    time = new QTimer(this);
    t=500;
    mice.setX(100);
    mice.setY(100);
    point_arr.push_back(QPoint(20,20));
    point_arr.push_back(QPoint(40,20));
    connect(time,SIGNAL(timeout()) ,this,SLOT(run_in_loop()));
    time->start(t);
}
snake_game::~snake_game()
{
    this->releaseKeyboard();
    delete ui;
}
void snake_game::run_in_loop()
{
    scene->clear();
    scene->addRect(0,0,scene->width()+2,scene->height()+2,QPen(Qt::red,4,Qt::SolidLine));
    int n=point_arr.size()-1,d=-1;
    QPoint p=point_arr[n];
    if( m_distance(p,mice)<=2 )
    {
        d = 1;
        mice.rx() =  rand()%qint16(scene->width());
        mice.ry() = rand()%qint16(scene->height());
        while(in_snake(mice))
        {
            mice.rx() =  rand()%qint16(scene->width());
            mice.ry() = rand()%qint16(scene->height());
        }
    }
    first();
    last(d);
    for(int i=0;i<point_arr.size()-1;i++)
    {
        scene->addLine(QLineF(point_arr[i],point_arr[i+1]),QPen(Qt::black,2,Qt::DashLine));
    }
    scene->addRect(mice.x()-2,mice.y()-2,4,4,QPen(Qt::blue,2,Qt::SolidLine));
//    scene->addSimpleText(QString::number(total_length()));
    ui->statusBar->showMessage("Total length: "+QString::number(total_length()));
}

float snake_game::length(QPoint p1, QPoint p2)
{
    float len=0;
    len = sqrt((p1.x()-p2.x())*(p1.x()-p2.x()) + (p1.y()-p2.y())*((p1.y()-p2.y())));
    return len;
}

void snake_game::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Left:
        send_left();
        break;
    case Qt::Key_Right:
        send_right();
        break;
    case Qt::Key_Up:
            send_up();
        break;
    case Qt::Key_Down:
        send_down();
        break;
    default:
        //QFrame::keyPressEvent(event);
        break;
    }
}

void snake_game::send_up()
{
    QPoint pl = QPoint(point_arr.last().x(),point_arr.last().y());
    pl.ry()--;
    if(!in_snake(pl))
    {
        last();
        point_arr.push_back(pl);
    }
}

void snake_game::send_down()
{
    QPoint pl = QPoint(point_arr.last().x(),point_arr.last().y());
    pl.ry()++;
    if(!in_snake(pl))
    {
        last();
        point_arr.push_back(pl);
    }
}

void snake_game::send_right()
{
    QPoint pl = QPoint(point_arr.last().x(),point_arr.last().y());
    pl.rx()++;
    if(!in_snake(pl))
    {
        last();
        point_arr.push_back(pl);
    }
}

void snake_game::send_left()
{
    QPoint pl = QPoint(point_arr.last().x(),point_arr.last().y());
    pl.rx()--;
    if(!in_snake(pl))
    {
        last();
        point_arr.push_back(pl);
    }
}

bool snake_game::is_boundary(QPoint p) //check if the point is within limits
{
    bool ret=false;
    if(p.x()==0 || p.y()==0 || p.x()==scene->width()-1 || p.y()==scene->height()-1)
        ret =true;
    return ret;
}

bool snake_game::in_snake(QPoint p) //check if the point intersects the snake.
{
    bool ret = false;
    int mn,mx;
    for(int i=0;i<point_arr.size()-1;i++)
    {
        if(p.y()==point_arr[i].y() && p.y()==point_arr[i+1].y())
        {
            mn = std::min(point_arr[i].x(),point_arr[i+1].x());
            mx =std::max(point_arr[i].x(),point_arr[i+1].x());
            if(p.x()>=mn && p.x()<=mx)
                ret = true;
        }
        else if (p.x()==point_arr[i].x() && p.x()==point_arr[i+1].x())
        {
            mn = std::min(point_arr[i].y(),point_arr[i+1].y());
            mx =std::max(point_arr[i].y(),point_arr[i+1].y());
            if(p.y()>=mn && p.y()<=mx)
                ret = true;
        }
    }
    return ret;
}

void snake_game::game_up() // game ended. Calculate the length and display in a message box.
{
    time->stop();
    int len=total_length();
    ui->centralWidget->releaseKeyboard();
    ui->centralWidget->clearFocus();
    QMessageBox *msg = new QMessageBox();
    msg->setWindowTitle(QString("GAME UP"));
    msg->setText("Total length of snake: "+QString::number(len));
    msg->addButton(QMessageBox::Ok);
    msg->addButton(QMessageBox::Close);
    connect(msg,SIGNAL(buttonClicked(QMessageBox::Ok)),time,SLOT(stop()));
    msg->show();
}
int snake_game::m_distance(QPoint p1,QPoint p2)
 {
     return (abs(p1.x()-p2.x())+abs(p1.y()-p2.y()));
 }

int snake_game::total_length()
{
    int len=0;
    for(int i=0;i<point_arr.size()-1;i++)
    {
        len+=m_distance(point_arr[i],point_arr[i+1]);
    }
    //int n = len/10;
    time->setInterval(t/len);
    return len;
}

void snake_game::last(int d)
{
    bool flag=true;
               if(point_arr[0].y()==point_arr[1].y()) //horizontal
               {
                   if(point_arr[0].x() <point_arr[1].x())
                   {
                       point_arr[0].rx()-=d;
                   }
                   else
                   {
                       point_arr[0].rx()+=d;
                   }
               }

               else //vertical
               {
                   if(point_arr[0].y() < point_arr[1].y())
                   {
                       point_arr[0].ry()-=d;
                       //point_arr[0].ry()++;
                   }
                   else
                   {
                       point_arr[0].ry()+=d;
                       //point_arr[0].ry()--;
                   }
               }
               if(length(point_arr[0],point_arr[1])<1)
                   flag = false;

               if(!flag)
                   {
                       point_arr.pop_front();
                   }
}

void snake_game::first()
{
    int n = point_arr.size()-1;
    QPoint p=point_arr[n];
    if(p.y()==point_arr[(n-1)].y()) //horizontal
    {
        if(p.x() >point_arr[(n-1)].x())
        {
            p.rx()++;
        }
        else
            p.rx()--;
    }

    else //vertical
    {
        if(p.y() > point_arr[(n-1)].y())
        {
            p.ry()++;
        }
        else
            p.ry()--;
    }


    if(!in_snake(p) && !is_boundary(p))
    {
        point_arr[n]=p;
    }
    else
    {
        game_up();
    }
}
