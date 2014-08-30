#ifndef XAV_SOUND_ENGINE_GLOBAL_H
#define XAV_SOUND_ENGINE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(XAV_SOUND_ENGINE_LIBRARY)
#  define XAV_SOUND_ENGINESHARED_EXPORT Q_DECL_EXPORT
#else
#  define XAV_SOUND_ENGINESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // XAV_SOUND_ENGINE_GLOBAL_H
