#include "page.h"


//页类
Page::Page(){
    PageId = -1;    //页号，从0开始编号
    BlockId = -1;   //归属的物理块号，从0开始
    isDwellIn = 0;  //表示是否导入到了内存， 1表示在内存，0表示不在
}

void Page::Init(int page_id, int is_dwell_in){
    PageId = page_id;
    isDwellIn = is_dwell_in;
}

//页表类
PageTable::PageTable(){
    page = new Page[25];        //页表内含有25个页
    length = 0;
}

void PageTable::InsertPage(Page target_page, int location){     //插入页
    int loc_point;
    if(!(location < 1 || location > length + 1)){
        length++;
        for(loc_point = length - 1; loc_point > location -1; loc_point--){
            page[loc_point + 1] = page[loc_point]
        }
        page[loc_point + 1] = target_page;
    }
}

void PageTable::GetPage(int page_id, Page &target_page){        //根据页号取出页
    int loc_point;
    for(loc_point = 1; loc_point < length + 1; loc_point++){
        if(page[loc_point].PageId == page_id){
            target_page = page[loc_point];
            return;
        }
    }
}
