// ==================== LAZY INV STREAMS ====================

inline static
void _lazyinvstream_ok(const LazyInvStream* lis) {
    assert(lis->header.typ.e == LAZY_INV_STREAM);
    assert(lis->header.len == sizeof(LazyInvStream));
    assert(lis->header.stream1 != NULL);
}

const LazyInvStream* make_LazyInvStream(
    Alloc alloc,
    const Stream stream[static 1]
) {
    LazyInvStream *p = ALLOCATE_ONE(alloc, LazyInvStream);
    assert(p);
    *p = (LazyInvStream){
        .header = (Stream){
            .typ = { LAZY_INV_STREAM },
            .len = sizeof(LazyInvStream),
            .stream1 = stream,
            .stream2 = NULL,
        },
    };
    return p;
}

fword head_LazyInvStream(const LazyInvStream stream[static 1]) {
    const Stream header = stream->header;
    const fword head = head_Stream(header.stream1);
    if (head < EPSILON) {
        return 0;
    } else {
        return 1.0 / head;
    }
}

const Stream* tail_LazyInvStream(Alloc alloc, const LazyInvStream stream[static 1]) {
    const Stream header = stream->header;
    const fword h = head_Stream(header.stream1);
    const Stream *head = (const Stream *) make_NumberStream(alloc, -h);
    const Stream *tail = tail_Stream(alloc, header.stream1);
    return mul_Stream(alloc,
        mul_Stream(alloc, head, tail),
        (const Stream *) stream
    );
}
