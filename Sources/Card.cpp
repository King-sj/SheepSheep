#include "Headers/Card.h"

Card::Card() : Card(-1,-1,QPoint(-1,-1))
{
    deepth = -1;
}

Card::Card(int t, int l, QPoint pos)
{
    this->team = t;
    this->level = l;
    this->pos = pos;
    this->path = ":/img" + QString::number(team+1) + "/img/" +QString::number(team+1)
            + "/"+QString::number(level+1)+".jpg";

}

