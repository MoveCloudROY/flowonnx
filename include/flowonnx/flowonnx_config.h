#ifndef DS_FLOWONNX_CONFIG_H
#define DS_FLOWONNX_CONFIG_H

#if defined _MSC_VER || defined __CYGWIN__
#  define FLOWONNX_DECL_EXPORT __declspec(dllexport)
#  define FLOWONNX_DECL_IMPORT __declspec(dllimport)
#else
#  define FLOWONNX_DECL_EXPORT __attribute__((visibility("default")))
#  define FLOWONNX_DECL_IMPORT __attribute__((visibility("default")))
#endif

#ifdef FLOWONNX_STATIC
#  define FLOWONNX_API
#else
#  ifdef FLOWONNX_LIBRARY
#    define FLOWONNX_API FLOWONNX_DECL_EXPORT
#  else
#    define FLOWONNX_API FLOWONNX_DECL_IMPORT
#  endif
#endif

#if defined(__GNUC__)
#  define FLOWONNX_DEPRECATED __attribute__((deprecated))
#elif defined(_MSC_VER)
#  define FLOWONNX_DEPRECATED __declspec(deprecated)
#else
#  define FLOWONNX_DEPRECATED
#endif

#endif // DS_FLOWONNX_CONFIG_H
