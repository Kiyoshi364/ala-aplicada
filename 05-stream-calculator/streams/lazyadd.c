// ==================== LAZY ADD STREAMS ====================

inline static
void _lazyaddstream_ok(const LazyAddStream* las) {
    assert(las->header.typ.e == LAZY_ADD_STREAM);
    assert(las->header.len == sizeof(LazyAddStream));
    assert(las->header.stream1 != NULL);
    assert(las->header.stream2 != NULL);
}

const LazyAddStream* make_LazyAddStream(
    Alloc alloc,
    const Stream a[static 1],
    const Stream b[static 1]
) {
    LazyAddStream *p = ALLOCATE_ONE(alloc, LazyAddStream);
    assert(p);
    *p = (LazyAddStream){
        .header = (Stream){
            .typ = { LAZY_ADD_STREAM },
            .len = sizeof(LazyAddStream),
            .stream1 = a,
            .stream2 = b,
        },
    };
    return p;
}

fword head_LazyAddStream(const LazyAddStream stream[static 1]) {
    const Stream header = stream->header;
    const fword head1 = head_Stream(header.stream1);
    const fword head2 = head_Stream(header.stream2);
    return head1 + head2;
}

const Stream* tail_LazyAddStream(Alloc alloc, const LazyAddStream stream[static 1]) {
    const Stream header = stream->header;
    const Stream *tail1 = tail_Stream(alloc, header.stream1);
    const Stream *tail2 = tail_Stream(alloc, header.stream2);
    return add_Stream(alloc, tail1, tail2);
}
