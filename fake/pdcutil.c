/* PDCurses */

#include "pdcwin.h"

void PDC_beep(void)
{
    PDC_LOG(("PDC_beep() - called\n"));
}

void PDC_napms(int ms)
{
    // for remotes that want to control the execution speed (skip delays)
    PDC_LOG(("PDC_napms() - called: ms=%d\n", ms));
    Sleep(ms);
}

const char *PDC_sysname(void)
{
    return "Richia";
}
