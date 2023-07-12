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
