#include "iobservable.h"

IObservable::IObservable():QObject()
{
}

void IObservable::notifyObservers()
{
    foreach (IObserver* observer, _observers)
        observer->update(this);
}
