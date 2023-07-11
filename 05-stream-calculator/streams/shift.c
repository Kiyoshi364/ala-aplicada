// ==================== ShiftStream ====================

inline static
void _local_shiftstream_ok(ShiftStream ss) {
    assert(ss.header.typ.e == SHIFT_STREAM);
    assert(ss.header.len == sizeof(ShiftStream));
}

inline static
void _shiftstream_ok(const ShiftStream* ss) {
    _local_shiftstream_ok(*ss);
}

ShiftStream local_ShiftStream(const Stream stream[static 1], uword shift) {
    return (ShiftStream){
        .header = (Stream){
            .typ = { SHIFT_STREAM },
            .len = sizeof(ShiftStream),
            .stream1 = stream,
            // Note: it is zero-encoded (0 means 1, ..., 9 means 10, ...)
            .data2 = shift,
        },
    };
}

const ShiftStream* make_ShiftStream(Alloc alloc, const Stream stream[static 1], uword shift) {
    ShiftStream *p = ALLOCATE_ONE(alloc, ShiftStream);
    assert(p);
    *p = local_ShiftStream(stream, shift);
    return p;
}

fword head_ShiftStream(const ShiftStream stream[static 1]) {
    _shiftstream_ok(stream);
    return 0;
}

const Stream* tail_ShiftStream(Alloc alloc, const ShiftStream stream[static 1]) {
    _shiftstream_ok(stream);
    const Stream *result = NULL;
    if (stream->header.data2 > 0) {
        result = (const Stream *) make_ShiftStream(alloc, stream->header.stream1, (stream->header.data2) - 1);
    } else {
        result = stream->header.stream1;
    }
    assert(result != NULL);
    return result;
}
