// ==================== OneStream ====================

inline static
void _onestream_ok(const OneStream os[const static 1]) {
    assert(os->header.typ.e == ONE_STREAM);
    assert(os->header.len == sizeof(OneStream));
    assert(os->header.data == 1);
    assert(os->header.data2 == 0);
    assert(os == &THE_ONE_STREAM);
}

const OneStream* make_OneStream(Alloc alloc) {
    (void) alloc;
    const OneStream *p = &THE_ONE_STREAM;
    _onestream_ok(p);
    return p;
}

void print_OneStream(FILE *out, const OneStream os[static 1]) {
    _onestream_ok(os);
    fprintf(out, "(One)");
}
