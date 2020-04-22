#include <QPainter>
#include <QRect>
#include <QMessageBox>
#include "board.h"

Board::Board(QWidget *parent) : QWidget(parent)
{
    for(int i = 0;i < 32;++i)
    {
        stone[i].init(i,false);
    }
    selectid = -1;
    redTurn = true;
    bSize = redTurn;
}

void Board::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    int d = 50;
    r = d / 2;
    //绘制棋盘，首先把棋盘的横线画出来
    for(int i = 1; i <=10; ++i)
    {
        painter.drawLine(QPoint(d,i*d),QPoint(9*d,i*d));
    }
    //然后把棋盘的竖线画出来，这里注意的是除了两边的线，中间的所有竖线都有 "楚河汉界" 隔断
    for(int i = 1; i <= 9; ++i)
    {
        if(i == 1 || i == 9)
        {
            painter.drawLine(QPoint(i*d,d),QPoint(i*d,10*d));
        }else
        {
            painter.drawLine(QPoint(i*d,d),QPoint(i*d,5*d));
            painter.drawLine(QPoint(i*d,6*d),QPoint(i*d,10*d));
        }

    }
    //绘制九宫的斜线
    painter.drawLine(QPoint(4*d,d),QPoint(6*d,3*d));
    painter.drawLine(QPoint(6*d,d),QPoint(4*d,3*d));

    painter.drawLine(QPoint(4*d,8*d),QPoint(6*d,10*d));
    painter.drawLine(QPoint(6*d,8*d),QPoint(4*d,10*d));


    //绘制棋子
    for(int i = 0;i < 32;++i)
    {
        drawStrone(painter,i);
    }




}

//绘制棋子
void Board::drawStrone(QPainter &painter, int id)
{
    if(stone[id].dead)
    {
        return;
    }
    QPoint c = center(id);   //获取棋子的坐标中点
    if(id == selectid)
    {
        painter.setBrush(Qt::gray);
    }else
    {
        painter.setBrush(Qt::yellow);
    }


    painter.setPen(Qt::black);


    painter.drawEllipse(c,r,r);  //围绕座标中点绘制圆形，圆形半径为棋盘格子边长的一半
    QRect rect = QRect(c.x() - r,c.y() - r,r*2,r*2);


    if(stone[id].red)
    {
        painter.setPen(Qt::red);
    }
    painter.setFont(QFont("STKaiti",r,500));
    painter.drawText(rect,stone[id].getText(),QTextOption(Qt::AlignCenter));         //绘制象棋文字
}

//返回象棋棋盘行列对应的坐标点
QPoint Board::center(int row, int col)
{
    QPoint ret;
    ret.rx() = (col + 1) * r * 2;
    ret.ry() = (row + 1) * r * 2;
    return ret;


}

QPoint Board::center(int id)
{
    return center(stone[id].row,stone[id].col);
}

void Board::mouseReleaseEvent(QMouseEvent *event)
{
    //获取鼠标点击位置
    QPoint pt = event->pos();
    int row,col;
    bool bRet = getRowCol(pt,row,col);
    if(bRet == false)   //点击到棋盘外面了
    {
        return;
    }
    int i;
    int clickid = -1;
    for(i = 0;i < 32;++i)
    {
        if(stone[i].row == row && stone[i].col == col && stone[i].dead == false)
        {
            break;
        }
    }
    if(i < 32)
    {
        clickid = i;

    }

    if(-1 == selectid)           //点击
    {
        if(-1 != clickid)
        {
            if(redTurn == stone[clickid].red)
            {
                selectid = clickid;
                update();
            }

        }
    }else         //移动
    {
        if(canMove(selectid,row,col,clickid)) //判断是否符合移动规则
        {
            stone[selectid].row = row;
            stone[selectid].col = col;
            if(clickid != -1)
            {
                stone[clickid].dead = true;
                if(stone[clickid].type == Stone::TYPE::JIANG)
                {
                   afterVictory(stone[clickid].red);
                }
            }
            redTurn = !redTurn;
            selectid  = -1;
            update();
        }

    }




}


