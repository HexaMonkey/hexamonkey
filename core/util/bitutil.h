#ifndef MASKUTIL_H
#define MASKUTIL_H

#include <stdint.h>

/*!
 * \brief msb
 * \param byte
 * \return
 */
int msb(uint8_t byte);

/*!
 * \brief msbMask
 * \param min
 * \return
 */
uint8_t msbMask(int min);

/*!
 * \brief lsbMask
 * \param max
 * \return
 */
uint8_t lsbMask(int max);

#endif // MASKUTIL_H
