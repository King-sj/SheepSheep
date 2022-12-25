#include "Headers/MainWindow.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi(this);
    this->setWindowTitle("SheepSheep");
    this->setFixedSize(this->width(),this->height());

    /****************************************************/

    pix = new QPixmap(this->width(),this->height());

    pix->fill(Qt::transparent);
    this->createCards();
    this->shuffleCards();//打乱
    //std::memset(this->sumCards,0,sizeof(this->sumCards));//初始化为0
    connect(this,&MainWindow::gameOver,this,&MainWindow::dealGameOver);
}

MainWindow::~MainWindow()
{
    delete pix;
    pix = NULL;
}

void MainWindow::paintEvent(QPaintEvent *e)
{
    e->accept();
    drawBackground();
    QRect rect(0,0,this->width(),this->height());
    QPainter painter(this);

    /*************************************************/
    painter.drawPixmap(rect,*pix);
    pix->fill(Qt::transparent);

    for(int i = this->cards.size() - 1 ; i >= 0 ; i--)
    {
        drawCard(cards[i]);//从后往前，选中从前往后，就会选中最顶部的了
    }
    painter.end();

}

void MainWindow::mouseReleaseEvent(QMouseEvent *e)
{
    Card* tempCard = new Card() , *otherTopCard = new Card();
    for(auto& c : this->cards)
    {
        if(isInCard(e->pos(),c))
        {
            if(c->deepth > tempCard->deepth)//通过位置获取card较为麻烦，没写，故此处deepth实际上没有更新
            {
                if(tempCard->deepth == -1)
                    delete tempCard;//释放内存
                tempCard = c;//获取最顶层的

                for(auto& card : this->cards)
                {
                    if(card == c)break;//再往后都在底层
                    if(isInCard(c->pos,card))//左上角
                    {
                        return;//被覆盖
                    }
                    if(isInCard(c->pos + QPoint(cardSize,0),card))//右上角
                    {
                        return;//被覆盖
                    }
                    if(isInCard(c->pos +QPoint(0,cardSize),card))//
                    {
                        return;//被覆盖
                    }
                    if(isInCard(c->pos + QPoint(cardSize,cardSize),card))//
                    {
                        return;//被覆盖
                    }
                }
                break;//直接跳出
            }

        }
    }
    if(tempCard->deepth != -1)
    {
        clickOn(tempCard);
    }else{
        delete tempCard;
        tempCard = NULL;
    }
//    delete tempCard;
//    tempCard = NULL;
    update();
    e->accept();
}

void MainWindow::drawCard(const Card *c)
{
    if(c->isDied)return;//已消除
    QPainter painter(this);
    QRect rec(c->pos,QSize(this->cardSize,this->cardSize));
    QImage img;
    img.load(c->path);
    QRect source(QPoint(0,0),img.size());

  //  cout<<img.size();
    painter.drawImage(rec,img,source);
    painter.end();

}

void MainWindow::drawBackground()
{
    QPainter painter(pix);
    QRect rec(0,this->height() - cardSize,this->width(),cardSize);
    QBrush bru(Qt::green);
    painter.setBrush(bru);
    painter.drawRect(rec);
    painter.drawLine(0,this->height() - cardSize,this->width(),this->height() - cardSize);
    painter.end();
}

void MainWindow::createCards()
{
    this->cards.clear();
    this->selectedCards.clear();
    liveNum = 0;
    cardNum = this->leavel*2;
    for(int i = 0 ; i < teamNum ; i++)
        for(int j = 0 ; j < foodListDeepth ; j++)
        {
            for(int k = 0 ; k < this->cardNum ; k++)
            {
                if(j != 0 && j != foodListDeepth - 1)
                {
                    this->cards.append(new Card(i,j,QPoint(0,0)));
                    this->liveNum++;
                }
                this->cards.append(new Card(i,j,QPoint(0,0)));
                this->liveNum++;
            }
        }
}

void MainWindow::shuffleCards()
{
    for(int j = this->cards.size() - 1 ; j > 0; j--)
    {
        std::swap(cards[ QRandomGenerator::global()->bounded(0,j) ],
                cards[j]);
    }//洗牌
    for(auto& card : this->cards)
    {
        card->pos = QPoint(QRandomGenerator::global()->bounded(10,this->width() - this->cardSize - 10)
                           ,QRandomGenerator::global()->bounded(10,(this->height() - 2*cardSize - 10)));
    }
}

bool MainWindow::isInCard(QPoint p, Card* c)
{
    if(c->isDied || c->isSelected)return 0;
    if(p.x() > c->pos.x() && p.y() > c->pos.y()
            && p.x() < c->pos.x() + cardSize && p.y() < c->pos.y() + cardSize)
        return 1;
    return 0;
}

void MainWindow::clickOn(Card* c)
{
    //todo:选中速度太慢 -> table储存，空间换时间
    //todo:覆盖状态的更新
    //note:与预期不符，上述未实现
    c->isSelected = true;
    int selectedNum = this->selectedCards.size();
    c->pos = QPoint(5+ selectedNum*cardSize,this->height() - cardSize);
    this->selectedCards.append(c);
    liveNum--;//更新还在未选区的数量
    if(liveNum == 0)
    {//通关了
        this->leavel++;

        QMessageBox msb;
        msb.setText("下一关 : 第" + QString::number(this->leavel) + "关");
        msb.exec();
        createCards();
        shuffleCards();
        update();
        //下一关
        return;
    }
    eat();//尝试消除

    if(this->selectedCards.size() == maxSelectedNum)emit gameOver();//游戏结束

    int n = 0;
    for(auto card : selectedCards)
    {
        card->pos = QPoint(5+ n*cardSize,this->height() - cardSize);
        n++;
    }
}

void MainWindow::eat()
{
    auto temp = this->selectedCards[this->selectedCards.count()-1];//最后一个
    int pos = 0;
    for(auto c : this->selectedCards)
    {
        if(temp->team == c->team && temp->level != c->level)
        {
            temp->isDied = true;
            c->isDied = true;
            this->selectedCards.removeAt(pos);
            this->selectedCards.removeLast();
            return;
        }
        pos++;
    }
}

void MainWindow::dealGameOver()
{
    QMessageBox msb;
    msb.setText("Failed!Do you want to start again?");
    msb.setStandardButtons(QMessageBox::Ok|QMessageBox::No);
    msb.setDefaultButton(QMessageBox::No);
    auto res = msb.exec();
    if(res == QMessageBox::Ok)
    {
        leavel = 1;
        createCards();
        shuffleCards();
    }else{
        this->close();//退出
    }
}

