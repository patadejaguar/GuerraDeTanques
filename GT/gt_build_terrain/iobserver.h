#ifndef IOBSERVER_H
#define IOBSERVER_H

class IObservable;

class IObserver
{
public:
    IObserver();
    virtual void update(IObservable *observable) = 0;
};

#endif // IOBSERVER_H
