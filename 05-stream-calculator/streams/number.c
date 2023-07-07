// ==================== NumberStream ====================

inline static
void _local_numberstream_ok(NumberStream ns) {
    assert(ns.header.typ.e == NUMBER_STREAM);
    assert(ns.header.len == sizeof(NumberStream));
    assert(ns.header.data2 == 0);
}

inline static
void _numberstream_ok(const NumberStream* ns) {
    _local_numberstream_ok(*ns);
}

NumberStream local_NumberStream(fword number) {
    return (NumberStream){
        .header = (Stream){
            .typ = { NUMBER_STREAM },
            .len = sizeof(NumberStream),
            .fdata = number,
            .data2 = 0,
        },
    };
}

const NumberStream* make_NumberStream(Alloc alloc, fword number) {
    NumberStream *p = ALLOCATE_ONE(alloc, NumberStream);
    assert(p);
    *p = local_NumberStream(number);
    return p;
}

fword local_head_NumberStream(NumberStream stream) {
    return stream.header.fdata;
}

fword head_NumberStream(const NumberStream stream[static 1]) {
    return local_head_NumberStream(*stream);
}

NumberStream add_NumberStream(NumberStream a, NumberStream b) {
    _local_numberstream_ok(a);
    _local_numberstream_ok(b);
    return local_NumberStream(a.header.fdata + b.header.fdata);
}

void local_print_NumberStream(FILE *out, NumberStream stream) {
    _local_numberstream_ok(stream);
    fprintf(out, "(Number: %"PRIfPTR")", stream.header.fdata);
}

void print_NumberStream(FILE *out, const NumberStream stream[static 1]) {
    local_print_NumberStream(out, *stream);
}
