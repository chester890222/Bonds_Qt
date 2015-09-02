#ifndef BASESQL_GLOBAL_H
#define BASESQL_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(BASESQL_LIBRARY)
#  define BASESQLSHARED_EXPORT Q_DECL_EXPORT
#else
#  define BASESQLSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // BASESQL_GLOBAL_H
