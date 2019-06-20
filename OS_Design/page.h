#ifndef PAGE_H
#define PAGE_H
#include "hardware.h"
class Page;
class PageTable;

//页表项类
class Page{
public:
    int PageId;     //页号
    int BlockId;    //分配到的页框号
    int isDwellIn;  //是否调入到内存中

    Page();
    void Init(int page_id, int is_dwell_in);    //初始化函数
};

//页表类
class PageTable{
public:
    Page *page;     //包含的页表项
    int length;     //页表长度

    PageTable();
    void InsertPage(Page target_page, int location);  //插入页表项
    void DelPage(int page_id);     //根据页号删除页表项
    void GetPage(int page_id, Page &target_page);  //查找页表项,并返回查找到的页表项
};

#endif


