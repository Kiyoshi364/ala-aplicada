#ifndef _STREAMS_H
#define _STREAMS_H

#include "hashi.h"

enum _StreamType {
    ZERO_STREAM,
    NUMBER_STREAM,
    LAZY_ADD_STREAM,
    LAZY_MUL_STREAM,
    STREAMTYPE_COUNT,
};

typedef struct {
    enum _StreamType e;
} StreamType;
CASSERT(STREAMTYPE_COUNT < UWORD_MAX, STREAM_TYPES_FIT);

typedef struct _Stream {
    StreamType typ;
    size_t len;
    union {
        uword data;
        fword fdata;
        const struct _Stream *stream1;
    };
    union {
        uword data2;
        const struct _Stream *stream2;
    };
} Stream;
CASSERT(sizeof(((Stream*)0)->data) == sizeof(((Stream*)0)->fdata), NUMBER_T_FITS_FLOAT);
CASSERT(sizeof(((Stream*)0)->data) == sizeof(((Stream*)0)->stream1), NUMBER_T_FITS_FLOAT);

CASSERT(sizeof(((Stream*)0)->data2) == sizeof(((Stream*)0)->stream2), NUMBER_T_FITS_FLOAT);

typedef struct {
    Stream header;
} ZeroStream;

typedef struct {
    Stream header;
} NumberStream;

typedef struct {
    Stream header;
} LazyAddStream;

typedef struct {
    Stream header;
} LazyMulStream;

fword head_Stream(const Stream stream[static 1]);
const Stream* tail_Stream(Alloc alloc, const Stream stream[static 1]);
const Stream* add_Stream(Alloc alloc, const Stream a[static 1], const Stream b[static 1]);
const Stream* mul_Stream(Alloc alloc, const Stream a[static 1], const Stream b[static 1]);
void print_Stream(FILE* out, const Stream stream[static 1]);
void print_Streamln(FILE* out, const Stream stream[static 1]);
void debug_Stream(FILE* out, const Stream stream[static 1]);
void debug_Streamln(FILE* out, const Stream stream[static 1]);
void print_N_Stream(FILE* out, Alloc alloc, const Stream stream[static 1], const size_t count);
void print_N_Streamln(FILE* out, Alloc alloc, const Stream stream[static 1], const size_t count);

#endif // _STREAMS_H

#ifdef STREAM_IMPLEMENTATION
#ifndef STREAM_IMPLEMENTED
#define STREAM_IMPLEMENTED

// ==================== GlobalContants ====================

static const char *STREAM_TYPENAME[] = {
    "ZERO",
    "NUMBER",
    "LAZY_ADD",
    "LAZY_MUL",
    "STREAMTYPE_COUNT",
};
CASSERT(ARRAY_SIZE(STREAM_TYPENAME) == STREAMTYPE_COUNT + 1, STREAM_TYPENAME_HAS_ALL_TYPES);
#define S_NAME(x) (STREAM_TYPENAME[(x)])

static ZeroStream THE_ZERO_STREAM = {
    .header = {
        .typ = { ZERO_STREAM },
        .len = sizeof(THE_ZERO_STREAM),
        .data = 0,
        .data2 = 0,
    },
};

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

// ==================== STREAMS ====================

inline static
void _stream_ok(const Stream stream[static 1]) {
    switch (stream->typ.e) {
        case ZERO_STREAM:
        {
            _zerostream_ok((const ZeroStream *) stream);
        } break;
        case NUMBER_STREAM:
        {
            _numberstream_ok((const NumberStream *) stream);
        } break;
        case LAZY_ADD_STREAM:
        {
            _lazyaddstream_ok((const LazyAddStream *) stream);
        } break;
        case LAZY_MUL_STREAM:
        {
            _lazymulstream_ok((const LazyMulStream *) stream);
        } break;
        case STREAMTYPE_COUNT:
        default:
        {
            fprintf(stderr, "stream->typ.e: %s\n", S_NAME(stream->typ.e));
            assert(0 && "Unhandled stream type");
        } break;
    }
}

fword head_Stream(const Stream stream[static 1]) {
    fword result = 0.0;
    switch (stream->typ.e) {
        case ZERO_STREAM:
        {
            _zerostream_ok((const ZeroStream *) stream);
            result = 0.0;
        } break;
        case NUMBER_STREAM:
        {
            _numberstream_ok((const NumberStream *) stream);
            result = head_NumberStream((const NumberStream *) stream);
        } break;
        case LAZY_ADD_STREAM:
        {
            _lazyaddstream_ok((const LazyAddStream *) stream);
            result = head_LazyAddStream((const LazyAddStream *) stream);
        } break;
        case LAZY_MUL_STREAM:
        {
            _lazymulstream_ok((const LazyMulStream *) stream);
            result = head_LazyMulStream((const LazyMulStream *) stream);
        } break;
        case STREAMTYPE_COUNT:
        default:
        {
            fprintf(stderr, "stream->typ.e: %s\n", S_NAME(stream->typ.e));
            assert(0 && "Unhandled stream type");
        } break;
    }
    return result;
}