bool Board::getRowCol(QPoint pt, int &row, int &col)
{
    for(row = 0;row <= 9;row++)
    {
        for(col = 0;col <= 8;col++)
        {
            QPoint c = center(row,col);
            int dx = c.x() - pt.x();
            int dy = c.y() - pt.y();
            int dist = dx*dx + dy*dy;
            if(dist < r*r)
                return true;
        }
    }
    return false;
}

void Board::getRowCol(int id, int &row, int &col)
{
   row = stone[id].row;
   col = stone[id].col;
}

bool Board::canMove(int moveid, int row, int col, int killid)
{
    if(stone[moveid].red == stone[killid].red)
    {
        selectid = killid;
        update();
        return false;
    }

    switch (stone[moveid].type)
    {
    case Stone::TYPE::JU:
        return canMoveJU(moveid,row,col,killid);

    case Stone::TYPE::MA:
        return canMoveMA(moveid, row,col,killid);

    case Stone::TYPE::XIANG:
        return canMoveXIANG(moveid,row,col,killid);

    case Stone::TYPE::SHI:
        return canMoveSHI(moveid,row,col,killid);

    case Stone::TYPE::JIANG:
        return canMoveJIANG(moveid,row,col,killid);

    case Stone::TYPE::PAO:
        return canMovePAO(moveid,row,col,killid);

    case Stone::TYPE::ZU:
        return canMoveZU(moveid,row,col,killid);


    }




}

bool Board::canMoveJU(int moveid, int row, int col, int killid)
{
    int row1;
    int col1;
    getRowCol(moveid,row1,col1);
    int ret = getStoneCountAtLine(row1,col1,row,col);  //计算两个位置之间是否有其他棋子挡路
    if(ret == 0)
    {
        return true;
    }
    return false;
}

bool Board::canMoveMA(int moveid, int row, int col, int killid)
{
    int row1,col1;
    getRowCol(moveid,row1,col1);             //通过给定棋子id，获取棋子的横竖坐标
    int r = relation(row1,col1,row,col);
    if(r != 12 && r != 21)                   //21表示是竖"日"移动，12表示是横"日"移动
    {
        return false;
    }
    if(r == 12)
    {
        if(getStoneId(row1,(col+col1)/2) != -1)       //横"日"，计算别马脚
        {
            return false;
        }
    }else
    {
        if(getStoneId((row+row1)/2,col1) != -1)       //竖"日"计算别马脚
        {
            return false;
        }

    }

    return true;

}

bool Board::canMoveXIANG(int moveid, int row, int col, int killid)
{
    int row1,col1;
    getRowCol(moveid,row1,col1);
    int r = relation(row1,col1,row,col);
    if(r != 22)
        return false;

    //计算象眼
    int rEye = (row + row1) /2;
    int cEye = (col + col1) /2;
    if(getStoneId(rEye,cEye) != -1)  //如果象眼有子
    {
        return false;
    }

    if(!stone[moveid].red)
    {
        if(row < 4)
        {
            return false;
        }
    }else
    {
        if(row > 5)
        {
            return false;
        }
    }
    return true;
}

bool Board::canMoveSHI(int moveid, int row, int col, int killid)
{

    if(stone[moveid].red) //判断行是否在范围内
    {
        if(row > 2)
        {
            return false;
        }
    }else
    {
        if(row < 7)
        {
            return false;
        }
    }
    if(col <3 || col > 5)   //判断列是否在范围内
    {
        return false;
    }

    int dr = stone[moveid].row - row;
    int dc = stone[moveid].col - col;

    int d = abs(dr)*10 + abs(dc);  //判断其走步数，原理同 老将 的计算方法
    if(d == 11)
    {
        return true;
    }
    return false;
}

