#ifndef _STREAMS_H
#define _STREAMS_H

enum _StreamType{
    ZERO_STREAM,
    NUMBER_STREAM,
    STREAMTYPE_COUNT,
};

typedef struct {
    uword e;
} StreamType;
CASSERT(STREAMTYPE_COUNT < UWORD_MAX, STREAM_TYPES_FIT);

typedef struct {
    StreamType typ;
    size_t len;
    union {
        uword data;
        fword fdata;
    };
} Stream;
CASSERT(sizeof(((Stream*)0)->data) == sizeof(((Stream*)0)->fdata), NUMBER_T_FITS_FLOAT);

typedef struct {
    Stream header;
} ZeroStream;

typedef struct {
    Stream header;
} NumberStream;

#endif // _STREAMS_H

#ifdef STREAM_IMPLEMENTATION
#ifndef STREAM_IMPLEMENTED
#define STREAM_IMPLEMENTED

// ==================== GlobalContants ====================

static ZeroStream THE_ZERO_STREAM = {
    .header = {
        .typ = { ZERO_STREAM },
        .len = sizeof(THE_ZERO_STREAM),
        .data = 0,
    },
};

// ==================== ZeroStream ====================

inline static
void _zerostream_ok(const ZeroStream zs[const static 1]) {
    assert(zs->header.typ.e == ZERO_STREAM);
    assert(zs->header.len == sizeof(ZeroStream));
    assert(zs->header.data == 0);
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

// ==================== NumberStream ====================

inline static
void _numberstream_ok(NumberStream ns) {
    assert(ns.header.typ.e == NUMBER_STREAM);
    assert(ns.header.len == sizeof(NumberStream));
}

NumberStream local_NumberStream(fword number) {
    return (NumberStream){
        .header = (Stream){
            .typ = { NUMBER_STREAM },
            .len = sizeof(NumberStream),
            .fdata = number,
        },
    };
}

const NumberStream* make_NumberStream(Alloc alloc, fword number) {
    NumberStream *p = ALLOCATE_ONE(alloc, NumberStream);
    assert(p);
    *p = (NumberStream){
        .header = (Stream){
            .typ = { NUMBER_STREAM },
            .len = sizeof(*p),
            .fdata = number,
        },
    };
    return p;
}

NumberStream add_NumberStream(NumberStream a, NumberStream b) {
    _numberstream_ok(a);
    _numberstream_ok(b);
    return (NumberStream){
        .header = (Stream){
            .typ = { NUMBER_STREAM },
            .len = sizeof(NumberStream),
            .fdata = a.header.fdata + b.header.fdata,
        },
    };
}

void print_NumberStream(FILE *out, NumberStream stream) {
    assert(stream.header.typ.e == NUMBER_STREAM);
    fprintf(out, "(Number: %"PRIfPTR")", stream.header.fdata);
}

// ==================== STREAMS ====================

fword head_Stream(Alloc alloc, Stream stream[static 1]) {
    (void) alloc;
    uword result = 0;
    switch (stream->typ.e) {
        case ZERO_STREAM: {
            _zerostream_ok((ZeroStream *) stream);
            result = 0;
        } break;
        case NUMBER_STREAM: {
            _numberstream_ok(*(NumberStream *) stream);
            result = stream->data;
        } break;
        default: {
            fprintf(stderr, "stream->typ.e: %"PRIuPTR"\n", stream->typ.e);
            assert(0 && "Unhandled stream type");
        } break;
    }
    return result;
}

const Stream* tail_Stream(Alloc alloc, Stream stream[static 1]) {
    (void) alloc;
    Stream *result = NULL;
    switch (stream->typ.e) {
        case ZERO_STREAM: {
            _zerostream_ok((ZeroStream *) stream);
            result = stream;
        } break;
        case NUMBER_STREAM: {
            _numberstream_ok(*(NumberStream *) stream);
            result = (Stream *) &THE_ZERO_STREAM;
        } break;
        default: {
            fprintf(stderr, "stream->typ.e: %"PRIuPTR"\n", stream->typ.e);
            assert(0 && "Unhandled stream type");
        } break;
    }
    return result;
}

#define CASE2(a, b) (((a) << 8) | (b))

const Stream* add_Stream(Alloc alloc, const Stream a[static 1], const Stream b[static 1]) {
    const Stream *result = NULL;
    switch CASE2(a->typ.e, b->typ.e) {
        case CASE2(ZERO_STREAM, ZERO_STREAM):
        {
            _zerostream_ok((const ZeroStream *) a);
            _zerostream_ok((const ZeroStream *) b);
            result = a;
        } break;
        case CASE2(ZERO_STREAM, NUMBER_STREAM):
        {
            _zerostream_ok((const ZeroStream *) a);
            _numberstream_ok(*(const NumberStream *) b);
            result = b;
        } break;
        case CASE2(NUMBER_STREAM, ZERO_STREAM):
        {
            _numberstream_ok(*(const NumberStream *) a);
            _zerostream_ok((const ZeroStream *) b);
            result = a;
        } break;
        case CASE2(NUMBER_STREAM, NUMBER_STREAM):
        {
            _numberstream_ok(*(NumberStream const *) a);
            _numberstream_ok(*(NumberStream const *) b);
            Stream *tmp = (Stream *) ALLOCATE_ONE(alloc, NumberStream);
            assert(tmp != NULL);
            const NumberStream ns = add_NumberStream(
                *(const NumberStream *) a,
                *(const NumberStream *) b
            );
            *tmp = ns.header;
            result = tmp;
        } break;
        default: {
            fprintf(stderr, "a->typ.e: %"PRIuPTR" b->typ.e: %"PRIuPTR"\n", a->typ.e, b->typ.e);
            assert(0 && "Unhandled stream type");
        } break;
    }
    assert(result != NULL);
    return b;
}

void print_Stream(FILE* out, const Stream stream[static 1]) {
    switch (stream->typ.e) {
        case ZERO_STREAM: {
            print_ZeroStream(out, (const ZeroStream *) stream);
        } break;
        case NUMBER_STREAM: {
            print_NumberStream(out, *(const NumberStream *) stream);
        } break;
        default: {
            fprintf(stderr, "stream->typ.e: %"PRIuPTR"\n", stream->typ.e);
            assert(0 && "Unhandled stream type");
        } break;
    }
}

void print_Streamln(FILE* out, const Stream stream[static 1]) {
    print_Stream(out, stream);
    fprintf(out, "\n");
}

void debug_Stream(FILE* out, const Stream stream[static 1]) {
    fprintf(out, "{%"PRIuPTR", %zu, 0x%016"PRIXPTR": ",
        stream->typ.e, stream->len, stream->data
    );
    print_Stream(out, stream);
    fprintf(out, "}");
}

void debug_Streamln(FILE* out, const Stream stream[static 1]) {
    debug_Stream(out, stream);
    fprintf(out, "\n");
}

#endif // STREAM_IMPLEMENTED
#endif // STREAM_IMPLEMENTATION
