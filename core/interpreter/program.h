#ifndef EBMLOBJECT_H
#define EBMLOBJECT_H

#include "object.h"

class Program
{
public:
    template<class It>
    class _const_iterator : public std::iterator<std::input_iterator_tag, Program>
    {
        friend class Program;
        It _it;

        _const_iterator<It>(const It& it):_it(it){}
        public:
            _const_iterator<It>(){}
            _const_iterator<It>& operator++() {++_it; return *this;}
            _const_iterator<It> operator++(int) {const_iterator dup(*this); ++_it; return dup;}
            _const_iterator<It>& operator--() {--_it; return *this;}
            _const_iterator<It> operator--(int) {const_iterator dup(*this); --_it; return dup;}
            Program operator*() const {return Program(**_it);}
            bool operator==(const _const_iterator<It>& other) const {return _it==other._it;}
            bool operator!=(const _const_iterator<It>& other) const {return !(*this==other);}
    };

    typedef _const_iterator< Object::iterator > const_iterator;
    typedef _const_iterator< Object::reverse_iterator > const_reverse_iterator;

    Program();
    Program(Object& object);

    uint32_t id() const;
    const Variant& payload() const;
    int size() const;
    Program elem(int index) const;

    const_iterator begin() const;
    const_iterator end() const;
    const_reverse_iterator rbegin() const;
    const_reverse_iterator rend() const;

private:
    Object* _object;
};

#endif // EBMLOBJECT_H
