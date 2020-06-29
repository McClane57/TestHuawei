#include "limits.h"

eRegDir Next(eRegDir current, int next, bool clockwise)
{
    return eRegDir(clockwise ? eRegDir((current + next) % 8): eRegDir((8 +current - next) % 8));
}
