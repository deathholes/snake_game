#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <cmath>
#include <QTimer>
#include <QVector>
#include <QLineF>
#include <QKeyEvent>
#include <QMessageBox>


namespace Ui {
class snake_game;
}

class snake_game : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit snake_game(QWidget *parent = 0);

    ~snake_game();
    
private:
    Ui::snake_game *ui;
    QGraphicsScene *scene;
    QGraphicsLineItem *ln;
    QTimer *time;
    QVector<QPoint> point_arr;
    QPoint mice;
    int t=20;

    float length(QPoint,QPoint);
    bool in_snake(QPoint);
    bool is_boundary(QPoint);
    int m_distance(QPoint,QPoint);
    int total_length();
    void last(int d=-1);
    void first();
    void game_up();
    void send_up();
    void send_down();
    void send_right();
    void send_left();


protected:
    void keyPressEvent(QKeyEvent *);

public slots:
    void run_in_loop();

};

#endif // SNAKE_GAME_H
