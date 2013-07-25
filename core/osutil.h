#ifndef OSUTIL_H
#define OSUTIL_H

#if defined(__MINGW32__)
#define PLATFORM_WIN32
#elif defined(linux) || defined(__linux)
#define PLATFORM_LINUX
#endif

#endif
