#ifndef CONSTRUCT_H
#define CONSTRUCT_H
//本文件包括以下内容
/*五个函数
 * construct：负责对象的构造
 * destroy：负责对象的析构
 * 内存基本处理工具：
 * unintialized_fill
 * unintialized_fill_n
 * unintialized_copy
*/

#include"traits.h"
#include<string.h>
#include<new>//为了construct的new
namespace mystl {

//construct 构造对象
template <class T1,class T2>
inline void construct(T1* p,const T2& value){
    new (p) T1(value);//调用T1::T1(value)
}

//destory 析构对象

//版本1：接受一个指针
template <class T>
inline void destory(T* pointer){
    pointer->~T();//调用T的析构函数
}

//版本2：接受两个迭代器
template <class ForwardIterator>
inline void destroy(ForwardIterator first,ForwardIterator last){
    __destory(first,last,value_type(first));
}

//判断value type是否有trivial destructor
template <class ForwardIterator,class T>
inline void __destroy(ForwardIterator first,ForwardIterator last,T*){
    typedef typename __type_traits<T>::has_trivial_destructor trivial_destructor;
     __destroy_aux(first,last,trivial_destructor());
}

//如果有trivial destructor,
//就什么都不做
template <class ForwardIterator>
inline void __destroy_aux(ForwardIterator first , ForwardIterator last,__true_type){}

//如果有non-trivial destructor
//就依次对[first,last)区间内调用destory
template <class ForwardIterator>
inline void __destroy_aux(ForwardIterator first,ForwardIterator last,__false_type){
    for (;first!=last;++first) {
        destory(&*first);//调用destroy版本1：接受一个指针(&*first)
    }
}

//以下是destroy的版本2特化版本，将迭代器换成char*和wchar_t*
//但是因为两者都有trivial destroctor。所以函数什么都不干
inline void destroy(char*,char*){}
inline void destroy(wchar_t*,wchar_t*){}


//以下三个unintialized函数都是commit or rollback




//unintialized_copy_aux

//如果是POD型别
template <class InputIterator , class ForwardIterator>
ForwardIterator __unintialized_copy_aux(InputIterator first , InputIterator last , ForwardIterator result , __true_type)
{
    //交由STL算法copy执行
    copy(first , last , result);
}

//如果不是POD型别
template <class InputIterator , class ForwardIterator>
ForwardIterator __unintialized_copy_aux(InputIterator first , InputIterator last , ForwardIterator result , __false_type)
{
    ForwardIterator cur = result;
    for(; first != last ;++first , ++cur)
        construct(&*cur , *first  );//必须一个一个元素构造，无法批量进行

    return cur;
}



template <class InputIterator , class ForwardIterator , class T>
inline ForwardIterator __unintialized_copy(InputIterator first , InputIterator last , ForwardIterator result , T*)
{
    typedef typename __type_traits<T>::is_POD_type is_POD;
    return __unintialized_copy_aux(first , last , result , is_POD());
}


//unintialized_copy()使我们将内存的配置与对象的构造行为分离开来
template <class InputIterator , class ForwardIterator>
inline ForwardIterator unintialized_copy(InputIterator first , InputIterator last , ForwardIterator result)
{
    return __unintialized_copy(first , last , result , value_type(result));
}

//以下是针对char* 和 wchar*两种型别的 unintialized_copy特化版本

//char *
inline char* unintialized_copy(const char* first , const char * last , char* result)
{
    memmove(result , first ,last - first);
    return result + (last - first);
}

inline wchar_t* unintialized_copy(const wchar_t* first , const wchar_t * last , wchar_t* result)
{
    memmove(result , first ,last - first);
    return result + (last - first);
}


//unintialized_fill_aux

//是POD型别
template <class ForwardIterator , class T , class T1>
inline void __unintialized_fill_aux(ForwardIterator first , ForwardIterator last , const T& x , __true_type)
{
    fill(first , last , x);//调用STL算法fill
}

//不是POD型别
template <class ForwardIterator , class T , class T1>
inline void __unintialized_fill_aux(ForwardIterator first , ForwardIterator last , const T& x , __false_type)
{
    ForwardIterator cur = first;
    for ( ; cur != last ; ++cur)
    {
        construct(&*cur , x);
    }
}


template <class ForwardIterator , class T , class T1>
inline void __unintialized_fill(ForwardIterator first , ForwardIterator last , const T& x , T1*)
{
    typedef  typename __type_traits<T1>::is_POD_type is_POD;
    __unintialized_fill_aux(first , last , x , is_POD());
}


//unintialized_fill()使我们将内存的配置与对象的构造行为分离开来
template <class ForwardIterator , class T>
inline void unintialized_fill(ForwardIterator first , ForwardIterator last , const T& x)
{
    __unintialized_copy(first , last , x , value_type(first));
}




//unintialized_fill_n_aux

//如果是POD型别
template <class ForwardIterator , class Size , class T>
inline ForwardIterator __unintialized_fill_n_aux(ForwardIterator first , Size n , const T& x , __true_type)
{
    return fill_n(first , n ,x);//交由高阶函数执行
}


//如果不是POD型别
template <class ForwardIterator , class Size , class T>
inline ForwardIterator __unintialized_fill_n_aux(ForwardIterator first , Size n , const T& x , __false_type)
{
    ForwardIterator cur = first;
    for( ; n>0 ; --n , ++cur)
    {
        construct(&*cur , x);
    }
    return cur;
}


template <class ForwardIterator , class Size , class T , class T1>
inline ForwardIterator __unintialized_fill_n(ForwardIterator first , Size n , const T& x , T1*)
{
    typedef typename __type_traits<T1>::is_POD_type is_POD;
    return __unintialized_fill_n_aux(first , n ,x , is_POD());
}

//unintialized_fill_n()使我们将内存的配置与对象的构造行为分离开来
template <class ForwardIterator , class Size , class T >
inline ForwardIterator unintialized_fill_n(ForwardIterator first , Size n , const T& x )
{
    return __unintialized_fill_n(first , n , x ,value_type(first));
}

}//namespace mystl













#endif // CONSTRUCT_H
