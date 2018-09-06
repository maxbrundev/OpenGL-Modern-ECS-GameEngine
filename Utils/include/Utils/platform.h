#pragma once

#if defined linux || defined __linux__ || defined __linux
#  define PLATFORM_LINUX
#  define PLATFORM_NAME "Linux"
#endif

// Plateforme Windows
#if defined _WIN32 || defined WIN32 || defined __NT__ || defined __WIN32__
#  define PLATFORM_WIN32
#  define PLATFORM_NAME "Windows"
#endif

// Plateforme MacOS X
#if ( defined __MWERKS__ && defined __powerc && !defined macintosh ) || defined __APPLE_CC__ || defined macosx
#  define PLATFORM_MACOSX
#  define PLATFORM_NAME "MacOS X"
#endif