#pragma once
#include<QString>
#include<QPoint>
#include<QString>
class Card
{
public:
    Card();
    Card(int t,int l,QPoint pos);
public:
    int team;//组别
    int level;//0为最高
    int deepth = 0;//深度，0在最底部
    QPoint pos;
    QString path;
    bool isDied = false;
    bool isSelected = false;
    bool isCovered = false;//是否被覆盖//todo
};

