#include "process.h"



//指令类
Instruct::Instruct(){
    InsId = 0;          //初始id为0
    InsTime = 0;        //初始时间为0
    InsType = 1;        //初始状态为用户态
    LogAddr = 0;        //初始地址0
    RemainTime = InsTime;       //初始剩余运行时间同总的运行时间相同
}

void Instruct::Init(int ins_id){
    InsId = ins_id;
    LogAddr = ins_id*1024;      //后面需要加入随机数
    //生成随机的运行时间以及随机的指令类型还没有写
}

void Instruct::ChangeId(int target_id){
    InsId = target_id;
}

//进程类
Process::Process(){
    ProcId = 0;             //初始进程号0
    ReferJobId = 0;         //初始分配的作业号0
    RunTime = 0;            //初始运行时间0
    ProcState = 1;          //初始运行状态为就绪
    InstructNum = 20;       //进程包含的指令数为random(10,20)
    InstructArray = new Instruct[InstructNum];      //默认生成的指令组
    CurrentInsID = 1;       //默认开始执行第一条指令
}

Process::Process(int proc_id, int refer_job_id, int ins_num){
    ProcId = proc_id;             //设定的进程id
    ReferJobId = refer_job_id;         //初始分配的作业号
    RunTime = 0;            //初始运行时间0
    ProcState = 1;          //初始运行状态为就绪
    InstructNum = ins_num;       //设定进程包含的指令数
    InstructArray = new Instruct[InstructNum];      //默认生成的指令组
    CurrentInsID = 1;       //默认开始执行第一条指令

    for(int i=0; i < InstructNum; i++){
        InstructArray[i].Init(i+1);     //初始化进程的指令集
    }
}

//创建进程
void Process::ForkProc(ProcTable &proc_table, ProcQueue &ready){
    Process temp_proc;
    temp_proc = *this;          //当前的进程
    ready.AppendProc(temp_proc);        //将该进程送入到就绪队列中去
    proc_table.InsertProc(temp_proc, ProcId);       //将该进程送到进程表中，位置是其进程号
}

//阻塞进程
//关于cpu现场保护部分还没有写
void Process::BlockProc(ProcTable &proc_table, ProcQueue &block){
    ProcState = 0;      //修改其状态为阻塞
    block.AppendProc(proc_table.process[ProcId]);       //送入到阻塞队列中
}

//kill进程
void Process::KillProc(ProcTable &proc_table, ProcQueue &finish){}

//唤醒进程
void Process::WakeProc(ProcTable &proc_table, ProcQueue &ready, ProcQueue &block){
    Process target_process;
    block.PopProc(target_process);      //从阻塞队列中取出进程
    ready.AppendProc(target_process);   //将该进程送入到就绪队列中去
    proc_table.process[ProcId].ProcState = 1;       //重新将该进程的状态置为1
}

//进程表类
ProcTable::ProcTable(){
    process = new Process[20];      //进程表中包含着20个进程
    length = 0;     //默认进程表的长度为0
}

void ProcTable::InsertProc(Process target_process, int location){       //插入进程
    int loc_point;      //定位
    if(!(location < 1 || location > length + 1)){       //如果没有越界的话
        length++;
        for(loc_point = length -1;loc_point > location -1; loc_point--){
            process[loc_point+1] = process[loc_point];
        }
        process[loc_point+1] = target_process;
    }
}

void ProcTable::DelProc(int proc_id){}
void ProcTable::SearchProc(int proc_id, Process &target_process){
    int loc_point;      //定位
    //int flag = 0;       //标志位，判断是否找到了该进程
    for(loc_point = 1; loc_point < length + 1; loc_point++){
        if(process[loc_point].ProcId == proc_id){

            target_process = process[loc_point];
        }
    }
}

//进程队列
ProcQueue::ProcQueue(){
    rear = new Qnode;       //初始化尾结点
    head = rear;            //头结点=尾结点
    head->next = nullptr;
}

void ProcQueue::AppendProc(Process process){        //链队列插入进程的操作
    ProcNodePtr p;
    p = new Qnode;
    p->data = process;
    p->next = nullptr;
    rear->next = p;
    rear = p;
}

void ProcQueue::PopProc(Process &process){          //出队列
    if(head != rear){       //队列不为空
        ProcNodePtr p;
        p = head->next;
        process = p->data;
        head->next = p->next;
        if(rear == p){
            rear = head;        //如果p是最后一个元素，则将首尾相接
        }
    }
}

void ProcQueue::GetTop(Process &process){       //获得队首进程
    if(head != rear){
        ProcNodePtr p;
        p = head->next;
        process = p->data;
    }
}

int ProcQueue::GetLength(){         //获得队列的长度
    int count = 0;
    ProcNodePtr p;
    p = head->next;
    while(p){
        count++;
        p = p->next;
    }
    return count;
}
