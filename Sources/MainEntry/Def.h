#ifndef DEF_H
#define DEF_H

#include <QObject>

#include "stdafx.h"

//枚举类导出  兼容QT5低版本
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
#define Q_BEGIN_ENUM_CREATE(CLASS) \
namespace CLASS                \
{                              \
        Q_NAMESPACE_EXPORT(ELA_EXPORT)

#define Q_END_ENUM_CREATE(CLASS) }

#define Q_ENUM_CREATE(CLASS) Q_ENUM_NS(CLASS)
#else
#define Q_BEGIN_ENUM_CREATE(CLASS)          \
class ELA_EXPORT CLASS : public QObject \
{                                       \
        Q_OBJECT                            \
    public:

#define Q_END_ENUM_CREATE(CLASS) \
             private:                         \
    Q_DISABLE_COPY(CLASS)        \
}                            \
    ;

#define Q_ENUM_CREATE(CLASS) Q_ENUM(CLASS)
#endif


// Q_BEGIN_ENUM_CREATE(AsulTheme)
// enum ThemeSyncSettings{
//     SyncColor=0x0000,
//     SyncThemeMode=0x0001,
// };

// Q_ENUM_CREATE(ThemeSyncSettings)
// Q_DECLARE_FLAGS(AsulTheme,ThemeSyncSettings)
// Q_END_ENUM_CREATE(AsulTheme)

#endif // DEF_H
