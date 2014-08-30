#ifndef UTILITIES_H
#define UTILITIES_H

#include <QPixmap>
#include <QElapsedTimer>

#include "math.h"
#include "gameglobals.h"
#include "gt_core_global.h"

GT_CORESHARED_EXPORT double getDistance(QPointF p1, QPointF p2);

enum ColorNumberType{MANA, LIVE};
enum ColorTeam {GREEN = 1, RED = 2};
enum Tank_Status {NORMAL, STUNNED, DEAD};

GT_CORESHARED_EXPORT QList<QPixmap> getListOfPixmapFromStripImage(QString image, int w);

GT_CORESHARED_EXPORT QList<QPixmap> getListOfPixmapFromTileSet(QPixmap tile_set, int w);

GT_CORESHARED_EXPORT QPixmap transformQPixmapToGrayScale(QPixmap pixmap);

GT_CORESHARED_EXPORT int getPart(int total, int percent);
GT_CORESHARED_EXPORT int getPercent(int total, int percent);

#endif // UTILITIES_H
