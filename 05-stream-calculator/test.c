#include <stdio.h>

#define STREAM_IMPLEMENTATION
#include "streams.h"

#define ARRAY_SIZE(arr) ((sizeof(arr))/sizeof((arr)[0]))
#define TEST(x) if (!(x))
#define ON_FIRST_ERR(block) do { \
        if (!first_err) { \
            block; \
            first_err += 1; \
        } \
    } while (0)
#define RUN_TEST(test, ctx, errs) {\
        int _local_errs = (test)(ctx); \
        if (_local_errs > 0) { \
            fprintf(ctx.out, "Found %d errors in " #test "!\n", _local_errs); \
        } \
        errs += _local_errs; \
    }

static int first_err = 0;

typedef struct {
    Alloc alloc;
    FILE *out;
} TestCtx;

int test_zero(TestCtx ctx) {
    int errs = 0;
    const ZeroStream *z0 = make_ZeroStream(ctx.alloc);
    const ZeroStream *z1 = make_ZeroStream(ctx.alloc);
    _zerostream_ok(z0);
    _zerostream_ok(z1);

    {
        TEST (z0 == z1) {
            errs += 1;
            ON_FIRST_ERR({
                fprintf(ctx.out,
                    "Two ZeroStreams should be equal\n");
            });
        }
    }

    {
        const fword head = head_Stream(ctx.alloc, (const Stream *) z0);
        TEST (head == 0.0) {
            errs += 1;
            ON_FIRST_ERR({
                fprintf(ctx.out,
                    "Head of (Zero) should be 0.0, but is %"PRIfPTR"\n",
                    head);
            });
        }
    }

    {
        const Stream *tail = tail_Stream(ctx.alloc, (const Stream *) z0);
        TEST ((const ZeroStream *) tail == z0) {
            errs += 1;
            ON_FIRST_ERR({
                fprintf(ctx.out,
                    "Tail of (Zero) should be itself, but is ");
                debug_Streamln(ctx.out, tail);
            });
        }
    }

    return errs;
}

int test_number(TestCtx ctx) {
    int errs = 0;

    NumberStream ns[] = {
        [0] = local_NumberStream(12.3),
        [1] = local_NumberStream(4.56),
    };
    for (size_t i = 0; i < ARRAY_SIZE(ns); i += 1) {
        _local_numberstream_ok(ns[i]);
    }

    NumberStream adds[ARRAY_SIZE(ns)*ARRAY_SIZE(ns)] = {0};
    for (size_t i = 0; i < ARRAY_SIZE(adds); i += 1) {
        const size_t ai = i / ARRAY_SIZE(ns);
        const size_t bi = i % ARRAY_SIZE(ns);
        adds[i] = add_NumberStream(ns[ai], ns[bi]);
        _local_numberstream_ok(adds[i]);
    }

    {
        const Stream *tail = tail_Stream(ctx.alloc, (const Stream *) &ns[0]);
        TEST (tail == (const Stream *) &THE_ZERO_STREAM) {
            errs += 1;
            ON_FIRST_ERR({
                fprintf(ctx.out, "Tail of ");
                debug_Stream(ctx.out, (const Stream *) &ns[0]);
                fprintf(ctx.out, " should be ");
                debug_Stream(ctx.out, (const Stream *) &THE_ZERO_STREAM);
                fprintf(ctx.out, ", but is ");
                debug_Streamln(ctx.out, tail);
            });
        }
    }

    for (size_t i = 0; i < ARRAY_SIZE(adds); i += 1) {
        const size_t ai = i / ARRAY_SIZE(ns);
        const size_t bi = i % ARRAY_SIZE(ns);
        const fword expected_head =
            local_head_NumberStream(ns[ai]) + local_head_NumberStream(ns[bi]);
        const fword head = local_head_NumberStream(adds[i]);
        TEST (head == expected_head) {
            errs += 1;
            ON_FIRST_ERR({
                fprintf(ctx.out, "Adding ");
                debug_Stream(ctx.out, (const Stream *) &ns[ai]);
                fprintf(ctx.out, " with ");
                debug_Stream(ctx.out, (const Stream *) &ns[bi]);
                fprintf(ctx.out, " results in ");
                debug_Stream(ctx.out, (const Stream *) &adds[i]);
                fprintf(ctx.out, ", but it's head is %"PRIfPTR" (expected is %"PRIfPTR")"
                    " (i=%zu, limit=%zu)\n",
                    head, expected_head, i, ARRAY_SIZE(adds));
            });
        }
    }
    return errs;
}

int test_all(TestCtx ctx) {
    int errs = 0;
    RUN_TEST(test_zero, ctx, errs);
    RUN_TEST(test_number, ctx, errs);
    return errs;
}

int main(void) {
    TestCtx ctx = (TestCtx){
        .alloc = MAKE_ALLOC(0x400),
        .out = stderr,
    };
    int errs = test_all(ctx);
    if (errs) {
        fprintf(ctx.out,
            "Tests failed. Found %d errors!\n", errs);
    } else {
        fprintf(ctx.out,
            "Ok!\n");
    }
    return errs;
}