/*
 * 本文件内容为：
 * 一级配置器__malloc_alloc_template 和 二级配置器__default_alloc_template 的
 * 共同接口:simple_alloc
*/

#ifndef ALLOC_H
#define ALLOC_H
/*
 * 这个文件包含了
 * 第一级配置器__malloc_alloc_template
 * 第二级配置器__default_alloc_template
 * 及两者的借口simple_alloc
*/


/*#ifdef __USE_MALLOC
typedef __malloc_alloc_template<0> malloc_alloc;
typedef malloc_alloc alloc;
#else
typedef __default_alloc_template<__NODE_ALLOCATOR_THREADS , 0> malloc_allc;
#endif
*/


#include"__malloc_alloc_template.h"
#include"__default_alloc_template.h"
#include"allocator.h"
namespace mystl {


//SGI STL的空间配置器:上面两个置配器的借口
template <class T , class Alloc>
class simple_alloc
{
public:
    static T* allocate(size_t n){return n==0?0:(T*)Alloc::allocate(n * sizeof(T));}
    static T* allocate(){return (T*)Alloc::allocate(sizeof(T));}
    static void deallocate(T* p , size_t n){if(n != 0)Alloc::deallocate(p , n*sizeof (T));}
    static void deallocate(T* p){Alloc::deallocate(p,sizeof (T));}
};
}//namespace mystl

#endif // ALLOC_H
