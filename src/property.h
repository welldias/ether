#ifndef __ETHER_PROPERTY_H__
#define __ETHER_PROPERTY_H__

namespace ether {
    template <typename T>
    class Property {
    public:
        Property(T* v) :value(v) {}
        virtual T& operator= (const T& f) { *value = f; return *value; }
        virtual const T& operator() () const { return *value; }
        virtual explicit operator const T& () const { return *value; }
        virtual T* operator->() { return value; }
        //virtual operator T* () const { return v; }
    protected:
        T* value;
    };

    template <typename T>
    class ConstProperty {
    public:
        ConstProperty(T* v) :value(v) {}
        virtual const T& operator() () const { return *value; }
        virtual explicit operator const T& () const { return *value; }
        virtual T* operator->() { return value; }
    protected:
        T* value;
    };
}

#endif //__ETHER_PROPERTY_H__