#ifndef PTRUTIL_H
#define PTRUTIL_H

#include <memory>

/**
 * @brief Smart pointer that can either own the object referenced or not
 */
template <class T>
class OptOwnPtr
{
public:
    /**
     * @brief Construct an instance with no reference
     */
    OptOwnPtr()
        : ptr(nullptr)
    {

    }

    /**
     * @brief Construct an instance owning the object referenced
     */
    static inline OptOwnPtr<T> move(T* t)
    {
        return OptOwnPtr<T>(t, true);
    }

    /**
     * @brief Construct an instance not owning the object referenced
     */
    static inline OptOwnPtr<T> ref(T& t)
    {
        return OptOwnPtr<T>(&t, false);
    }

    /**
     * @brief Get a pointer to the reference object
     */
    T* get() const
    {
        return ptr;
    }

    /**
     * @brief Check if the reference is non-null
     */
    operator bool() const
    {
        return ptr != nullptr;
    }

    T& operator*() const
    {
        return *ptr;
    }

    T* operator->() const
    {
        return ptr;
    }

private:
    OptOwnPtr(T* ptr, bool own) : ptr(ptr)
    {
        if (own) owner.reset(ptr);
    }
    T* ptr;
    std::shared_ptr<T> owner;
};

#endif // PTRUTIL_H