const Stream* tail_Stream(Alloc alloc, const Stream stream[static 1]) {
    (void) alloc;
    const Stream *result = NULL;
    switch (stream->typ.e) {
        case ZERO_STREAM:
        {
            _zerostream_ok((const ZeroStream *) stream);
            result = stream;
        } break;
        case NUMBER_STREAM:
        {
            _numberstream_ok((const NumberStream *) stream);
            result = (const Stream *) &THE_ZERO_STREAM;
        } break;
        case LAZY_ADD_STREAM:
        {
            _lazyaddstream_ok((const LazyAddStream *) stream);
            result = (const Stream *) tail_LazyAddStream(alloc, (const LazyAddStream *) stream);
        } break;
        case LAZY_MUL_STREAM:
        {
            _lazymulstream_ok((const LazyMulStream *) stream);
            result = (const Stream *) tail_LazyMulStream(alloc, (const LazyMulStream *) stream);
        } break;
        case STREAMTYPE_COUNT:
        default:
        {
            fprintf(stderr, "stream->typ.e: %s\n", S_NAME(stream->typ.e));
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
        case CASE2(ZERO_STREAM, LAZY_ADD_STREAM):
        case CASE2(ZERO_STREAM, LAZY_MUL_STREAM):
        {
            _zerostream_ok((const ZeroStream *) a);
            _stream_ok(b);
            result = b;
        } break;
        case CASE2(NUMBER_STREAM, ZERO_STREAM):
        case CASE2(LAZY_ADD_STREAM, ZERO_STREAM):
        case CASE2(LAZY_MUL_STREAM, ZERO_STREAM):
        {
            _stream_ok(a);
            _zerostream_ok((const ZeroStream *) b);
            result = a;
        } break;
        case CASE2(NUMBER_STREAM, NUMBER_STREAM):
        {
            _numberstream_ok((const NumberStream *) a);
            _numberstream_ok((const NumberStream *) b);
            Stream *tmp = (Stream *) ALLOCATE_ONE(alloc, NumberStream);
            assert(tmp != NULL);
            const NumberStream ns = add_NumberStream(
                *(const NumberStream *) a,
                *(const NumberStream *) b
            );
            *tmp = ns.header;
            result = tmp;
        } break;
        case CASE2(NUMBER_STREAM, LAZY_ADD_STREAM):
        case CASE2(NUMBER_STREAM, LAZY_MUL_STREAM):

        case CASE2(LAZY_ADD_STREAM, NUMBER_STREAM):
        case CASE2(LAZY_ADD_STREAM, LAZY_ADD_STREAM):
        case CASE2(LAZY_ADD_STREAM, LAZY_MUL_STREAM):

        case CASE2(LAZY_MUL_STREAM, NUMBER_STREAM):
        case CASE2(LAZY_MUL_STREAM, LAZY_ADD_STREAM):
        case CASE2(LAZY_MUL_STREAM, LAZY_MUL_STREAM):
        {
            _stream_ok(a);
            _stream_ok(b);
            result = (const Stream *) make_LazyAddStream(alloc, a, b);
        } break;
        default:
        {
            fprintf(stderr, "a->typ.e: %s b->typ.e: %s\n", S_NAME(a->typ.e), S_NAME(b->typ.e));
            assert(0 && "Unhandled stream type");
        } break;
    }
    assert(result != NULL);
    return result;
}

const Stream* mul_Stream(Alloc alloc, const Stream a[static 1], const Stream b[static 1]) {
    const Stream *result = NULL;
    switch CASE2(a->typ.e, b->typ.e) {
        case CASE2(ZERO_STREAM, ZERO_STREAM):
        {
            _zerostream_ok((const ZeroStream *) a);
            _zerostream_ok((const ZeroStream *) b);
            result = a;
        } break;
        case CASE2(ZERO_STREAM, NUMBER_STREAM):
        case CASE2(ZERO_STREAM, LAZY_ADD_STREAM):
        case CASE2(ZERO_STREAM, LAZY_MUL_STREAM):
        {
            _zerostream_ok((const ZeroStream *) a);
            _stream_ok(b);
            result = a;
        } break;
        case CASE2(NUMBER_STREAM, ZERO_STREAM):
        case CASE2(LAZY_ADD_STREAM, ZERO_STREAM):
        case CASE2(LAZY_MUL_STREAM, ZERO_STREAM):
        {
            _stream_ok(a);
            _zerostream_ok((const ZeroStream *) b);
            result = b;
        } break;
        case CASE2(NUMBER_STREAM, NUMBER_STREAM):
        {
            _numberstream_ok((const NumberStream *) a);
            _numberstream_ok((const NumberStream *) b);
            const fword ha = local_head_NumberStream(*(const NumberStream *) a);
            const fword hb = local_head_NumberStream(*(const NumberStream *) b);
            result = (const Stream *) make_NumberStream(alloc, ha * hb);
        } break;
        // TODO: Try to join (number + s) and (s + number)
        case CASE2(NUMBER_STREAM, LAZY_ADD_STREAM):
        case CASE2(NUMBER_STREAM, LAZY_MUL_STREAM):

        case CASE2(LAZY_ADD_STREAM, NUMBER_STREAM):
        case CASE2(LAZY_ADD_STREAM, LAZY_ADD_STREAM):
        case CASE2(LAZY_ADD_STREAM, LAZY_MUL_STREAM):

        case CASE2(LAZY_MUL_STREAM, NUMBER_STREAM):
        case CASE2(LAZY_MUL_STREAM, LAZY_ADD_STREAM):
        case CASE2(LAZY_MUL_STREAM, LAZY_MUL_STREAM):
        {
            _stream_ok(a);
            _stream_ok(b);
            result = (const Stream *) make_LazyMulStream(alloc, a, b);
        } break;
        default:
        {
            fprintf(stderr, "a->typ.e: %s b->typ.e: %s\n", S_NAME(a->typ.e), S_NAME(b->typ.e));
            assert(0 && "Unhandled stream type");
        } break;
    }
    assert(result != NULL);
    return result;
}

void print_Stream(FILE* out, const Stream stream[static 1]) {
    switch (stream->typ.e) {
        case ZERO_STREAM:
        {
            print_ZeroStream(out, (const ZeroStream *) stream);
        } break;
        case NUMBER_STREAM:
        {
            print_NumberStream(out, (const NumberStream *) stream);
        } break;
        case LAZY_ADD_STREAM:
        {
            fprintf(out, "(");
            print_Stream(out, stream->stream1);
            fprintf(out, " ");
            print_Stream(out, stream->stream2);
            fprintf(out, " +)");
        } break;
        case LAZY_MUL_STREAM:
        {
            fprintf(out, "(");
            print_Stream(out, stream->stream1);
            fprintf(out, " ");
            print_Stream(out, stream->stream2);
            fprintf(out, " *)");
        } break;
        case STREAMTYPE_COUNT:
        default:
        {
            fprintf(stderr, "stream->typ.e: %s\n", S_NAME(stream->typ.e));
            assert(0 && "Unhandled stream type");
        } break;
    }
}

void print_Streamln(FILE* out, const Stream stream[static 1]) {
    print_Stream(out, stream);
    fprintf(out, "\n");
}

void debug_Stream(FILE* out, const Stream stream[static 1]) {
    fprintf(out, "{%s, %zu, 0x%016"PRIXPTR", 0x%016"PRIXPTR": ",
        S_NAME(stream->typ.e), stream->len, stream->data, stream->data2
    );
    print_Stream(out, stream);
    fprintf(out, "}");
}

void debug_Streamln(FILE* out, const Stream stream[static 1]) {
    debug_Stream(out, stream);
    fprintf(out, "\n");
}

void print_N_Stream(FILE* out, Alloc alloc,
    const Stream stream[static 1],
    const size_t count
) {
    if (count > 0) {
        fprintf(out, "%"PRIfPTR, head_Stream(stream));
    }
    const Stream *curr = stream;
    for (size_t i = 1; i < count; i += 1) {
        curr = tail_Stream(alloc, curr);
        fprintf(out, " %"PRIfPTR, head_Stream(curr));
    }

    fprintf(out, " [tail of ");
    print_Stream(out, curr);
    fprintf(out, "]");
}

void print_N_Streamln(FILE* out, Alloc alloc,
    const Stream stream[static 1],
    const size_t count
) {
    print_N_Stream(out, alloc, stream, count);
    fprintf(out, "\n");
}

#undef S_NAME

#endif // STREAM_IMPLEMENTED
#endif // STREAM_IMPLEMENTATION
