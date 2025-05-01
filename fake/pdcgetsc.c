/* PDCurses */

#include "pdcwin.h"
#include "_richia.h"

/* get the cursor size/shape */

int PDC_get_cursor_mode(void)
{
    int ret = 0;
    char *reply = _richia_ask_and_wait("get_cursor_mode");
    if (reply)
    {
        sscanf(reply, "%d", &ret);
        _richia_freemsg(reply);
    }
    return ret;
}

/* return number of screen rows */

int PDC_get_rows(void)
{
    int ret = 0;
    char *reply = _richia_ask_and_wait("get_rows");
    if (reply)
    {
        sscanf(reply, "%d", &ret);
        _richia_freemsg(reply);
    }
    return ret;
}

/* return width of screen/viewport */

int PDC_get_columns(void)
{
    int ret = 0;
    char *reply = _richia_ask_and_wait("get_columns");
    if (reply)
    {
        sscanf(reply, "%d", &ret);
        _richia_freemsg(reply);
    }
    return ret;
}
