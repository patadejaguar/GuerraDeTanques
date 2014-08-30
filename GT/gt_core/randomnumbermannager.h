#ifndef RANDOMNUMBERMANNAGER_H
#define RANDOMNUMBERMANNAGER_H

#include <QObject>
#include <QList>

#include "gt_core_global.h"

class FormGame;
class FormCreatedGame;
class FormResults;
class FormReplayView;

class GT_CORESHARED_EXPORT RandomNumberMannager : public QObject
{
        Q_OBJECT

        friend class                    FormGame;
        friend class                    FormCreatedGame;
        friend class                    FormResults;
        friend class                    FormReplayView;

    public:

        static int                      getRandomNumber();

        static int                      getMaxRandomNumber(int max);

    private:

        static void                     nextIndex();

        static void                     setShema(QList<int> shema);

        static QList<int>               _shema;

        static int                      _index;


};

#endif // RANDOMNUMBERMANNAGER_H
