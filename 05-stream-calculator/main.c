#include <stdio.h>

#include "hashi.h"
#define STREAM_IMPLEMENTATION
#include "streams.h"

void func(u32 ap[static 1]) {
    (void) ap;
}

int main(void) {
    FILE *out = stdout;
    Alloc alloc = MAKE_ALLOC(0x100);
    const Stream *s0 = (const Stream *) make_ZeroStream(alloc);
    const Stream *s1 = (const Stream *) make_NumberStream(alloc, 10);

    debug_Streamln(out, s0);

    debug_Streamln(out, s1);

    {
        const Stream *add = add_Stream(alloc, s0, s0);

        fprintf(out, "add(zero, zero): ");
        debug_Streamln(out, add);
    }
    {
        const Stream *add = add_Stream(alloc, s0, s1);

        fprintf(out, "add(zero, [10]): ");
        debug_Streamln(out, add);
    }
    {
        const Stream *add = add_Stream(alloc, s1, s0);

        fprintf(out, "add([10], zero): ");
        debug_Streamln(out, add);
    }
    {
        const Stream *add = add_Stream(alloc, s1, s1);

        fprintf(out, "add([10], [10]): ");
        debug_Streamln(out, add);
    }

    // u32 a = 0;
    // func(&a);
    // func((u32 *) 1234);
    return 0;
}
