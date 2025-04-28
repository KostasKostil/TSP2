#include "common.h"

double Time()
{
    return sfclock.getElapsedTime().asSeconds();
}
