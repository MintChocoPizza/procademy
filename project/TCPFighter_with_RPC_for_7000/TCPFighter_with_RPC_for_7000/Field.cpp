
#include <Windows.h>
#include <unordered_map>
#include "Field.h"

C_Field C_Field::_C_Field;

C_Field* C_Field::GetInstance(void)
{
    return &_C_Field;
}

C_Field::C_Field()
{
}

C_Field::~C_Field()
{
}
