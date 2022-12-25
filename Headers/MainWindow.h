#pragma once

#include "ui_MainWindow.h"

#include <QMainWindow>
#include<QPaintEvent>
#include<QMouseEvent>
#include<QPixmap>
#include<QPainter>
#include"Headers/Card.h"
#include<QVector>
#include<QRandomGenerator>
#include<QMessageBox>
#include<QDebug>
#define cout qDebug()<<"["<<__LINE__<<":"<<__FILE__<<":"<<__FUNCTION__<<"]"
class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    virtual void paintEvent(QPaintEvent*);
    virtual void mouseReleaseEvent(QMouseEvent* e);//鼠标事件
    /*******************************************************/
    virtual void drawCard(const Card* c);
    virtual void drawBackground();
    virtual void createCards();
    virtual void shuffleCards();//洗牌
    virtual bool isInCard(QPoint , Card*);
    virtual void clickOn(Card*);
    virtual void eat();
    virtual void dealGameOver();
private:
    int leavel = 1;//关卡
    const int cardSize = 80;//
    QPixmap *pix;
    int cardNum =2;
    int teamNum = 3;
    int foodListDeepth = 3;
    int liveNum = 0;//还存在的卡数量
    const int maxSelectedNum = 7;
    QVector<Card*> cards;
    QVector<Card*> selectedCards;
signals:
    void gameOver();
};
