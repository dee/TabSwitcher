#ifndef TABSWITCH_GLOBAL_H
#define TABSWITCH_GLOBAL_H

#include <QtGlobal>

#if defined(TABSWITCH_LIBRARY)
#  define TABSWITCHSHARED_EXPORT Q_DECL_EXPORT
#else
#  define TABSWITCHSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // TABSWITCH_GLOBAL_H
