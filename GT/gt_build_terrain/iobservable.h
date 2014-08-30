#ifndef IOBSERVABLE_H
#define IOBSERVABLE_H

#include <QObject>
#include "iobserver.h"
#include <QList>

class IObservable: public QObject
{
public:
    IObservable();
    inline void addObserver(IObserver *observer){_observers.append(observer);}
    inline void removeObserver(IObserver *observer){_observers.removeAll(observer);}
    inline void removeAllObservers(){_observers.clear();}
    void notifyObservers();

private:
    QList<IObserver*> _observers;
};

#endif // IOBSERVABLE_H