bool Board::canMoveJIANG(int moveid, int row, int col, int killid)
{
    /*
     * 移动范围在九宫内
     * 移动步长是一个格子
     * 双方老将见面会飞将
     */
    if(killid != -1 && stone[killid].type == Stone::TYPE::JIANG)       //判断“飞将”情况，可以用"車"的逻辑
    {
        return canMoveJU(moveid,row,col,killid);
    }

    if(stone[moveid].red) //判断行是否在范围内
    {
        if(row > 2)
        {
            return false;
        }
    }else
    {
        if(row < 7)
        {
            return false;
        }
    }
    if(col <3 || col > 5)   //判断列是否在范围内
    {
        return false;
    }


    /*
     * 判断走位
     * 老将 一次只能走一步，如果是竖向移动，x坐标未变，那么dr必然为零，dc的绝对值是1
     * 如果是横向移动，y坐标未变，那么dc必然为零，dr的绝对值是1
     */
    int dr = stone[moveid].row - row;  //横向移动差值
    int dc = stone[moveid].col - col;  //竖向移动差值

    int d = abs(dr)*10 + abs(dc);    //后面这个方法被relation()函数替代
    if(d == 1 || d == 10)
    {
        return true;
    }
    return false;


}

bool Board::canMovePAO(int moveid, int row, int col, int killid)
{
    int row1,col1;
    getRowCol(moveid,row1,col1);
    int ret = getStoneCountAtLine(row1,col1,row,col);    //计算两点之间隔着多少子
    if(killid != -1)
    {
        if(ret == 1)     //如果中间隔着一个子，表示吃子
            return true;
    }else
    {
        if(ret == 0)       //中间没有隔着子，表示移动
            return true;
    }
    return false;
}

bool Board::canMoveZU(int moveid, int row, int col, int killid)
{
    int row1,col1;
    getRowCol(moveid,row1,col1);
    int r = relation(row1,col1,row,col);
    if(r != 1 && r!= 10)
    {
        return false;
    }
    if(stone[moveid].red)
    {
        if(row < row1)
        {
            return false;
        }
        if(row1 <= 4 && row == row1)
        {
            return false;
        }
    }else
    {
        if(row > row1)
        {
            return false;
        }
        if(row1 >= 5 && row == row1)   //未过河的情况
        {
            return false;
        }
    }
    return true;
}

int Board::getStoneCountAtLine(int row1, int col1, int row2, int col2)
{
    int ret = 0;
    if(row1 != row2 && col1 != col2)       //两点不在一条直线
    {
        return -1;
    }
    if(row1 == row2 && col1 == col2)       //两点是同一位置
    {
        return -1;
    }

    if(row1 == row2)         //同一行，表示棋子要横向移动，计算横向移动的列数
    {
        int min = col1 < col2 ? col1 : col2;
        int max = col1 > col2 ? col1 : col2;

        for(int col = min + 1;col < max; ++col) //计算横向直线上是否有其他棋子挡路
        {
            if(getStoneId(row1,col) != -1)
            {
                ++ret;
            }
        }
    }else
    {
        int min = row1 < row2 ? row1 : row2;
        int max = row1 < row2 ? row2 : row1;
        for(int row = min+1; row<max; ++row)     //计算竖向直线上是否有其他棋子挡路
        {
            if(getStoneId(row, col1) != -1)
            {
                ++ret;
            }
        }
    }
    return ret;

}

int Board::getStoneId(int row, int col)    //返回在给定行列上的棋子的id
{
    for(int i = 0;i < 32;++i)
    {
        if(stone[i].row == row && stone[i].col == col && !stone[i].dead)
        {
            return i;
        }
    }
    return -1;
}

int Board::relation(int row1, int col1, int row2, int col2)   //计算两点之间位置关系，详解看canMoveJIANG的说明
{
    return qAbs(row1 - row2) * 10 + qAbs(col1 - col2);
}

bool Board::isBottomSide(int id)
{
    return bSize == stone[id].red;
}

void Board::afterVictory(bool isRed)
{
    QMessageBox::StandardButton but;
    if(isRed)
    {
        but = QMessageBox::information(nullptr,QString::fromLocal8Bit("胜负已分"),QString::fromLocal8Bit("黑方胜利!\n是否继续？"),QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
    }else{
        but = QMessageBox::information(nullptr,QString::fromLocal8Bit("胜负已分"),QString::fromLocal8Bit("红方胜利!\n是否继续？"),QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
    }

    if (but == QMessageBox::Yes)
    {
      for(int i = 0; i < 32;++i)
      {
          stone[i].init(i,true);

      }
      redTurn = !redTurn;
      update();
    }else
    {
        exit(0);
    }
}


