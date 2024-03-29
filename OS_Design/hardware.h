#ifndef HARDWARE_H
#define HARDWARE_H
#include <fstream>
#include <iostream>
using namespace std;
/**************************************************
    在我写这个文件时未定义但在该文件中出现的类
    1, Process

    另：CpuSceneProtect,CpuSceneRecovery还未实现
***************************************************/

//物理块
class Block{
public:
    //物理块号
    int BlockID;

    //物理块分配状态，0表示空闲，1表示占有
    int BlockState;

    //当前使用这个块的进程ID(?)
    int OwnerProcessID;

    //构造函数
    Block(){
        BlockID = -1;   //物理块ID从0开始
        BlockState = 0; //处于未分配状态
        OwnerProcessID = -1;    //分配给的进程的ID
    }

    Block(int ID){
        Block();
        SetBlockID(ID);
    }

    //对应原InitBlock()
    void SetBlockID(int ID){
        BlockID = ID;
    }
};

//物理块表
class BlockTable{
public:
    //基地址
    Block* blocks;

    //表长
    int length;

    //构造函数
    BlockTable(){
        blocks = new Block[200];
        length = 0;
    }

    //在Block_Table表的第i个数据元素之前插入一个Block x (Block_Table表的0位空出)
    void InsertBlock(Block inputBlock,int location){
        if(location < 1 || location > length + 1)
            cout << "插入位置错误" << endl;
        else{
            length++;
            int i;
            for (i = length - 1;i > location - 1;i--)
                blocks[i+1] = blocks[i];
            blocks[i + 1] = inputBlock;
        }
    }

    //根据物理块ID，查找物理块表中的某个物理块，返回给同类的参数
    void SearchBlockID(int blockID,Block &outputBlock){
        for(int i = 1;i < length;i++)
            if(blocks[i].BlockID == blockID){
                outputBlock = blocks[i];
                return;
            }
        cout << "未找到该ID对应的物理块" << endl;
        return;
    }

    //根据ID删除物理块表中该位置的物理块
    void DeleteBlockID(int blockID){
        for (int i = 1;i < length + 1;i++) {
            if(blocks[i].BlockID == blockID){
                //找到该块后:
                for (int k = i+1;k < length + 1;k++)
                    blocks[k-1] = blocks[k];
                length--;
                return ;
            }
        }
        cout << "BlockTable表中没有该ID的物理块" << endl;
        return;
    }

};

//对CPU的模拟，主要包含一些记录数据
class CPU{
public:
    //当前正在执行的进程ID
    int NowProcessID;

    //当前正在执行的进程的指令的序号
    int CurrentInsID;

    //记录当前CPU的状态
    enum StatusCode{
        FREE,
        BUSY
    } CpuStatus;

    //构造函数
    CPU(){
        NowProcessID = 1;
        CurrentInsID = 0;
        CpuStatus = FREE;
    }

    //CPU现场保护函数
    void CpuSceneProtect(Process &e){
        e.CurrentInsID = CurrentInsID;
        CpuStatus = FREE;
    }

    //CPU现场恢复函数
    void CpuSceneRecovery(Process e){
        NowProcessID = e.ProcId;
        CurrentInsID = e.CurrentInsID;
        CpuStatus = BUSY;
    }

};


//对内存空间的模拟,设定总内存空间大小为 物理块数量 x 物理块大小 = 100块 x 1000字节
class Memory{
public:
    //固定产生五次缺页中断
    const static int MissPageIntNum = 5;

    //物理块数量
    int BlockNumber;

    //每个物理块的大小
    int BlockSize;

    //总内存空间大小(默认为 BlockNumber x BlockSize )
    int TotalSize;

    //内存管理信息
    BlockTable blockTable;

    //剩余空间
    int RemainSpace;

    //已用空间
    int UsedSpace;



    //构造函数，默认大小为100 x 1000
    Memory(){
        //初始化内存大小
        BlockSize = 1000;
        BlockNumber = 100;
        TotalSize = BlockSize * BlockNumber;

        //初始化物理块表
        Block* blocks = new Block[BlockNumber];
        for (int i = 0;i < BlockNumber;i++) {
            blocks[i].SetBlockID(i);
            blockTable.InsertBlock(blocks[i],i+1);
        }
    }

