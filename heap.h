
#ifndef HEAP_H
#define HEAP_H
namespace mystl {
#include <type_traits>
#include"traits.h"
#include<iostream>
using namespace std;
/*----------------------------------------push_heap()------------------------------------*/
template <class RandomAccessIterator>
inline void push_heap(RandomAccessIterator first , RandomAccessIterator last);
template <class RandomAccessIterator , class Distance , class T>
void __push_heap_aux(RandomAccessIterator first , RandomAccessIterator last , Distance* , T*);
template <class RandomAccessIterator , class Distance , class T>
void __push_heap(RandomAccessIterator first , Distance holeIndex , Distance topIndex ,  T value);

template <class RandomAccessIterator>
inline void push_heap(RandomAccessIterator first , RandomAccessIterator last) {
    __push_heap_aux(first , last , distance_type(first) , value_type(first));
}

template <class RandomAccessIterator , class Distance , class T>
void __push_heap_aux(RandomAccessIterator first , RandomAccessIterator last , Distance* , T*) {
    __push_heap(first , Distance((last - first) - 1) , Distance(0) , T(*(last - 1)));
}

template <class RandomAccessIterator , class Distance , class T>
void __push_heap(RandomAccessIterator first , Distance holeIndex , Distance topIndex ,  T value) {
    Distance parent = (holeIndex - 1) / 2;//找出新增节点的父节点,因为根节点是从0开始的，所以要减一
    while (holeIndex > topIndex && *(first + parent) < value) {//当未到达堆顶且父节点小于新增的值value
        *(first + holeIndex) = *(first + parent);//将父节点的值往下移，
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    *(first + holeIndex) = value;
}

/*-----------------------------------------pop_heap()----------------------------------------*/

template <class RandomAccessIterator>
inline void pop_heap(RandomAccessIterator first , RandomAccessIterator last);
template <class RandomAccessIterator , class T>
inline void __pop_heap_aux(RandomAccessIterator first , RandomAccessIterator last , T*);
template <class RandomAccessIterator , class T , class Distance>
inline void __pop_heap(RandomAccessIterator first , RandomAccessIterator last , RandomAccessIterator result , T value , Distance* );
template <class RandomAccessIterator , class T , class Distance>
void __adjust_heap(RandomAccessIterator first , Distance holeIndex , Distance len , T value);

template <class RandomAccessIterator>
inline void pop_heap(RandomAccessIterator first , RandomAccessIterator last) {
    __pop_heap_aux(first , last , value_type(first));
}

template <class RandomAccessIterator , class T>
inline void __pop_heap_aux(RandomAccessIterator first , RandomAccessIterator last , T*) {
    __pop_heap(first , last - 1 , last - 1 , T(*(last - 1 )) , distance_type(first));
}

template <class RandomAccessIterator , class T , class Distance>
inline void __pop_heap(RandomAccessIterator first , RandomAccessIterator last , RandomAccessIterator result , T value , Distance* ) {
    *result = *first;//将堆顶的值放到尾部，可以通过back()、pop_back()获得
    __adjust_heap(first , Distance(0) , Distance(last - first) , value);
}

template <class RandomAccessIterator , class T , class Distance>
void __adjust_heap(RandomAccessIterator first , Distance holeIndex , Distance len , T value) {
    Distance topIndex = holeIndex;
    Distance secondChild = 2 * holeIndex + 2;//堆顶的右孩子
    while(secondChild < len){//右孩子在堆的范围内
        if(*(first + secondChild) < *(first + (secondChild - 1))){//左孩子比右孩子大
            secondChild--;
        }
        *(first + holeIndex) = *(first + secondChild);
        holeIndex = secondChild;
        secondChild = 2 * (secondChild + 1);
        if(secondChild == len) {
            *(first + holeIndex) = *(first + (secondChild - 1));
            holeIndex = secondChild - 1;
        }
        __push_heap(first , holeIndex , topIndex , value);
    }
}


/*-------------------------------------sort_heap()------------------------------------------------------*/
template <class RandomAccessIterator>
void sort_heap(RandomAccessIterator first , RandomAccessIterator last) {//将迭代器所指范围按从小到大排序
    //cout<<"sort_heap:"<<*first<<" "<<*last<<endl;
    while(last - first > 1){//区间内还有两个或两个以上元素，继续调用pop_heap(),将大的值放到尾部
        pop_heap(first , last--);
    }
}


/*--------------------------------------make_heap()------------------------------------------------*/

template <class RandomAccessIterator>
inline void make_heap(RandomAccessIterator first , RandomAccessIterator last);
template <class RandomAccessIterator , class Distance , class T>
inline void __make_heap(RandomAccessIterator first , RandomAccessIterator last , T* , Distance*);

template <class RandomAccessIterator>
inline void make_heap(RandomAccessIterator first , RandomAccessIterator last) {
    __make_heap(first , last , value_type(first) , distance_type(first));
}

template <class RandomAccessIterator , class Distance , class T>
inline void __make_heap(RandomAccessIterator first , RandomAccessIterator last , T* , Distance*) {
    if(last - first < 2)//只有一个元素或者没有元素，直接返回
        return ;
    Distance len = last - first;
    Distance parent = (len - 2) / 2;//相当于从最后一个非叶子节点开始对每个非叶子节点进行sift_down算法
    while(true) {
        __adjust_heap(first , parent , len , T(*(first + parent)));
        if(parent == 0)return ;
        parent--;
    }
}
}
#endif // HEAP_H
