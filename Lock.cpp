#include "Lock.h"

void Lock::acquire()
{
    while (held.test_and_set());
}

void Lock::release()
{
    held.clear();
}
