/*
 * 本文件内容为：
 * 二级配置器__default_alloc_template
*/

#ifndef __DEFAULT_ALLOC_TEMPLATE_H
#define __DEFAULT_ALLOC_TEMPLATE_H

#if 0
#include<new>
#define __THROW_BAD_ALLOC throw bad_alloc
#elif !defined (__THROW_BAD_ALLOC)
#include<iostream>
#define __THROW_BAD_ALLOC cerr<<"out of memory"<<endl;exit(1);
#endif

#include"__malloc_alloc_template.h"
namespace mystl {
/*>>>>>>>>>>>>>>>>>>>>>>第二级配置器<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

enum{__ALGIN=8};//小型区块的上调边界
enum{__MAX_BYTES=128};//小型区块的上限
enum{__NFREELISTS=__MAX_BYTES/__ALGIN};//free-lists的个数

//为简单实现，省略多线程部分
template <bool threads,int inst>
class __default_alloc_template
{
private:
    //将bytes上调至8的倍数
    static size_t ROUND_UP(size_t bytes){
        //先加7，然后与000 &减去比8的倍数多出的部分
        return (((bytes)+__ALGIN-1) & ~(__ALGIN-1));
    }

private:
    //查看union与struct的区别
    union obj{//free-lists节点的构造
        union obj* free_list_link;
        char client_data[1];
    };

private:
    //16个free-lists
    //原本有volatie关键字
    static obj* volatile free_list[__NFREELISTS];

    //根据数据块的大小，选择使用第n号free-list.n从0起算
    static size_t FREELIST_INDEX(size_t bytes){
        //(bytes+7)/8 - 1
        return ((bytes+__ALGIN-1)/__ALGIN - 1);
    }

    //返回一个大小为n的对象，并可能加入大小为n的其他区块到free list
    static void *refill(size_t n){
        int nobjs = 20;
        char* chunk = chunk_alloc(n , nobjs);
        obj* volatile *my_free_list;
        obj * result;
        obj* current_obj , *next_obj;
        int i;
        if(nobjs == 1)return (chunk);
        my_free_list = free_list + FREELIST_INDEX(n);
        result = (obj*)chunk;
        *my_free_list = next_obj = (obj*) (chunk + n);
        for(i =1 ; ; ++i) {
            current_obj = next_obj;
            next_obj = (obj*) ((char*)next_obj + n);
            if(nobjs - 1 == i) {
                current_obj->free_list_link = 0;
                break;
            }
            else{
                current_obj->free_list_link = next_obj;
            }
        }
        return result;
    }

    //配置一大块空间，可容纳nobjs个大小为size
    //如果配置nobjs个有所不便，就降低nobjs
    static char *chunk_alloc(size_t size,int &nobjs){
        char *result;
        size_t total_bytes=nobjs*size;
        //内存池所剩空间
        size_t bytes_left=end_free-start_free;

        //如果所剩空间能满足需求
        if(bytes_left>=total_bytes){
            result=start_free;
            start_free+=total_bytes;
            return result;
        }

        //如果所剩空间不能满足需求，但大于1个size
        else if(bytes_left>=size){
            //nobjs为引用传递，改变nobjs为实际返回区块的个数
            nobjs=bytes_left/size;
            total_bytes=nobjs*size;
            result=start_free;
            start_free+=total_bytes;
            return result;
        }

        //如果所剩空间连1个size都满足不了
        else{
            size_t bytes_to_get=2*total_bytes+ROUND_UP(heap_size>>4);

            //如果还有剩余空间
            if(bytes_left>0){
                //找到与bytes_left大小匹配的free-list
                obj **my_free_list=free_list+FREELIST_INDEX(bytes_left);

                //用头插法将剩余空间加入到该free-list中
                ((obj*)start_free)->free_list_link=*my_free_list;
                *my_free_list=(obj*)start_free;
            }

            //配置heap空间，用来补充内存池
            start_free=(char *)malloc(bytes_to_get);

            //如果malloc失败
            if(start_free==0){
                int i;
                obj **my_free_list,*p;

                //从区块大小等于size的free-list往后寻找适当的free-list
                //所谓适当的：就是还有未用的，但是区块比total_size还要大的free-lisrt
                for(i=size;i<=__MAX_BYTES;i+=__ALGIN){
                    my_free_list=free_list+FREELIST_INDEX(i);
                    p=*my_free_list;

                    //如果当前free-list还有区块未用
                    if(p!=0){
                        //将释放出未用的区块
                        //使start_free指向释放出的区块
                        *my_free_list=p->free_list_link;
                        start_free=(char *)p;
                        end_free=start_free+i;

                        //递归调用自己，为了修正nobjs
                        return (chunk_alloc(size,nobjs));
                    }//end if
                }//end for

                //此时，找不到可用的内存
                end_free=0;
                //调用第一级配置器
                start_free=(char *)malloc_alloc::allocate(bytes_to_get);
            }

            heap_size+=bytes_to_get;
            end_free=start_free+bytes_to_get;
            //递归调用自己，为了修正nobjs
            return (chunk_alloc(size,nobjs));
        }
    }

    //chunk allocation state
    //只在chunk_alloc()中变化
    static char *start_free;//内存池起始位置
    static char *end_free;//内存池结束位置
    static size_t heap_size;

public:
    static void *allocate(size_t n){

        obj **my_free_list;
        obj *result;

        //若申请空间大于128Byte就调用第一级配置器
        if(n>__MAX_BYTES)return (malloc_alloc::allocate(n));

        //寻找16个free-list中大小合适的一个
        my_free_list =free_list+FREELIST_INDEX(n);
        result=*my_free_list;

        //若没有可用的free-list,准备重新填充free list
        if(result==0){
            void *r=refill(ROUND_UP(n));
            return r;
        }

        //去my_free_list指向的链表的第一个节点指向的数据块，并且调整所在链表
        *my_free_list=result->free_list_link;
        return result;
    }

    static void deallocate(void *p,size_t n){
        obj *q=(obj *)p;
        obj **my_free_list;

        //大于128就交给第一级配置器
        if(n>(size_t)__MAX_BYTES){
            malloc_alloc::deallocate(p,n);
            return ;
        }

        my_free_list=free_list+FREELIST_INDEX(n);
        //找到适合的free-list后，用头插法将释放的空间放回free-list中
        q->free_list_link=*my_free_list;
        *my_free_list=q;
    }


};

//以下是static data member的定义与初始化

template <bool threads,int inst>
char *__default_alloc_template<threads , inst>::start_free=0;
template <bool threads,int inst>
char *__default_alloc_template<threads , inst>::end_free=0;
template <bool threads,int inst>
size_t __default_alloc_template<threads , inst>::heap_size=0;
template <bool threads,int inst>
typename __default_alloc_template<threads , inst>::obj * volatile
__default_alloc_template<threads , inst>::free_list[__NFREELISTS]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

}//namespace mystl
#endif // __DEFAULT_ALLOC_TEMPLATE_H
