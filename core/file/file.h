
#ifndef FILE_H
#define FILE_H


#include <iostream>
#include <fstream>
#include <stdint.h>
#include <map>

/** @brief High-level input stream operations on files with bit precision

The class is implemented as an adaptor for a std::ifstream instance that
reimplements common operation with bit precision instead of byte precision*/
class File
{
public:
    File();

    /** @brief Sets the path to the file*/
    virtual void setPath(const std::string& path) = 0;

    /** @brief Returns the path to the file*/
    virtual const std::string& path() const = 0;

    /** @brief Opens stream */
    virtual void open() = 0;

    /** @brief Closes stream*/
    virtual void close() = 0;

    /** @brief Clears the file error flags*/
    virtual void clear() = 0;


    /** @brief Extracts bits from stream

    Puts the result in a byte array already allocated
    the result is right aligned and zero padded*/
    virtual void read(char* s, int64_t size) = 0;

    /** @brief Offsets the position

     * \param off Offset to apply in bits.
     * \param dir Where to start from to apply the offset.
     * begin (std::ios_base::beg), current (std::ios_base::cur) or
     * end (std::ios_base::end).
     */
    virtual void seekg(int64_t off, std::ios_base::seekdir dir) = 0;

    /** @brief Returns the current stream position */
    virtual int64_t tellg() = 0;

    /** @brief Returns the size of the file*/
    virtual int64_t size() = 0;

    /** @brief Checks if data can be recovered from the stream*/
    virtual bool good() = 0;

protected:
    char _bitPosition;

private:
    File& operator=(const File&) = delete;
    File(const File&) = delete;
};

/**
 * @brief RAII object that insure that the \link File file\endlink returns to its
 * original position when the ressource is liberated
 */
class FileAnchor
{
public:
    FileAnchor(File& file);
    ~FileAnchor();
private:
    File& file;
    int64_t position;
};

#endif // FILE_H
