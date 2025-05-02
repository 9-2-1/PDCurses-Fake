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
    short fore, back;

    PDC_LOG(("PDC_transform_line() - called: lineno=%d\n", lineno));

    char* out = malloc(len * 12 + 1); // every char takes 12 hex chars
    int outi = 0;
    for (i = 0; i < len; i++)
    {
        // 31 ... 24  23 ... 16 15 ... 1
        // color_pair attr      char
        chtype ch = srcp[i];
        PDC_pair_content(PAIR_NUMBER(ch), &fore, &back);
        // It need to look up another table to set color.
        // That's why we not support color setting.

        // fore (3 char is enough since PDC_MAXCOL == 768 < 4096)
        for (j = 0; j < 3; j++)
        {
            int val = (fore >> ((2 - j) * 4)) & 0x0f;
            if (val < 10)
                out[outi] = '0' + val;
            else
                out[outi] = 'a' + val - 10;
            outi++;
        }
        // back
        for (j = 0; j < 3; j++)
        {
            int val = (back >> ((2 - j) * 4)) & 0x0f;
            if (val < 10)
                out[outi] = '0' + val;
            else
                out[outi] = 'a' + val - 10;
            outi++;
        }
        // attr
        for (j = 0; j < 2; j++)
        {
            int val = ((ch >> 16) >> ((1 - j) * 4)) & 0x0f;
            if (val < 10)
                out[outi] = '0' + val;
            else
                out[outi] = 'a' + val - 10;
            outi++;
        }
        // char
        for (j = 0; j < 4; j++)
        {
            int val = (ch >> ((3 - j) * 4)) & 0x0f;
            if (val < 10)
                out[outi] = '0' + val;
            else
                out[outi] = 'a' + val - 10;
            outi++;
        }
    }
    out[outi] = '\0';
    _richia_write("update_line %d %d %s", lineno, x, out);
    free(out);
}

void PDC_blink_text(void)
{
    // Not Implemented
}
