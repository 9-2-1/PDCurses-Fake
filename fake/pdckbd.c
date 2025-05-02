/* PDCurses */

#include "pdcwin.h"
#include "_richia.h"

// Ignored as keyboard is always binary.
void PDC_set_keyboard_binary(bool on)
{
}

/* check if a key or mouse event is waiting */

bool PDC_check_key(void)
{
    int ret = 0;
    char *reply = _richia_ask_and_wait("check_key");
    if (reply)
    {
        sscanf(reply, "%d", &ret);
        _richia_freemsg(reply);
    }
    return ret != 0;
}


/* return the next available key or mouse event */
// Note: mouse event is not supported yet.

int PDC_get_key(void)
{
    int ret;
    char *reply = _richia_ask_and_wait("get_key %d", SP->return_key_modifiers);
    SP->key_modifiers = 0;
    SP->key_code = FALSE;
    int is_keycode;
    if (reply)
    {
        sscanf(reply, "%d %d %lu", &ret, &is_keycode, &SP->key_modifiers);
        _richia_freemsg(reply);
    }
    if (is_keycode != 0)
        SP->key_code = TRUE;
    if (SP->key_code && ret == KEY_RESIZE)
        SP->resized = TRUE;
    return ret;
}

/* discard any pending keyboard or mouse input -- this is the core
   routine for flushinp() */

void PDC_flushinp(void)
{
    _richia_write("flushinp");
    fflush(stdout);
}

// Mouse is currently not supported
bool PDC_has_mouse(void)
{
    return FALSE;
}

int PDC_mouse_set(void)
{
    if (SP->_trap_mbe != 0)
    {
        return ERR;
    }
    else
    {
        return OK;
    }
}

int PDC_modifiers_set(void)
{
    return OK;
}
