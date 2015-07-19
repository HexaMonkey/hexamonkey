#ifndef OSUTIL_H
#define OSUTIL_H

#if defined(__MINGW32__)
#define PLATFORM_WIN32

# define WIN32_LEAN_AND_MEAN
# include <windows.h>

#elif defined(linux) || defined(__linux)
#define PLATFORM_LINUX

#elif defined(__APPLE__)
#define PLATFORM_APPLE
#endif

#endif
