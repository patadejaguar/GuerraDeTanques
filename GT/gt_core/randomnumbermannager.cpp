#include "randomnumbermannager.h"

QList<int> RandomNumberMannager::_shema = QList<int>();
int RandomNumberMannager::_index = -1;

int RandomNumberMannager::getRandomNumber()
{
    int value = _shema.at(_index);
    nextIndex();
    return value;
}

int RandomNumberMannager::getMaxRandomNumber(int max)
{
    int value = _shema.at(_index)%(max+1);
    nextIndex();
    return value;
}

void RandomNumberMannager::setShema(QList<int> shema)
{
    _shema = shema;
    _index = 0;
}

void RandomNumberMannager::nextIndex()
{
    if(!_shema.isEmpty())
        _index = (_index + 1) % _shema.size();
}
