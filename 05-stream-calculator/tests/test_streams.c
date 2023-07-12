#include <stdio.h>

#define STREAM_IMPLEMENTATION
#include "../streams.h"

#include "test.h"

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

    const NumberStream ns[] = {
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

int test_shift(TestCtx ctx) {
    int errs = 0;

    const Stream *n1 = (const Stream *) make_NumberStream(ctx.alloc, 1.0);
    const Stream *x = (const Stream *) make_ShiftStream(ctx.alloc,
        n1,
        0
    );
    const uword count = 3;
    const Stream *xcount = (const Stream *) make_ShiftStream(ctx.alloc,
        n1,
        count
    );

    {
        const Stream *tail = tail_Stream(ctx.alloc, (const Stream *) x);
        const fword h = head_Stream(tail);
        TEST (h - 1.0 < EPSILON) {
            errs += 1;
            ON_FIRST_ERR({
                fprintf(ctx.out, "The head of tail of ");
                debug_Stream(ctx.out, (const Stream *) x);
                fprintf(ctx.out, " should be %"PRIfPTR", but is %"PRIfPTR"\n"
                    "The tail is: ",
                    1.0, h);
                debug_Streamln(ctx.out, tail);
            });
        }
    }

    {
        const Stream *curr = xcount;
        for (uword i = 0; i < count + 1; i += 1) {
            const fword h = head_Stream(curr);
            curr = tail_Stream(ctx.alloc, curr);
            TEST (h == 0.0) {
                errs += 1;
                ON_FIRST_ERR({
                    fprintf(ctx.out, "The head of %"PRIuPTR"th tail of ",
                        i+1);
                    debug_Stream(ctx.out, (const Stream *) xcount);
                    fprintf(ctx.out, " should be %"PRIfPTR", but is %"PRIfPTR"\n"
                        "The tail is: ",
                        0.0, h);
                    debug_Streamln(ctx.out, curr);
                });
            }
        }

        const fword h = head_Stream(curr);
        TEST (h == 1.0) {
            errs += 1;
            ON_FIRST_ERR({
                fprintf(ctx.out, "The head of %"PRIuPTR"th tail of ",
                    count+1);
                debug_Stream(ctx.out, (const Stream *) xcount);
                fprintf(ctx.out, " should be %"PRIfPTR", but is %"PRIfPTR"\n"
                    "The tail is: ",
                    1.0, h);
                debug_Streamln(ctx.out, curr);
            });
        }
    }
    return errs;
}

int input_test_comm(
    TestCtx ctx,
    const Stream* (*operation)(Alloc, const Stream[1], const Stream[1]),
    size_t n,
    const Stream* streams[n][3],
    const size_t depths[n]
) {
    int errs = 0;
    for (size_t i = 0; i < n; i += 1) {
        for (size_t j = 0; j <= 1; j += 1) {
            int this_errs = 0;
            const Stream *a = streams[i][j];
            const Stream *b = streams[i][1-j];
            const Stream *const expected = streams[i][2];
            const Stream *const result = operation(ctx.alloc, a, b);

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
    }
    return errs;
}

int test_add_comm(TestCtx ctx) {
    int errs = 0;

    const Stream *n10 = (const Stream *) make_NumberStream(ctx.alloc, 10.0);
    const Stream *n15 = (const Stream *) make_NumberStream(ctx.alloc, 15.0);
    const Stream *n25 = (const Stream *) make_NumberStream(ctx.alloc, 25.0);

    const size_t depths[] = {
        3, 3, 6, 3, 3
    };

    const Stream *streams[][3] = {
        {
            n10,
            n15,
            n25,
        }, {
            n10,
            (const Stream *) make_ZeroStream(ctx.alloc),
            n10,
        }, {
            (const Stream *) make_ShiftStream(ctx.alloc,
                n10,
                2
            ),
            (const Stream *) make_ShiftStream(ctx.alloc,
                n15,
                2
            ),
            (const Stream *) make_ShiftStream(ctx.alloc,
                n25,
                2
            ),
        }, {
            (const Stream *) make_LazyAddStream(ctx.alloc,
                n10,
                n25
            ),
            (const Stream *) make_NumberStream(ctx.alloc, 7),
            (const Stream *) make_NumberStream(ctx.alloc, 42.0),
        }, {
            (const Stream *) make_LazyMulStream(ctx.alloc,
                n10,
                (const Stream *) make_NumberStream(ctx.alloc, 5.0)
            ),
            (const Stream *) make_NumberStream(ctx.alloc, 1.0),
            (const Stream *) make_NumberStream(ctx.alloc, 51.0),
        }
    };
    CASSERT(ARRAY_SIZE(depths) == ARRAY_SIZE(streams), ADD_DEPTHS_SIZE_EQ_STREAM_SIZE);

    errs += input_test_comm(
        ctx, add_Stream,
        ARRAY_SIZE(streams), streams, depths
    );
    return errs;
}

int test_mul_comm(TestCtx ctx) {
    int errs = 0;

    const Stream *n0 = (const Stream *) make_NumberStream(ctx.alloc, 0.0);
    const Stream *n10 = (const Stream *) make_NumberStream(ctx.alloc, 10.0);
    const Stream *n15 = (const Stream *) make_NumberStream(ctx.alloc, 15.0);

    const size_t depths[] = {
        3, 3, 8, 3, 3
    };

    const Stream *streams[][3] = {
        {
            n10,
            n15,
            (const Stream *) make_NumberStream(ctx.alloc, 150.0),
        }, {
            n10,
            (const Stream *) make_ZeroStream(ctx.alloc),
            n0,
        }, {
            (const Stream *) make_ShiftStream(ctx.alloc,
                n10,
                2
            ),
            (const Stream *) make_ShiftStream(ctx.alloc,
                n15,
                2
            ),
            (const Stream *) make_ShiftStream(ctx.alloc,
                (const Stream *) make_NumberStream(ctx.alloc, 150.0),
                5
            ),
        }, {
            (const Stream *) make_LazyAddStream(ctx.alloc,
                n10,
                (const Stream *) make_NumberStream(ctx.alloc, 25.0)
            ),
            (const Stream *) make_NumberStream(ctx.alloc, 7),
            (const Stream *) make_NumberStream(ctx.alloc, 245.0),
        }, {
            (const Stream *) make_LazyMulStream(ctx.alloc,
                n10,
                (const Stream *) make_NumberStream(ctx.alloc, 5.0)
            ),
            (const Stream *) make_NumberStream(ctx.alloc, 1.0),
            (const Stream *) make_NumberStream(ctx.alloc, 50.0),
        }
    };
    CASSERT(ARRAY_SIZE(depths) == ARRAY_SIZE(streams), MUL_DEPTHS_SIZE_EQ_STREAM_SIZE);

    errs += input_test_comm(
        ctx, mul_Stream,
        ARRAY_SIZE(streams), streams, depths
    );
    return errs;
}

int test_inv(TestCtx ctx) {
    int errs = 0;

    const Stream *n1 = (const Stream *) make_NumberStream(ctx.alloc, 1.0);
    const Stream *n_1 = (const Stream *) make_NumberStream(ctx.alloc, -1.0);
    const Stream *x = (const Stream *) make_ShiftStream(ctx.alloc, n1, 0);
    const Stream *inv_loop1 = (const Stream *) make_LazyAddStream(ctx.alloc,
        n1,
        (const Stream *) make_LazyMulStream(ctx.alloc,
            n_1,
            x
        )
    );
    const Stream *loop1 = (const Stream *) make_LazyInvStream(ctx.alloc,
        inv_loop1
    );
    const Stream *inv_loopnat = (const Stream *) make_LazyMulStream(ctx.alloc,
        inv_loop1,
        inv_loop1
    );
    const Stream *loopnat = (const Stream *) make_LazyInvStream(ctx.alloc,
        inv_loopnat
    );

    {
        const Stream *curr = loop1;
        for (size_t i = 0; i < 10; i += 1) {
            const fword h = head_Stream(curr);
            curr = tail_Stream(ctx.alloc, curr);

            TEST (h == 1.0) {
                errs += 1;
                ON_FIRST_ERR({
                    fprintf(ctx.out, "The head of %"PRIuPTR"th tail of ",
                        i+1);
                    debug_Stream(ctx.out, loop1);
                    fprintf(ctx.out, " should be %"PRIfPTR", but is %"PRIfPTR"\n"
                        "The tail is: ",
                        1.0, h);
                    debug_Streamln(ctx.out, curr);
                });
            }
        }
    }

    {
        const Stream *curr = loopnat;
        for (size_t i = 0; i < 10; i += 1) {
            const fword h = head_Stream(curr);
            curr = tail_Stream(ctx.alloc, curr);

            TEST (h == (fword) (i + 1)) {
                errs += 1;
                ON_FIRST_ERR({
                    fprintf(ctx.out, "The head of %"PRIuPTR"th tail of ",
                        i+1);
                    debug_Stream(ctx.out, loopnat);
                    fprintf(ctx.out, " should be %"PRIfPTR", but is %"PRIfPTR"\n"
                        "The tail is: ",
                        1.0, h);
                    debug_Streamln(ctx.out, curr);
                });
            }
        }
    }

    {
        const Stream *n10 = (const Stream *) make_NumberStream(ctx.alloc, 10.0);
        const Stream *inv_n10 = (const Stream *) inv_Stream(ctx.alloc, n10);
        const fword h = head_Stream(inv_n10);
        const Stream *tail = tail_Stream(ctx.alloc, inv_n10);
        TEST (h == 0.1) {
            errs += 1;
            ON_FIRST_ERR({
                fprintf(ctx.out, "The head of ");
                debug_Stream(ctx.out, inv_n10);
                fprintf(ctx.out, " should be %"PRIfPTR", but is %"PRIfPTR"\n"
                    "The tail is: ",
                    0.1, h);
                debug_Streamln(ctx.out, tail);
            });
        }

        for (size_t i = 0; i < 10; i += 1) {
            const fword th = head_Stream(tail);
            tail = tail_Stream(ctx.alloc, tail);
            TEST (th == 0.0) {
                errs += 1;
                ON_FIRST_ERR({
                    fprintf(ctx.out, "The head of %zuth tail of",
                        i+1);
                    debug_Stream(ctx.out, inv_n10);
                    fprintf(ctx.out, " should be %"PRIfPTR", but is %"PRIfPTR"\n"
                        "The tail is: ",
                        0.0, th);
                    debug_Streamln(ctx.out, tail);
                });
            }
        }
    }

    return errs;
}

int test_all(TestCtx ctx) {
    int errs = 0;
    RUN_TEST(test_zero, ctx, errs);
    RUN_TEST(test_number, ctx, errs);
    RUN_TEST(test_shift, ctx, errs);
    RUN_TEST(test_add_comm, ctx, errs);
    RUN_TEST(test_mul_comm, ctx, errs);
    RUN_TEST(test_inv, ctx, errs);
    // TODO test_add_assoc
    // TODO test_mul_assoc
    return errs;
}

#include "main_test.h"
