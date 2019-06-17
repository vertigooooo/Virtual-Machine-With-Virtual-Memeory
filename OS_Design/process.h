#ifndef PROCESS_H
#define PROCESS_H

#include "page.h"

//进程类
class Process{
public:


};

//PCB
class PCB{
public:

};

//进程队列类
typedef struct Qnode{       //队列结点类型
    Process data;
    struct Qnode *next;
}ProcNode, *ProcNodePtr;

class ProcQueue{           //进程队列
public:
    ProcNodePtr head;
    ProcNodePtr rear;

    ProcQueue();
    void AppendProc();      //进程入队
    void PopProc();         //进程出队
    void GetTop();          //取队首进程
    int GetLength();        //获取队列长度

};
#endif // PROCESS_H
