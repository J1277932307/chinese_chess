#include "stone.h"

Stone::Stone()
{


}

Stone::~Stone()
{

}

QString Stone::getText()        //获取对应棋子的文字
{
    switch (this->type) {
    case TYPE::JU:
        return QString::fromLocal8Bit("車");


    case TYPE::MA:
        return QString::fromLocal8Bit("馬");


    case TYPE::XIANG:
        if(red)
        {
            return QString::fromLocal8Bit("相");
        }
        return QString::fromLocal8Bit("象");


    case TYPE::SHI:
        return QString::fromLocal8Bit("士");


    case TYPE::JIANG:
        if(red)
        {
            return QString::fromLocal8Bit("将");
        }
        return QString::fromLocal8Bit("帅");


    case TYPE::PAO:
        return QString::fromLocal8Bit("炮");


    case TYPE::ZU:
        if(red)
        {
            return QString::fromLocal8Bit("兵");
        }
        return QString::fromLocal8Bit("卒");

    default:
        return QString::fromLocal8Bit("错误");
    }



}

//初始化每个棋子
void Stone::init(int id,bool changePos)
{

    struct {
            int row, col;
            TYPE type;
        } pos[16] = {
        {0, 0, TYPE::JU},
        {0, 1, TYPE::MA},
        {0, 2, TYPE::XIANG},
        {0, 3, TYPE::SHI},
        {0, 4, TYPE::JIANG},
        {0, 5, TYPE::SHI},
        {0, 6, TYPE::XIANG},
        {0, 7, TYPE::MA},
        {0, 8, TYPE::JU},

        {2, 1, TYPE::PAO},
        {2, 7, TYPE::PAO},
        {3, 0, TYPE::ZU},
        {3, 2, TYPE::ZU},
        {3, 4, TYPE::ZU},
        {3, 6, TYPE::ZU},
        {3, 8, TYPE::ZU},
        };

    this->id = id;
    this->dead = false;
    this->red = id < 16;



    if(id < 16)
    {
        this->col = pos[id].col;
        this->row = pos[id].row;
        this->type = pos[id].type;
    }else
    {
        //此处是用对称原理，例如棋盘两个对角的“车”，其y坐标和为8，x坐标和为9
        this->col = 8 - pos[id-16].col;
        this->row = 9 - pos[id-16].row;
        this->type = pos[id-16].type;
    }

}
