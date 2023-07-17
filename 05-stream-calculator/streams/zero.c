// ==================== ZeroStream ====================

inline static
void _zerostream_ok(const ZeroStream zs[const static 1]) {
    assert(zs->header.typ.e == ZERO_STREAM);
    assert(zs->header.len == sizeof(ZeroStream));
    assert(zs->header.data == 0);
    assert(zs->header.data2 == 0);
    assert(zs == &THE_ZERO_STREAM);
}

const ZeroStream* make_ZeroStream(Alloc alloc) {
    (void) alloc;
    const ZeroStream *p = &THE_ZERO_STREAM;
    _zerostream_ok(p);
    return p;
}

void print_ZeroStream(FILE *out, const ZeroStream zs[static 1]) {
    _zerostream_ok(zs);
    fprintf(out, "(Zero)");
}
