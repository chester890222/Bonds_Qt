#ifndef BASEMARKET_GLOBAL_H
#define BASEMARKET_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(BASEMARKET_LIBRARY)
#  define BASEMARKETSHARED_EXPORT Q_DECL_EXPORT
#else
#  define BASEMARKETSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // BASEMARKET_GLOBAL_H
