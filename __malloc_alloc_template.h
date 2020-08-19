/*
 * 本文件内容为：
 * 一级配置器__malloc_alloc_template
*/

#ifndef __MALLOC_ALLOC_TEMPLATE_H
#define __MALLOC_ALLOC_TEMPLATE_H


#if 0
#include<new>
#define __THROW_BAD_ALLOC throw bad_alloc
#elif !defined (__THROW_BAD_ALLOC)
#include<iostream>
#define __THROW_BAD_ALLOC cerr<<"out of memory"<<endl;exit(1);
#endif

namespace mystl {
//无templ型别参数
template <int inst>
class __malloc_alloc_template
{

private:
    //一下函数用于处理内存不足的情况
    //oom : out of memory
    static void *oom_malloc(size_t n){

        //函数指针
        void (* my_malloc_handler)();
        void *result;

        //不断尝试释放内存、配置、再释放、再配置
        for(;;)
        {
            //为什么要多此一举，不直接调用__malloc_alloc_oom_handler
            //是因为__malloc_alloc_oom_handler可能不存在？
            my_malloc_handler=__malloc_alloc_oom_handler;
            if(my_malloc_handler==0){/*抛出异常*/}

            //企图释放内存
            (*my_malloc_handler)();

            //尝试配置内存
            result=malloc(n);

            //如果配置内存成功，则返回指向内存指针
            //否则继续执行函数
            if(result)return result;
        }
    }

    static void *oom_realloc(void *p,size_t n){

        //函数指针
        void (* my_malloc_handler)();
        void *result;

        //不断尝试释放内存、配置、再释放、再配置
        for(;;)
        {
            //为什么要多此一举，不直接调用__malloc_alloc_oom_handler
            //是因为__malloc_alloc_oom_handler可能不存在？
            my_malloc_handler=__malloc_alloc_oom_handler;
            if(my_malloc_handler==0){/*抛出异常*/}

            //企图释放内存
            (*my_malloc_handler)();

            //尝试配置内存
            result=realloc(p,n);

            //如果配置内存成功，则返回指向内存指针
            //否则继续执行函数
            if(result)return result;
        }
    }

    static void (*__malloc_alloc_oom_handler)();

public:

    static void *allocate(size_t n){
        //第一级配置器直接使用malloc()
        void *result=malloc(n);

        //如果无法通过malloc直接获得内存，改用oom_malloc
        if(result==0)result=oom_malloc(n);

        return result;
    }

    static void deallocate(void *p,size_t/*n*/){
        //第一级配置器调用free
        free(p);
    }

    static void* reallocate(void *p,size_t/*old_sz*/,size_t new_sz){
        //第一级配置器直接使用realloc()
         void *result=realloc(p,new_sz);

         //如果无法通过malloc直接获得内存，改用oom_malloc
         if(result==0)result=oom_realloc(p,new_sz);

         return result;
    }

    //用于设置out-of-memory handler
    static void (* set_malloc_handler(void (*f)()))(){
        void (*old)()=__malloc_alloc_oom_handler;
        __malloc_alloc_oom_handler=f;
        return (old);
    }

};//end of class __malloc_alloc_template

typedef __malloc_alloc_template<0> malloc_alloc;
typedef malloc_alloc alloc;

//static函数指针初始化
template <int inst>
void (*__malloc_alloc_template<inst>::__malloc_alloc_oom_handler)()=0;

}//namespace mystl
#endif // __MALLOC_ALLOC_TEMPLATE_H
