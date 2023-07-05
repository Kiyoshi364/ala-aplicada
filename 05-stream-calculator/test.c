#include <stdio.h>

#define STREAM_IMPLEMENTATION
#include "streams.h"

#define EPSILON 1e-16
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
        if (_local_errs != 0) { \
            fprintf(ctx.out, "Found %d errors in " #test "!\n", _local_errs); \
        } else { \
            fprintf(ctx.out, "Test ok (" #test ")\n"); \
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
        const fword head = head_Stream((const Stream *) z0);
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

int test_lazyadd(TestCtx ctx) {
    int errs = 0;
    const size_t depths[] = {
        3, 3, 3, 3
    };
    const Stream *streams[][3] = {
        {
            (const Stream *) make_NumberStream(ctx.alloc, 10.0),
            (const Stream *) make_NumberStream(ctx.alloc, 15.0),
            (const Stream *) make_NumberStream(ctx.alloc, 25.0),
        }, {
            (const Stream *) make_NumberStream(ctx.alloc, 10.0),
            (const Stream *) make_ZeroStream(ctx.alloc),
            (const Stream *) make_NumberStream(ctx.alloc, 10.0),
        }, {
            (const Stream *) make_LazyAddStream(ctx.alloc,
                (const Stream *) make_NumberStream(ctx.alloc, 10.0),
                (const Stream *) make_NumberStream(ctx.alloc, 25.0)
            ),
            (const Stream *) make_ZeroStream(ctx.alloc),
            (const Stream *) make_NumberStream(ctx.alloc, 35.0),
        }, {
            (const Stream *) make_LazyMulStream(ctx.alloc,
                (const Stream *) make_NumberStream(ctx.alloc, 10.0),
                (const Stream *) make_NumberStream(ctx.alloc, 5.0)
            ),
            (const Stream *) make_ZeroStream(ctx.alloc),
            (const Stream *) make_NumberStream(ctx.alloc, 50.0),
        }
    };
    CASSERT(ARRAY_SIZE(depths) == ARRAY_SIZE(streams), DEPTHS_SIZE_EQ_STREAM_SIZE);

    for (size_t i = 0; i < ARRAY_SIZE(streams); i += 1) {
        int this_errs = 0;
        const Stream *a = streams[i][0];
        const Stream *b = streams[i][1];
        const Stream *const expected = streams[i][2];
        const Stream *const result = add_Stream(ctx.alloc, a, b);

        if (depths[i] > 0) {
            const fword head = head_Stream(result);
            const fword expected_head = head_Stream(expected);
            TEST (head - expected_head < EPSILON) {
                this_errs += 1;
                ON_FIRST_ERR({
                    fprintf(ctx.out, "Head of the sum of ");
                    debug_Stream(ctx.out, a);
                    fprintf(ctx.out, " and ");
                    debug_Stream(ctx.out, b);
                    fprintf(ctx.out, ", namelly ");
                    debug_Stream(ctx.out, result);
                    fprintf(ctx.out, ", should be %"PRIfPTR", but is %"PRIfPTR
                        ". The expected sum is ",
                        head, expected_head);
                    debug_Streamln(ctx.out, expected);
                });
            }
        } else {
            assert(depths[i] > 0);
        }

        const Stream *curr = result;
        const Stream *expected_curr = expected;
        for (size_t depth = 1; depth < depths[i]; depth += 1) {
            curr = tail_Stream(ctx.alloc, curr);
            expected_curr = tail_Stream(ctx.alloc, expected_curr);
            const fword head = head_Stream(curr);
            const fword expected_head = head_Stream(expected_curr);
            TEST (head - expected_head < EPSILON) {
                this_errs += 1;
                ON_FIRST_ERR({
                    fprintf(ctx.out, "At depth %zu, the head of the sum of ",
                        depth);
                    debug_Stream(ctx.out, a);
                    fprintf(ctx.out, " and ");
                    debug_Stream(ctx.out, b);
                    fprintf(ctx.out, ", namelly ");
                    debug_Stream(ctx.out, curr);
                    fprintf(ctx.out, ", should be %"PRIfPTR", but is %"PRIfPTR
                        ". The expected sum is ",
                        head, expected_head);
                    debug_Streamln(ctx.out, expected_curr);
                });
            }
        }

        if (this_errs) {
            errs += 1;
            fprintf(ctx.out, "displaying expected stream:\n");
            print_N_Streamln(ctx.out, ctx.alloc, expected, depths[i]);
            fprintf(ctx.out, "displaying result stream:\n");
            print_N_Streamln(ctx.out, ctx.alloc, result, depths[i]);
        }
    }
    return errs;
}

int test_all(TestCtx ctx) {
    int errs = 0;
    RUN_TEST(test_zero, ctx, errs);
    RUN_TEST(test_number, ctx, errs);
    RUN_TEST(test_lazyadd, ctx, errs);
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
            "All ok!\n");
    }
    return errs;
}
