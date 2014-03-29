#ifndef __MINIMAL_H__
#define __MINIMAL_H__

#include <Eigen/Core>
#include <iostream>

// very nice piece of heavy meta-template-programming
//
// wanna have base-class defining and interface and common functions for all child-classes
//
// kudos to aschepler. see
//  https://stackoverflow.com/questions/22725867/eigen-template-parameter-in-specialization-of-pure-abstract-class/22726414#22726414
// 
// for the "this->" and using base-class functions see also
//  http://isocpp.org/wiki/faq/templates#nondependent-name-lookup-members

namespace is_eigen_dense_detail {
    typedef char yes_type;
    typedef int no_type;
    template <typename T>
    yes_type test(const Eigen::DenseBase<T>*);
    no_type test(...);
}
template <typename T>
struct is_eigen_matrix {
    static const bool value =
        (sizeof(is_eigen_dense_detail::test(static_cast<T*>(0))) ==
         sizeof(is_eigen_dense_detail::yes_type));
};

// std::enable_if is C++11. In C++03, either substitute boost::enable_if or just define and use your own:
template <bool DummCheck, typename T = void>
struct enable_if {};
template <typename T>
struct enable_if<true, T> {
    typedef T type;
// the "false" version is actually not implemented, so this can not be used during compile
// time and is ignored
};

// the interface and common functionality
template<typename T>
class MinimalBase
{
    protected:
        MinimalBase(const T& val)
            :val(val){};
    public:
        // the payload
        T val;
        void setZero() { val = static_cast<T>(0); }
        // some functions
        void increase() { val = val*100; }
        void decreace() { val = val/10; }
        // don't test self-assignment, causes hopefully no problem here
        // what about complex eigen-types like Eigen::Block<>?
        const T& operator=(const T& other) { this->val = other.val; };
        // pretty printing
        template<typename P>
        friend std::ostream& operator<<(std::ostream& os, const MinimalBase<P>& other);
};

// tooling
template<typename P>
std::ostream& operator<<(std::ostream& os, const MinimalBase<P>& base)
{
    os << "val: " << base.val;
    return os;
}

// the common child-template class, using the "Enable" trick
template <typename T, typename Enable = void>
class Minimal : public MinimalBase<T>
{
    public:
        Minimal(const T& val)
            :MinimalBase<T>(val) {}
        Minimal()
            :MinimalBase<T>(static_cast<T>(0)){}
};

// specific types are changed. like short
template<>
class Minimal<short> : public MinimalBase<short>
{
    public:
        Minimal(const short& val)
            :MinimalBase<short>(val) {}
        Minimal()
            :MinimalBase<short>(0){}
        void increase() { val = val*11; }
};

// and tadaah. this specialization only exists when T is or inherits an Eigen::DenseBase
// specialization.
template <typename T>
class Minimal<T, typename enable_if<is_eigen_matrix<T>::value>::type>
: public MinimalBase<T>
{
    public:
        EIGEN_MAKE_ALIGNED_OPERATOR_NEW
        Minimal(const T& val)
            :MinimalBase<T>(val) {}
        Minimal()
            :MinimalBase<T>(T::Zero()) {}
        void setZero() { MinimalBase<T>::val.setZero(); }
};

#endif
