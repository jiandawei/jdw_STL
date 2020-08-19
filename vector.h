/*
 * 本文件内容为：
 * vector
*/
#ifndef VECTOR_H
#define VECTOR_H
#include"alloc.h"
#include"construct.h"
#include"traits.h"
using namespace  mystl;
namespace mystl {

template <class T , class Alloc = alloc>
class vector
{
public:
    typedef T value_type;
    typedef T* pointer;
    typedef T* iterator;
    typedef T& reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

protected:
    typedef simple_alloc<value_type , Alloc> data_allocator;
    iterator start;//目前使用空间的头
    iterator finish;//目前使用空间的尾
    iterator end_of_storage;//目前可用空间的尾

    void insert_aux(iterator position , const T& x)
    {
        //还有备用空间
        if(finish != end_of_storage)
        {
            construct(finish , *(finish - 1));
            ++finish;
            T x_copy = x;
            copy_backward(position , finish - 2 , finish - 1);
            *position = x_copy;
        }
        else//备用空间已用光
        {
            const size_type old_size =size();
            const size_type len = old_size != 0 ? 2 * old_size : 1;//如果原来大小不为0，即为原来的两倍

            //配置空间
            iterator new_start = data_allocator::allocate(len);
            iterator new_finish = new_start;


            try {
                //将原来的内容搬过来
                new_finish = unintialized_copy(start , position , new_start);
                construct(new_finish , x);
                ++new_finish;
                new_finish = unintialized_copy(position , finish , new_finish);
            } catch (...) {
                //如果配置空间失败,释放已配置空间
                //destory(new_start , new_finish);
                data_allocator::deallocate(new_start , len);
                throw;
            }

            //析构原来的vector
            //destory(begin() , end());
            deallocate();

            //调整迭代器指向新的vector
            start = new_start;
            finish = new_finish;
            end_of_storage = new_start + len;
        }
    }

    void deallocate()
    {
        if(start)
            data_allocator::deallocate(start , end_of_storage - start);
    }

    iterator allocate_and_fill(size_type n , const T&x){
        iterator result = data_allocator::allocate(n);
        unintialized_fill_n(result , n ,x);
        return result;
    }

    void fill_initialize(size_type n , const T& value)
    {
        start = allocate_and_fill(n , value);
        finish = start + n;
        end_of_storage = finish;
    }

public:
    iterator begin() {return start;}
    iterator end() {return finish;}
    size_type size()  {return size_type(end() - begin());}
    size_type capacity() {return size_type(end_of_storage - begin());}
    bool empty() {return begin() == end();}
    reference operator [](size_type n){return *(begin() + n);}

    //构造
    vector():start(0),finish(0),end_of_storage(0){}

    vector(size_type n , const T& value){fill_initialize(n,value);}
    vector(int n , const T& value){fill_initialize(n,value);}
    vector(long n , const T& value){fill_initialize(n,value);}
    //显式声明，不能进行自动地隐式类型转换
    explicit vector(size_type n ){fill_initialize(n , T());}

    //析构
    ~vector()
    {
        //destory(start , finish);
        deallocate();
    }

    //第一个元素
    reference front(){return *begin();}
   //最后一个元素
    reference back(){return *(end() - 1);}
    //将元素插入尾端
    void push_back(const T& x)
    {
        //如果还有空间
        if(finish != end_of_storage)
        {
            construct(finish , x);
            ++finish;
        }
        //没有空间了，调用insert_aux
        else
        {
            insert_aux(end() , x);
        }
    }
    //删除最后一个元素
    void pop_back()
    {
        --finish;
        destory(finish);
    }

    iterator erase(iterator first , iterator last) {
        iterator i = copy(last , finish , first);
       // destory(i , finish);
        finish = finish - (last - first);
        return first;
    }

    //删除position上的元素
    iterator erase(iterator position)
    {
        if(position + 1 != end())
                copy(position+1 , finish , position);//position后面的元素前移
        --finish;
        destory(finish);
        return position;
    }

    //在position处插入n个元素x
    void insert(iterator position , size_type n ,const T& x)
    {
        if(n==0)
            return ;

        //备用空间还够用
        if(end_of_storage - finish  >= n)
        {
            T x_copy = x;
            //插入点后面有多少个元素
            const size_type elems_after = finish - position;
            iterator old_finish = finish;

            if(elems_after > n)//插入点后的个数大于新增元素个数
            {
                //因为要顺便初始化，所以要用unintialized
                unintialized_copy(finish - n , finish , finish);
                finish += n;
                copy_backward(position , old_finish - n , old_finish);
                fill(position , position + n , x_copy);
            }
            else//插入点后的个数小于新增元素个数
            {
                unintialized_fill_n(finish , n - elems_after , x_copy);
                finish  += n- elems_after;
                unintialized_copy(position , old_finish , finish);
                finish += elems_after;
                fill(position , old_finish , x_copy);
            }
        }
        else//备用空间不够用了
        {
            const size_type old_size = size();
            const size_type len = old_size + max(old_size , n);

            //以下配置新vector的空间
            iterator new_start = data_allocator::allocate(len);
            iterator new_finish = new_start;
            try {
                new_finish = unintialized_copy(start , position ,new_start);
                new_finish = unintialized_fill_n(new_finish, n , x);
                new_finish = unintialized_copy(position , finish , new_finish);
            } catch (...) {
                //destory(new_start , new_finish);
                data_allocator::deallocate(new_start , len);
                throw;
            }
            //destory(begin() ,end());
            deallocate();

            start = new_start;
            finish = new_finish;
            end_of_storage = start + len;
        }
    }
   //重新分配vector的大小
    void resize(size_type new_size , const T& x)
    {
        if(new_size<size())
            erase(begin()+new_size,end());
        else
            insert(finish , new_size - size() , x);
    }

    void resize(size_type new_size){resize(new_size,T());}

    //清除vector
    void clear(){erase(begin() , end());}

};
}//namespace mystl
#endif // VECTOR_H
