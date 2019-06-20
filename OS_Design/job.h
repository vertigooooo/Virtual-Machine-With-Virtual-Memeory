#ifndef JOB_H
#define JOB_H
#include "page.h"
#include "process.h"
#include "hardware.h"
//作业类
class Job{
public:
    int JobId;      //作业号
    int ReferProcId;        //包含的进程号
    int JobTime;    //作业执行完毕需要的时间
    int MemorySize; //作业执行需要的内存空间，以页为单位
    int MemoryAddress;      //内存地址

    Job();
    void Init(int job_id, int memory_size);     //初始化函数
};

//作业表类
class JobTable{
public:
    Job *job;
    int length;

    JobTable();

    void InsertJob(Job target_job, int location);       //插入作业
    void SearchJob(int job_id, Job &target_job);       //查找作业并返回结果
    void DelJob(int job_id);          //删除作业

};

//作业队列，用于存储生成的作业，便于后面进行调度
//作业结点
typedef struct JQnode{
    Job data;
    struct JQnode *next;
}JobNode, *JobNodePtr;

class JobQueue{
public:
    JobNodePtr head;    //头结点
    JobNodePtr rear;    //尾结点

    JobQueue();
    void AppendJob(Job temp_job);       //job入队
    void PopJob(Job &target_job);          //job出队
    void GetTop(Job &target_job);          //取队首元素
    int GetLength();        //获取队列长度
};



//作业调度类
class JobAllocate{
public:
    enum AllocRes{
        success,
        fail
    }AllocationResult;      //定义枚举类型，判断分配作业资源是否成功

    JobAllocate();
    AllocRes ScheduleJob(JobQueue &job_queue, Job &target_job,
                         Memory &memory, PageTable &page_table,
                         ProcQueue &ready, JobTable job_table,
                         ProcTable &proc_table );     //分配资源给作业

    void ChooseJob(JobQueue &job_queue, Job &target_job);   //根据先来先服务原则选择作业
    int AllocateResource(Memory &memory, Job job, PageTable &page_table,
                         JobQueue &job_queue, JobTable &job_table);     //分配资源
    void CreateProcess(Job temp_job, ProcTable &proc_table, ProcQueue &ready);  //为作业创建进程
};
#endif
