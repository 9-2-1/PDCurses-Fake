/* PDCurses */

#include "pdcwin.h"
#include "_richia.h"

#include <stdlib.h>
#include <string.h>

#ifdef PDC_WIDE
# include "../common/acsuni.h"
#else
# include "../common/acs437.h"
#endif

/* position hardware cursor at (y, x) */

void PDC_gotoyx(int row, int col)
{
    PDC_LOG(("PDC_gotoyx() - called: row %d col %d from row %d col %d\n",
             row, col, SP->cursrow, SP->curscol));

    _richia_write("gotoyx %d, %d", row, col);
}

/* update the given physical line to look like the corresponding line in
   curscr */

void PDC_transform_line(int lineno, int x, int len, const chtype *srcp)
{
    int i;
    int dataid = rand();

    PDC_LOG(("PDC_transform_line() - called: lineno=%d\n", lineno));

    _richia_write("update_line %d, %d len %d dataid %d", lineno, x, len, dataid);

    for (i = 0; i < len; i++)
    {
        // 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 ... 1
        // bg bg bg bg fg fg fg fg attr...............attr char
        int bg = (srcp[i] >> 28);
        int fg = (srcp[i] >> 24) & 0xf;
        int attr = (srcp[i] >> 16) & 0xff;
        int chr = srcp[i] & 0xffff;
        _richia_write(" data %d bg %d fg %d attr %d chr %d", dataid, bg, fg, attr, chr);
    }
}

void PDC_blink_text(void)
{
    // Not Implemented
}

void PDC_doupdate(void)
{
    _richia_write("doupdate");
    _richia_flush();
}
