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

    _richia_write("gotoyx %d %d", row, col);
}

/* update the given physical line to look like the corresponding line in
   curscr */

void PDC_transform_line(int lineno, int x, int len, const chtype *srcp)
{
    int i, j;

    PDC_LOG(("PDC_transform_line() - called: lineno=%d\n", lineno));

    char* out = malloc(len * 8 + 1); // every char takes 4 bytes, 8 hex chars
    for (i = 0; i < len; i++)
    {
        // 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 ... 1
        // bg bg bg bg fg fg fg fg attr...............attr char
        chtype ch = srcp[i];
        for (j = 0; j < 8; j++)
        {
            int val = (ch >> ((7 - j) * 4)) & 0x0f;
            if (val < 10)
                out[i * 8 + j] = '0' + val;
            else
                out[i * 8 + j] = 'a' + val - 10;
        }
    }
    out[len * 8] = '\0';
    _richia_write("update_line %d %d %s", lineno, x, out);
    free(out);
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
