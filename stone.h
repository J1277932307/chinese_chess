#ifndef STONE_H
#define STONE_H

#include <QString>
//棋子类
class Stone
{
public:
    Stone();
    ~Stone();

public:
    enum class TYPE{JU,MA,XIANG,SHI,JIANG,PAO,ZU};         //定义一个枚举类型，表示象棋棋子(车，马，象，士，将，炮，卒)

    int row;   //棋子所在行
    int col;   //棋子所在列
    int id;    //棋子id
    bool dead; //棋子状态，是否死亡
    bool red;  //棋子属于哪一方
    TYPE type;  //棋子类型

public:
    QString getText();
    void init(int id,bool changePos);

};

#endif // STONE_H
