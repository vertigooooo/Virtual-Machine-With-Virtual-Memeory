#ifndef PROCESS_H
#define PROCESS_H

#include "page.h"
#include "hardware.h"


//指令类
class Instruct{
public:
    int InsId;      //指令号
    int InsTime;    //指令运行需要的时间
    int InsType;    //指令的类型， 1是用户态， 0是系统态
    int LogAddr;    //指令的逻辑地址，pc随机生成 ，-> ID*1024+random(0,1023)
    int RemainTime; //指令运行剩余需要的时间

    Instruct();
    void Init(int ins_id);      //初始化
    void ChangeId(int target_id);     //修改id

};

//进程类
class Process{
public:
    int ProcId;         //进程号
    int ReferJobId;     //对应的作业号
    int ProcState;      //进程的状态 0是阻塞， 1是就绪， 2是在运行
    int InstructNum;    //对应的作业的指令个数，用于判断作业是否完成
    int RunTime;        //进程的运行时间
    int CurrentInsID;   //当前运行的指令的id
    Instruct *InstructArray;    //进程包含的指令

    Process();
    Process(int proc_id, int refer_jobi_id, int proc_state, int ins_num);   //初始化

    void ForkProc(ProcTable &proc_table, ProcQueue &ready);    //就绪进程创建,送入到ready中去
    void BlockProc(ProcTable &proc_table, ProcQueue &block);   //进程阻塞
    void WakeProc(ProcTable &proc_table, ProcQueue &ready, ProcQueue &block);    //进程唤醒
    void KillProc(ProcTable &proc_table, ProcQueue &finish);    //进程kill

};

//进程表类
class ProcTable{
public:
    Process *process;       //存储的进程
    int length;     //表长

    ProcTable();

    void InsertProc(Process process, int location);     //插入进程
    void DelProc(int proc_id);      //根据进程号删除进程
    void SearchProc(int proc_id, Process &target_process);    //根据进程号查找进程并返回

};

//进程队列类
typedef struct Qnode{       //队列结点类型
    Process data;
    struct Qnode *next;
}ProcNode, *ProcNodePtr;

class ProcQueue{           //进程队列
public:
    ProcNodePtr head;       //头结点
    ProcNodePtr rear;       //尾结点

    ProcQueue();
    void AppendProc(Process process);      //进程入队
    void PopProc(Process &process);         //进程出队
    void GetTop(Process &process);          //取队首进程
    int GetLength();        //获取队列长度

};
#endif // PROCESS_H
