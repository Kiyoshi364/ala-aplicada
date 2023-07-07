// ==================== LAZY MUL STREAMS ====================

inline static
void _lazymulstream_ok(const LazyMulStream* lms) {
    assert(lms->header.typ.e == LAZY_MUL_STREAM);
    assert(lms->header.len == sizeof(LazyMulStream));
    assert(lms->header.stream1 != NULL);
    assert(lms->header.stream2 != NULL);
}

const LazyMulStream* make_LazyMulStream(Alloc alloc,
    const Stream a[static 1],
    const Stream b[static 1]
) {
    LazyMulStream *p = ALLOCATE_ONE(alloc, LazyMulStream);
    assert(p);
    *p = (LazyMulStream){
        .header = (Stream){
            .typ = { LAZY_MUL_STREAM },
            .len = sizeof(LazyMulStream),
            .stream1 = a,
            .stream2 = b,
        },
    };
    return p;
}

fword head_LazyMulStream(const LazyMulStream stream[static 1]) {
    const Stream header = stream->header;
    const fword head1 = head_Stream(header.stream1);
    const fword head2 = head_Stream(header.stream2);
    return head1 * head2;
}

const Stream* tail_LazyMulStream(Alloc alloc, const LazyMulStream stream[static 1]) {
    const Stream header = stream->header;
    const fword h1 = head_Stream(header.stream1);
    const Stream *head1 = (const Stream *) make_NumberStream(alloc, h1);
    const Stream *tail1 = tail_Stream(alloc, header.stream1);
    const Stream *tail2 = tail_Stream(alloc, header.stream2);

    const Stream *left = mul_Stream(alloc, head1, tail2);
    const Stream *right = mul_Stream(alloc, tail1, header.stream2);
    return add_Stream(alloc, left, right);
}
