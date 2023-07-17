// ==================== LAZY SUB STREAMS ====================

inline static
void _lazysubstream_ok(const LazySubStream* lbs) {
    assert(lbs->header.typ.e == LAZY_SUB_STREAM);
    assert(lbs->header.len == sizeof(LazySubStream));
    assert(lbs->header.stream1 != NULL);
    assert(lbs->header.stream2 != NULL);
}

const LazySubStream* make_LazySubStream(
    Alloc alloc,
    const Stream a[static 1],
    const Stream b[static 1]
) {
    LazySubStream *p = ALLOCATE_ONE(alloc, LazySubStream);
    assert(p);
    *p = (LazySubStream){
        .header = (Stream){
            .typ = { LAZY_SUB_STREAM },
            .len = sizeof(LazySubStream),
            .stream1 = a,
            .stream2 = b,
        },
    };
    return p;
}

fword head_LazySubStream(const LazySubStream stream[static 1]) {
    const Stream header = stream->header;
    const fword head1 = head_Stream(header.stream1);
    const fword head2 = head_Stream(header.stream2);
    return head1 - head2;
}

const Stream* tail_LazySubStream(Alloc alloc, const LazySubStream stream[static 1]) {
    const Stream header = stream->header;
    const Stream *tail1 = tail_Stream(alloc, header.stream1);
    const Stream *tail2 = tail_Stream(alloc, header.stream2);
    return sub_Stream(alloc, tail1, tail2);
}
