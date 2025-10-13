#include "AsulApplication.h"
#include <QTimer>
#include "../Global/GlobalSettings.h"
bool AsulApplication::event(QEvent *event){
    static bool isProcessing = false; // 防止重入
    if (event->type() == QEvent::ApplicationPaletteChange && gSets->getEnableThemeColorSyncWithSystem()) {
        if (!isProcessing) {
            isProcessing = true; // 标记为正在处理
            emit themeChanged();
            // 延迟重置标志位，避免短时间内重复触发
            QTimer::singleShot(100, this, [&](){ isProcessing = false; });
        }
    }
    return QApplication::event(event);
}
