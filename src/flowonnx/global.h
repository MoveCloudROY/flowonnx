#ifndef FLOWONNXGLOBAL_H
#define FLOWONNXGLOBAL_H

#ifdef _MSC_VER
#  define FLOWONNX_DECL_EXPORT __declspec(dllexport)
#  define FLOWONNX_DECL_IMPORT __declspec(dllimport)
#else
#  define FLOWONNX_DECL_EXPORT __attribute__((visibility("default")))
#  define FLOWONNX_DECL_IMPORT __attribute__((visibility("default")))
#endif

#ifndef FLOWONNX_EXPORT
#  ifdef FLOWONNX_STATIC
#    define FLOWONNX_EXPORT
#  else
#    ifdef FLOWONNX_LIBRARY
#      define FLOWONNX_EXPORT FLOWONNX_DECL_EXPORT
#    else
#      define FLOWONNX_EXPORT FLOWONNX_DECL_IMPORT
#    endif
#  endif
#endif

#endif // FLOWONNXGLOBAL_H