    //分配内存，0表示分配失败，1表示分配成功
    //每个作业在同一时间内分配一次
    int AllocateMem(int needSize,int JobID,PageTable& page_table){

        //空闲块数 多于 需要的块数 时，则分配
        if((needSize - MissPageIntNum) < RemainSpace){

            //根据需要给他分配足够的页，同时后几个请求分页时会触发缺页中断
            for (int i  = 0;i < needSize;i++) {
                Page page;

                if(i < needSize - MissPageIntNum)
                    page.Init(0,1);
                else
                    page.Init(i,0);

                page_table.InsertPage(page, i+1);
            }
            int index_blocks = 1, index_pages = 1;
            needSize -= MissPageIntNum;

            //逐块对需要的块的数量进行分配
            while (needSize > 0) {

                //如果当前块是空闲的(0)，就进行分配
                if(blockTable.blocks[index_blocks].BlockState == 0){

                    //设置该物理块的信息：占用、对应进程的ID
                    blockTable.blocks[index_blocks].BlockState = 1;
                    blockTable.blocks[index_blocks].OwnerProcessID = JobID;
                    page_table.page[index_pages].BlockId = blockTable.blocks[index_blocks].BlockID;

                    //更新索引
                    RemainSpace--;UsedSpace++;needSize--;index_pages++;
                }
                index_blocks++;
            }
            return 1;//分配成功
        }
        else
            return 0;//分配失败
    }

    //回收内存
    int RecycleMem(int usedSize,int ProID){

        int i = 0;
        while(usedSize > 0)
        {
            if(blockTable.blocks[i].OwnerProcessID == ProID)//找出需要归还的块
            {
                blockTable.blocks[i].BlockState = 0;//置占用标志为0
                blockTable.blocks[i].OwnerProcessID = -1;//清空占有进程标志位
                RemainSpace++;//从空闲块数中减去本次占用块数
                UsedSpace--;
                usedSize--;
            }
            i++;
        }
    }

//    //把物理块表输出到文件
//    void Print(std::ofstream& file){

//        file<<"剩余空间（块）："<<RemainSpace<<"\r\n"<<"已用空间（块）："<<UsedSpace<<"\r\n";
//        file<<"内存物理块情况（第一个数：0为空闲，1为占用;第二个数：-1表示未分配给任何进程，其他数字表示占有改块的进程ID）:"<<"\r\n";
//        for(int i = 1;i <= blockTable.length;i++)
//        {
//            file<<blockTable.block[i].BlockId<<"("<<blockTable.block[i].BlockState<<","<<blockTable.block[i].OwnerPro<<")";
//            file<<"    ";
//            if(i % 10 == 0)
//            {
//                file<<"\r\n";
//            }
//        }
//        file<<"\r\n";
//    }
};


//存储器单元。页表操作(保存页表、地址转换、访问等)，发出缺页中断，
class MMU{
public:
    //页表的基址
    PageTable* pageTableAddr;



    //构造函数
    MMU() = default;

    //使用页号和偏移量访问内存（？）
    void pageManage(std::ofstream& file, PageTable& page_table,int addr,int& pageid, int& offset,int& paddr){
        setPageTableAddr(page_table);
        AddrToPage(addr, pageid, offset);
        std::cout<<"逻辑地址分解完成:页号为"<<pageid<<",偏移地址为"<<offset<<std::endl;
        //file << "逻辑地址分解完成:页号为"<<pageid<<",偏移地址为"<<offset<<std::endl;
        VisitPageTable(file,pageid,offset,paddr);
    }

    //设置页表的基地址的值
    void setPageTableAddr(PageTable& input_addr){
        pageTableAddr = &input_addr;
    }

    //将地址转化为页号和偏移量
    void AddrToPage(int addr, int& pageID, int& offset){
        pageID = addr/1000;
        offset = addr%1000;
    }

    //通过页号和页内偏移访问页表
    void VisitPageTable(std::ofstream& file, int pageID, int pageOffset, int& realAddr){

        Page temp;
        pageTableAddr->GetPage(pageID,temp);//页号为索引搜索页表
        if(temp.isDwellIn == 1)//如果页表命中,可送出页框号，并与页内位移拼接成物理地址
        {
            realAddr = temp.BlockId * 1000 + pageOffset;
            cout<<"页表命中，物理地址为"<<realAddr<<endl;
            //file<<"页表命中，物理地址为"<<realAddr<<endl;
        }
        else
        {
            MissPage(pageID);//缺页异常
            cout<<"页表未命中！产生缺页中断！"<<endl;
            cout<<"替换成功！"<<endl;
            cout<<"重新运行中断指令！"<<endl;
            //file<<"页表未命中！产生缺页中断！"<<endl;
            //file<<"替换成功！"<<endl;
            //file<<"中断指令运行成功！"<<endl;
        }
    }

    //缺页中断
    //??????????????????????????????????????什么鬼
    //缺页中断：待实现
    void MissPage(int pageID){
        pageTableAddr->page[pageID].isDwellIn = 1;//调入内存
        pageTableAddr->page[pageID].BlockId = pageID*987%100;
    }
};
#endif
