#include "job.h"

//作业类
Job::Job(){
    JobId = 0;      //默认编号0
    ReferProcId = 0;    //默认没有分配进程
    MemorySize = 0;     //初始内存需求0,在此处简单定义为指令的个数
    MemoryAddress = 0;  //无地址
    JobTime = 0;        //运行时间0
}

void Job::Init(int job_id, int memory_size){
    JobId = job_id;
    MemorySize = memory_size;
}

//作业表类
JobTable::JobTable(){
    job = new Job[20];      //可以存储20个作业
    length = 0;
}

void JobTable::InsertJob(Job target_job, int location){ //插入作业
    int loc_point;
    if(!(location < 1 || location > length + 1)){
        length++;
        for(loc_point = length - 1; loc_point > location - 1; loc_point--){
            job[loc_point + 1] = job[loc_point];
        }
        job[loc_point + 1] = target_job;
    }
}

void JobTable::SearchJob(int job_id, Job &target_job){      //查找作业并返回结果
    int loc_point;
    for(loc_point = 1;loc_point < length + 1; loc_point++){
        if(job[loc_point].JobId == job_id){
            target_job = job[loc_point];
        }
    }
}


//作业队列类
JobQueue::JobQueue(){
    rear = new JQnode;
    head = rear;
    head->next = nullptr;
}

void JobQueue::AppendJob(Job temp_job){     //作业入队
    JobNodePtr p;
    p = new JQnode;
    p->data = temp_job;
    rear->next = p;
    rear = p;
}

void JobQueue::PopJob(Job &target_job){     //作业出队，返回给target_job参数中
    JobNodePtr p;
    p = head->next;
    target_job = p->data;
    head->next = p->next;
    if(rear == p) head = rear;
    delete p;
}

void JobQueue::GetTop(Job &target_job){    //获取队首作业
    JobNodePtr p;
    p = head->next;
    target_job = p->data;
}

int JobQueue::GetLength(){
    if(head == rear){
        return 0;
    }else{
        int count = 0;
        JobNodePtr p;
        p = head->next;
        while(p){
            count++;
            p = p->next;
        }
        return count;
    }
}

//作业调度
JobAllocate::JobAllocate(){}

//调度主函数
JobAllocate::AllocRes JobAllocate::ScheduleJob(JobQueue &job_queue, Job &target_job,
                                          Memory &memory, PageTable &page_table,
                                          ProcQueue &ready, JobTable job_table,
                                          ProcTable &proc_table){
    ChooseJob(job_queue, target_job);   //选择作业
    int i = AllocateResource(memory, target_job, page_table, job_queue, job_table); //分配资源

    if(i == 1){     //如果资源分配成功
        CreateProcess(target_job, proc_table, ready);
        return success;

    }else{
        return fail;
    }
}

//选择作业
void JobAllocate::ChooseJob(JobQueue &job_queue, Job &target_job){
    if(job_queue.head != job_queue.rear){       //作业队列不为空
        job_queue.GetTop(target_job);
    }
}

//分配资源
int JobAllocate::AllocateResource(Memory &memory, Job job, PageTable &page_table,
                                   JobQueue &job_queue, JobTable &job_table){
    int i = memory.AllocateMem(job.MemorySize, job.JobId, page_table);
    if(i){
        job_queue.PopJob(job);
        job_table.job[job.JobId].ReferProcId = job.JobId;
        return 1;
    }else{
        return 0;
    }
}

//为作业创建进程
void JobAllocate::CreateProcess(Job temp_job, ProcTable &proc_table, ProcQueue &ready){
    Process temp_process = * new Process(temp_job.JobId, temp_job.ReferProcId,
                                       temp_job.MemorySize);
    temp_process.blockQueue =  temp_job.MemorySize
    temp_process.ForkProc(proc_table,ready);
}
