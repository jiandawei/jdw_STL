#ifndef FUNCTIONAL_H
#define FUNCTIONAL_H
namespace mystl {
using namespace std;
//一元函数
template <class Arg , class Result>
struct unary_function {
    typedef  Arg argument_type;
    typedef Result result_type;
};

//二元函数
template <class Arg1 , class Arg2 , class Result>
struct binary_function {
    typedef  Arg1 first_argument_type;
    typedef Arg2 second_argument_type;
    typedef Result result_type;
};


/*----------------------------算术类-------------------------------------------*/
//加
template <class T>
struct plus : public binary_function<T , T , T> {
    T operator () (const T& x , const T& y) const {return x + y;}
};

//减
template <class T>
struct minus : public binary_function<T , T , T> {
    T operator () (const T& x , const T& y) const {return x - y;}
};

//乘
template <class T>
struct multiplies : public binary_function<T , T , T> {
    T operator () (const T& x , const T& y) const {return x * y;}
};

//除
template <class T>
struct divides : public binary_function<T , T , T> {
    T operator () (const T& x , const T& y) const {return x / y;}
};

//取模
template <class T>
struct modulus : public binary_function<T , T , T> {
    T operator () (const T& x , const T& y) const {return x % y;}
};

//否定
template <class T>
struct negate : public unary_function<T , T> {
    T operator () (const T& x) const {return -x;}
};



/*------------------------------------关系运算类--------------------------------------*/
//相等
template <class T>
struct equal_to : public binary_function<T , T , bool> {
    bool operator () (const T& x , const T& y) const {return x == y;}
};

//不等
template <class T>
struct not_equal_to : public binary_function<T , T , bool> {
    bool operator () (const T& x , const T& y) const {return x != y;}
};

//大于
template <class T>
struct greater : public binary_function<T , T , bool> {
    bool operator () (const T& x , const T& y) const {return x > y;}
};

//大于
template <class T>
struct less : public binary_function<T , T , bool> {
    bool operator () (const T& x , const T& y) const {return x < y;}
};

//大于或等于
template <class T>
struct greater_equal : public binary_function<T , T , bool> {
    bool operator () (const T& x , const T& y) const {return x >= y;}
};

//小于或等于
template <class T>
struct less_equal : public binary_function<T , T , bool> {
    bool operator () (const T& x , const T& y) const {return x <= y;}
};


/*------------------------------------逻辑运算类--------------------------------------*/
//与
template <class T>
struct logical_and : public binary_function<T , T , bool> {
    bool operator () (const T& x , const T& y) const {return x && y;}
};

//或
template <class T>
struct logical_or : public binary_function<T , T , bool> {
    bool operator () (const T& x , const T& y)const {return x || y;}
};

//非
template <class T>
struct logical_not : public unary_function<T  , bool> {
    bool operator () (const T& x ) const{return !x;}
};


//证同元素
template <class T>
inline T indentity_element(plus<T>)//任何数加0等于其自身
{return T(0);}

template <class T>
inline T indentity_element(multiplies<T>)//任何数乘1等于其自身
{return T(1);}

//证同函数
template <class T>
struct identity : public unary_function<T , T> {
    const T& operator () (const T& x) const {return x;}
};

//选择函数传回pair的第一个元素
template <class pair>
struct select1st : public unary_function<pair , typename pair::first_type> {
    const typename pair::first_type& operator () (const pair& x) const {
        return x.first;
    }
};

//选择函数传回pair的第二个元素
template <class pair>
struct select2st : public unary_function<pair , typename pair::second_type> {
    const typename pair::second_type& operator () (const pair& x) const {
        return x.second;
    }
};
}//namespace mystl
#endif // FUNCTIONAL_H
