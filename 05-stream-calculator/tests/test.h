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
