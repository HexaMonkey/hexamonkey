#ifndef SHOWCASE_H
#define SHOWCASE_H

#include <list>
#include <string>

/*!
 * \brief The Showcase class
 */
class Showcase
{
public:
    typedef std::list<std::string> container;
    typedef container::iterator iterator;
    typedef container::const_iterator const_iterator;

    Showcase();

    void add(const std::string& elem);

    const_iterator begin() const;
    const_iterator end() const;

    bool empty() const;
    int size() const;
private:
    container elems;

};

#endif // SHOWCASE_H
