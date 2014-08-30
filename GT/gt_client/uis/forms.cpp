#include "forms.h"

Forms * Forms::_instance = NULL;

Forms *Forms::instance()
{
    if(_instance == NULL)
        _instance = new Forms();
    return _instance;
}

Forms::Forms()
{
}
