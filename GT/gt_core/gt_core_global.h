#ifndef GT_CORE_GLOBAL_H
#define GT_CORE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(GT_CORE_LIBRARY)
#  define GT_CORESHARED_EXPORT Q_DECL_EXPORT
#else
#  define GT_CORESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // GT_CORE_GLOBAL_H
