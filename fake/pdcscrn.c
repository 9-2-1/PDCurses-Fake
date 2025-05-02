/* PDCurses */

#include "pdcwin.h"
#include "_richia.h"

#include <stdlib.h>

static struct {short f, b;} atrtab[PDC_COLOR_PAIRS];

/* close the physical screen -- may restore the screen to its state
   before PDC_scr_open(); miscellaneous cleanup */

void PDC_scr_close(void)
{
    PDC_LOG(("PDC_scr_close() - called\n"));

    _richia_write("scr_close");
    _richia_close();
}

void PDC_scr_free(void)
{
    if (SP)
        free(SP);
}

/* open the physical screen -- miscellaneous initialization, may save
   the existing screen for later restoration */

int PDC_scr_open(int argc, char **argv)
{
    PDC_LOG(("PDC_scr_open() - called\n"));

    _richia_init();
    _richia_write("scr_open");

    SP = calloc(1, sizeof(SCREEN));
    if (!SP)
        return ERR;

    SP->lines = PDC_get_rows();
    SP->cols = PDC_get_columns();

    SP->mouse_wait = PDC_CLICK_PERIOD;
    SP->audible = TRUE;

    // SP->termattrs = A_COLOR | A_REVERSE | A_UNDERLINE | A_ITALIC | A_LEFT | A_RIGHT;
    SP->termattrs = A_COLOR | A_REVERSE;
    SP->orig_fore = 7;
    SP->orig_back = 0;
    SP->orig_attr = TRUE;
    SP->_restore = 0;
    SP->_preserve = 0;
    SP->mono = FALSE;

    return OK;
}


/* the core of resize_term() */

int PDC_resize_screen(int nlines, int ncols)
{
    _richia_write("resize_screen %d %d", nlines, ncols);
    PDC_flushinp();

    return OK;
}

void PDC_reset_prog_mode(void)
{
    PDC_LOG(("PDC_reset_prog_mode() - called.\n"));
    PDC_mouse_set();
}

void PDC_reset_shell_mode(void)
{
    PDC_LOG(("PDC_reset_shell_mode() - called.\n"));
}

void PDC_restore_screen_mode(int i)
{
}

void PDC_save_screen_mode(int i)
{
}

// no change color support
bool PDC_can_change_color(void)
{
    return FALSE;
}

void PDC_init_pair(short pair, short fg, short bg)
{
    atrtab[pair].f = fg;
    atrtab[pair].b = bg;
}

int PDC_pair_content(short pair, short *fg, short *bg)
{
    *fg = atrtab[pair].f;
    *bg = atrtab[pair].b;

    return OK;
}
int PDC_color_content(short color, short *red, short *green, short *blue)
{
    return ERR;
}

int PDC_init_color(short color, short red, short green, short blue)
{
    return ERR;
}
