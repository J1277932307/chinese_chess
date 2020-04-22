#ifndef BOARD_H
#define BOARD_H

#include <QWidget>
#include <QMouseEvent>
#include "stone.h"

//棋盘类
class Board : public QWidget
{
    Q_OBJECT
public:
    explicit Board(QWidget *parent = nullptr);

public:
    void paintEvent(QPaintEvent *event);
    void drawStrone(QPainter& painter,int id);                            //绘制棋子
    QPoint center(int row,int col);                                       //获取棋子坐标中心点
    QPoint center(int id);                                                //获取棋子坐标中心点
    void mouseReleaseEvent(QMouseEvent *event);
    bool getRowCol(QPoint pt,int& row,int& col);                          //当点击鼠标时，确认是否点击到棋子上
    void getRowCol(int id,int& row,int& col);                             //获取给定id棋子的位置
    bool canMove(int moveid,int row,int col,int killid);                  //判断棋子是否可以按规则移动
    bool canMoveJU(int moveid,int row,int col,int killid);
    bool canMoveMA(int moveid,int row,int col,int killid);
    bool canMoveXIANG(int moveid,int row,int col,int killid);
    bool canMoveSHI(int moveid,int row,int col,int killid);
    bool canMoveJIANG(int moveid,int row,int col,int killid);
    bool canMovePAO(int moveid,int row,int col,int killid);
    bool canMoveZU(int moveid,int row,int col,int killid);
    int getStoneCountAtLine(int row1, int col1, int row2, int col2);      //计算直线两点之间,存在多少棋子
    int getStoneId(int row,int col);
    int relation(int row1,int col1,int row2,int col2);                    //计算两点之间位置关系
    bool isBottomSide(int id);                                            //计算给定棋子是在棋盘的上半部分还是下半部分
    void afterVictory(bool isRed);                                                  //某方赢棋之后


private:
    Stone stone[32];      //32颗棋子
    int r;                //棋子的半径
    int selectid;         //被鼠标点击的棋子id
    bool redTurn;
    bool bSize;

signals:

};

#endif // BOARD_H
