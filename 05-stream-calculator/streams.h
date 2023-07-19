#ifndef _STREAMS_H
#define _STREAMS_H

#include "hashi.h"

enum _StreamType {
    ZERO_STREAM,
    ONE_STREAM,
    NUMBER_STREAM,
    SHIFT_STREAM,
    LAZY_ADD_STREAM,
    LAZY_SUB_STREAM,
    LAZY_MUL_STREAM,
    LAZY_INV_STREAM,
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
} OneStream;

typedef struct {
    Stream header;
} NumberStream;

typedef struct {
    Stream header;
} ShiftStream;

typedef struct {
    Stream header;
} LazyAddStream;

typedef struct {
    Stream header;
} LazySubStream;

typedef struct {
    Stream header;
} LazyMulStream;

typedef struct {
    Stream header;
} LazyInvStream;

fword head_Stream(const Stream stream[static 1]);
const Stream* tail_Stream(Alloc alloc, const Stream stream[static 1]);
const Stream* add_Stream(Alloc alloc, const Stream a[static 1], const Stream b[static 1]);
const Stream* sub_Stream(Alloc alloc, const Stream a[static 1], const Stream b[static 1]);
const Stream* mul_Stream(Alloc alloc, const Stream a[static 1], const Stream b[static 1]);
const Stream* inv_Stream(Alloc alloc, const Stream stream[static 1]);
void print_Stream(FILE* out, const Stream stream[static 1], const size_t depth);
void print_Streamln(FILE* out, const Stream stream[static 1], const size_t depth);
void debug_Stream(FILE* out, const Stream stream[static 1]);
void debug_Streamln(FILE* out, const Stream stream[static 1]);
void print_N_Stream(FILE* out, Alloc alloc, const Stream stream[static 1], const size_t count, const size_t depth);
void print_N_Streamln(FILE* out, Alloc alloc, const Stream stream[static 1], const size_t count, const size_t depth);

#endif // _STREAMS_H

#ifdef STREAM_IMPLEMENTATION
#ifndef STREAM_IMPLEMENTED
#define STREAM_IMPLEMENTED

// ==================== GlobalContants ====================

#define DEFAULT_STREAM_CASE(stream) \
    default: \
    { \
        fprintf(stderr, "stream->typ.e: %s\n", S_NAME((stream)->typ.e)); \
        assert(0 && "Unhandled stream type"); \
    } break;

#define DEFAULT_STREAM_CASE2(a, b) \
    default: \
    { \
        fprintf(stderr, "a->typ.e: %s b->typ.e: %s\n", S_NAME((a)->typ.e), S_NAME((b)->typ.e)); \
        assert(0 && "Unhandled stream type"); \
    } break;

static const char *STREAM_TYPENAME[] = {
    "ZERO",
    "ONE",
    "NUMBER",
    "SHIFT",
    "LAZY_ADD",
    "LAZY_SUB",
    "LAZY_MUL",
    "LAZY_INV",
    "STREAMTYPE_COUNT",
};
CASSERT(ARRAY_SIZE(STREAM_TYPENAME) == STREAMTYPE_COUNT + 1, STREAM_TYPENAME_HAS_ALL_TYPES);
#define S_NAME(x) (STREAM_TYPENAME[(x)])

static ZeroStream THE_ZERO_STREAM = {
    .header = {
        .typ = { ZERO_STREAM },
        .len = sizeof(THE_ZERO_STREAM),
        .fdata = 0.0,
        .data2 = 0,
    },
};

static OneStream THE_ONE_STREAM = {
    .header = {
        .typ = { ONE_STREAM },
        .len = sizeof(THE_ONE_STREAM),
        .fdata = 1.0,
        .data2 = 0,
    },
};

#include "streams/zero.c"
#include "streams/one.c"
#include "streams/number.c"
#include "streams/shift.c"
#include "streams/lazyadd.c"
#include "streams/lazysub.c"
#include "streams/lazymul.c"
#include "streams/lazyinv.c"

// ==================== Dispach ADD ====================

const Stream* add_Number_Stream(Alloc alloc, const NumberStream ns[static 1], const Stream b[static 1]) {
    const Stream *result = NULL;
    switch (b->typ.e) {
        case ZERO_STREAM:
        {
            _zerostream_ok((const ZeroStream *) b);
            result = (const Stream *) ns;
        } break;
        case ONE_STREAM:
        {
            _onestream_ok((const OneStream *) b);
        } // fallthrough
        case NUMBER_STREAM:
        {
            _numberstream_ok((const NumberStream *) b);
            const fword ha = local_head_NumberStream(*(const NumberStream *) ns);
            const fword hb = local_head_NumberStream(*(const NumberStream *) b);
            result = (const Stream *) make_NumberStream(alloc, ha + hb);
        } break;
        case SHIFT_STREAM:
        {
            _shiftstream_ok((const ShiftStream *) b);
            result = (const Stream *) make_LazyAddStream(alloc, (const Stream *) ns, b);
        } break;
        case LAZY_ADD_STREAM:
        {
            _lazyaddstream_ok((const LazyAddStream *) b);
            result = (const Stream *) make_LazyAddStream(alloc, (const Stream *) ns, b);
        } break;
        case LAZY_SUB_STREAM:
        {
            _lazysubstream_ok((const LazySubStream *) b);
            result = (const Stream *) make_LazyAddStream(alloc, (const Stream *) ns, b);
        } break;
        case LAZY_MUL_STREAM:
        {
            _lazymulstream_ok((const LazyMulStream *) b);
            result = (const Stream *) make_LazyAddStream(alloc, (const Stream *) ns, b);
        } break;
        case LAZY_INV_STREAM:
        {
            _lazyinvstream_ok((const LazyInvStream *) b);
            result = (const Stream *) make_LazyAddStream(alloc, (const Stream *) ns, b);
        } break;
        case STREAMTYPE_COUNT:
        DEFAULT_STREAM_CASE(b);
    }
    assert(result != NULL);
    return result;
}

// ==================== Dispach SUB ====================

const Stream* sub_Number_Stream(Alloc alloc, const NumberStream ns[static 1], const Stream b[static 1]) {
    const Stream *result = NULL;
    switch (b->typ.e) {
        case ZERO_STREAM:
        {
            _zerostream_ok((const ZeroStream *) b);
            result = (const Stream *) ns;
        } break;
        case ONE_STREAM:
        {
            _onestream_ok((const OneStream *) b);
        } // fallthrough
        case NUMBER_STREAM:
        {
            _numberstream_ok((const NumberStream *) b);
            const fword ha = local_head_NumberStream(*(const NumberStream *) ns);
            const fword hb = local_head_NumberStream(*(const NumberStream *) b);
            result = (const Stream *) make_NumberStream(alloc, ha - hb);
        } break;
        case SHIFT_STREAM:
        {
            _shiftstream_ok((const ShiftStream *) b);
            result = (const Stream *) make_LazySubStream(alloc, (const Stream *) ns, b);
        } break;
        case LAZY_ADD_STREAM:
        {
            _lazyaddstream_ok((const LazyAddStream *) b);
            result = (const Stream *) make_LazySubStream(alloc, (const Stream *) ns, b);
        } break;
        case LAZY_SUB_STREAM:
        {
            _lazysubstream_ok((const LazySubStream *) b);
            result = (const Stream *) make_LazySubStream(alloc, (const Stream *) ns, b);
        } break;
        case LAZY_MUL_STREAM:
        {
            _lazymulstream_ok((const LazyMulStream *) b);
            result = (const Stream *) make_LazySubStream(alloc, (const Stream *) ns, b);
        } break;
        case LAZY_INV_STREAM:
        {
            _lazyinvstream_ok((const LazyInvStream *) b);
            result = (const Stream *) make_LazySubStream(alloc, (const Stream *) ns, b);
        } break;
        case STREAMTYPE_COUNT:
        DEFAULT_STREAM_CASE(b);
    }
    assert(result != NULL);
    return result;
}

// ==================== Dispach MUL ====================

const Stream* mul_Number_Stream(Alloc alloc, const NumberStream ns[static 1], const Stream b[static 1]) {
    const Stream *result = NULL;
    switch (b->typ.e) {
        case ZERO_STREAM:
        {
            _zerostream_ok((const ZeroStream *) b);
            result = b;
        } break;
        case ONE_STREAM:
        {
            _onestream_ok((const OneStream *) b);
            result = (const Stream *) ns;
        } break;
        case NUMBER_STREAM:
            _numberstream_ok((const NumberStream *) b);
            const fword ha = local_head_NumberStream(*(const NumberStream *) ns);
            const fword hb = local_head_NumberStream(*(const NumberStream *) b);
            result = (const Stream *) make_NumberStream(alloc, ha * hb);
        {
        } break;
        case SHIFT_STREAM:
        {
            _shiftstream_ok((const ShiftStream *) b);
            result = (const Stream *) make_LazyMulStream(alloc, (const Stream *) ns, b);
        } break;
        case LAZY_ADD_STREAM:
        {
            _lazyaddstream_ok((const LazyAddStream *) b);
            result = (const Stream *) make_LazyMulStream(alloc, (const Stream *) ns, b);
        } break;
        case LAZY_SUB_STREAM:
        {
            _lazysubstream_ok((const LazySubStream *) b);
            result = (const Stream *) make_LazyMulStream(alloc, (const Stream *) ns, b);
        } break;
        case LAZY_MUL_STREAM:
        {
            _lazymulstream_ok((const LazyMulStream *) b);
            result = (const Stream *) make_LazyMulStream(alloc, (const Stream *) ns, b);
        } break;
        case LAZY_INV_STREAM:
        {
            _lazyinvstream_ok((const LazyInvStream *) b);
            result = (const Stream *) make_LazyMulStream(alloc, (const Stream *) ns, b);
        } break;
        case STREAMTYPE_COUNT:
        DEFAULT_STREAM_CASE(b);
    }
    assert(result != NULL);
    return result;
}

// ==================== STREAMS ====================

inline static
void _stream_ok(const Stream stream[static 1]) {
    switch (stream->typ.e) {
        case ZERO_STREAM:
        {
            _zerostream_ok((const ZeroStream *) stream);
        } break;
        case ONE_STREAM:
        {
            _onestream_ok((const OneStream *) stream);
        } break;
        case NUMBER_STREAM:
        {
            _numberstream_ok((const NumberStream *) stream);
        } break;
        case SHIFT_STREAM:
        {
            _shiftstream_ok((const ShiftStream *) stream);
        } break;
        case LAZY_ADD_STREAM:
        {
            _lazyaddstream_ok((const LazyAddStream *) stream);
        } break;
        case LAZY_SUB_STREAM:
        {
            _lazysubstream_ok((const LazySubStream *) stream);
        } break;
        case LAZY_MUL_STREAM:
        {
            _lazymulstream_ok((const LazyMulStream *) stream);
        } break;
        case LAZY_INV_STREAM:
        {
            _lazyinvstream_ok((const LazyInvStream *) stream);
        } break;
        case STREAMTYPE_COUNT:
        DEFAULT_STREAM_CASE(stream);
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
        case ONE_STREAM:
        {
            _onestream_ok((const OneStream *) stream);
            result = 1.0;
        } break;
        case NUMBER_STREAM:
        {
            _numberstream_ok((const NumberStream *) stream);
            result = head_NumberStream((const NumberStream *) stream);
        } break;
        case SHIFT_STREAM:
        {
            _shiftstream_ok((const ShiftStream *) stream);
            result = head_ShiftStream((const ShiftStream *) stream);
        } break;
        case LAZY_ADD_STREAM:
        {
            _lazyaddstream_ok((const LazyAddStream *) stream);
            result = head_LazyAddStream((const LazyAddStream *) stream);
        } break;
        case LAZY_SUB_STREAM:
        {
            _lazysubstream_ok((const LazySubStream *) stream);
            result = head_LazySubStream((const LazySubStream *) stream);
        } break;
        case LAZY_MUL_STREAM:
        {
            _lazymulstream_ok((const LazyMulStream *) stream);
            result = head_LazyMulStream((const LazyMulStream *) stream);
        } break;
        case LAZY_INV_STREAM:
        {
            _lazyinvstream_ok((const LazyInvStream *) stream);
            result = head_LazyInvStream((const LazyInvStream *) stream);
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
        case ONE_STREAM:
        {
            _onestream_ok((const OneStream *) stream);
        } // fallthrough
        case NUMBER_STREAM:
        {
            _numberstream_ok((const NumberStream *) stream);
            result = (const Stream *) &THE_ZERO_STREAM;
        } break;
        case SHIFT_STREAM:
        {
            _shiftstream_ok((const ShiftStream *) stream);
            result = tail_ShiftStream(alloc, (const ShiftStream *) stream);
        } break;
        case LAZY_ADD_STREAM:
        {
            _lazyaddstream_ok((const LazyAddStream *) stream);
            result = (const Stream *) tail_LazyAddStream(alloc, (const LazyAddStream *) stream);
        } break;
        case LAZY_SUB_STREAM:
        {
            _lazysubstream_ok((const LazySubStream *) stream);
            result = (const Stream *) tail_LazySubStream(alloc, (const LazySubStream *) stream);
        } break;
        case LAZY_MUL_STREAM:
        {
            _lazymulstream_ok((const LazyMulStream *) stream);
            result = (const Stream *) tail_LazyMulStream(alloc, (const LazyMulStream *) stream);
        } break;
        case LAZY_INV_STREAM:
        {
            _lazyinvstream_ok((const LazyInvStream *) stream);
            result = (const Stream *) tail_LazyInvStream(alloc, (const LazyInvStream *) stream);
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
    switch (a->typ.e) {
        case ZERO_STREAM:
        {
            _zerostream_ok((const ZeroStream *) a);
            _stream_ok(b);
            result = b;
        } break;
        case ONE_STREAM:
        {
            _onestream_ok((const OneStream *) a);
        } // fallthrough
        case NUMBER_STREAM:
        {
            _numberstream_ok((const NumberStream *) a);
            result = add_Number_Stream(alloc, (const NumberStream *) a, b);
        } break;
        // TODO: Try to join (shift + shift)
        case SHIFT_STREAM:
        // TODO: Try to join (s + number)
        case LAZY_ADD_STREAM:
        case LAZY_SUB_STREAM:
        case LAZY_MUL_STREAM:
        case LAZY_INV_STREAM:
        {
            _stream_ok(a);
            switch (b->typ.e) {
                case ZERO_STREAM:
                {
                    _zerostream_ok((const ZeroStream *) b);
                    result = a;
                } break;
                case ONE_STREAM:
                case NUMBER_STREAM:
                case SHIFT_STREAM:
                case LAZY_ADD_STREAM:
                case LAZY_SUB_STREAM:
                case LAZY_MUL_STREAM:
                case LAZY_INV_STREAM:
                {
                    _stream_ok(b);
                    result = (const Stream *) make_LazyAddStream(alloc, a, b);
                } break;
                case STREAMTYPE_COUNT:
                DEFAULT_STREAM_CASE2(a, b);
            }
        } break;
        case STREAMTYPE_COUNT:
        DEFAULT_STREAM_CASE2(a, b);
    }
    assert(result != NULL);
    return result;
}

const Stream* sub_Stream(Alloc alloc, const Stream a[static 1], const Stream b[static 1]) {
    const Stream *result = NULL;
    switch (a->typ.e) {
        case ZERO_STREAM:
        {
            _zerostream_ok((const ZeroStream *) a);
            _stream_ok(b);
            if (b->typ.e == ZERO_STREAM) {
                result = a;
            } else {
                result = (const Stream *) make_LazySubStream(alloc, a, b);
            }
        } break;
        case ONE_STREAM:
        {
            _onestream_ok((const OneStream *) a);
        } // fallthrough
        case NUMBER_STREAM:
        {
            _numberstream_ok((const NumberStream *) a);
            result = sub_Number_Stream(alloc, (const NumberStream *) a, b);
        } break;
        // TODO: Try to join (shift + shift)
        case SHIFT_STREAM:
        // TODO: Try to join (s + number)
        case LAZY_ADD_STREAM:
        case LAZY_SUB_STREAM:
        case LAZY_MUL_STREAM:
        case LAZY_INV_STREAM:
        {
            _stream_ok(a);
            switch (b->typ.e) {
                case ZERO_STREAM:
                {
                    _zerostream_ok((const ZeroStream *) b);
                    result = a;
                } break;
                case ONE_STREAM:
                case NUMBER_STREAM:
                case SHIFT_STREAM:
                case LAZY_ADD_STREAM:
                case LAZY_SUB_STREAM:
                case LAZY_MUL_STREAM:
                case LAZY_INV_STREAM:
                {
                    _stream_ok(b);
                    result = (const Stream *) make_LazySubStream(alloc, a, b);
                } break;
                case STREAMTYPE_COUNT:
                DEFAULT_STREAM_CASE2(a, b);
            }
        } break;
        case STREAMTYPE_COUNT:
        DEFAULT_STREAM_CASE2(a, b);
    }
    assert(result != NULL);
    return result;
}

const Stream* mul_Stream(Alloc alloc, const Stream a[static 1], const Stream b[static 1]) {
    const Stream *result = NULL;
    switch (a->typ.e) {
        case ZERO_STREAM:
        {
            _zerostream_ok((const ZeroStream *) a);
            _stream_ok(b);
            result = a;
        } break;
        case ONE_STREAM:
        {
            _onestream_ok((const OneStream *) a);
            _stream_ok(b);
            result = b;
        } break;
        case NUMBER_STREAM:
        {
            _numberstream_ok((const NumberStream *) a);
            result = mul_Number_Stream(alloc, (const NumberStream *) a, b);
        } break;
        // TODO: Try to join (shift * shift)
        case SHIFT_STREAM:
        // TODO: Try to join (s * number)
        case LAZY_ADD_STREAM:
        case LAZY_SUB_STREAM:
        case LAZY_MUL_STREAM:
        case LAZY_INV_STREAM:
        {
            _stream_ok(a);
            switch (b->typ.e) {
                case ZERO_STREAM:
                {
                    _zerostream_ok((const ZeroStream *) b);
                    result = b;
                } break;
                case ONE_STREAM:
                {
                    _onestream_ok((const OneStream *) b);
                    result = a;
                } break;
                case NUMBER_STREAM:
                case SHIFT_STREAM:
                case LAZY_ADD_STREAM:
                case LAZY_SUB_STREAM:
                case LAZY_MUL_STREAM:
                case LAZY_INV_STREAM:
                {
                    _stream_ok(b);
                    result = (const Stream *) make_LazyMulStream(alloc, a, b);
                } break;
                case STREAMTYPE_COUNT:
                DEFAULT_STREAM_CASE2(a, b);
            }
        } break;
        case STREAMTYPE_COUNT:
        DEFAULT_STREAM_CASE2(a, b);
    }
    assert(result != NULL);
    return result;
}

const Stream* inv_Stream(Alloc alloc, const Stream stream[static 1]) {
    const Stream *result = NULL;
    switch (stream->typ.e) {
        case ZERO_STREAM:
        {
            _zerostream_ok((const ZeroStream *) stream);
            // NOTE: it is not defined
            result = stream;
        } break;
        case ONE_STREAM:
        {
            _onestream_ok((const OneStream *) stream);
            result = stream;
        } break;
        case NUMBER_STREAM:
        {
            _numberstream_ok((const NumberStream *) stream);
            const fword head = head_NumberStream((const NumberStream *) stream);
            result = (const Stream *) make_NumberStream(alloc, 1.0 / head);
        } break;
        case SHIFT_STREAM:
        {
            _shiftstream_ok((const ShiftStream *) stream);
            // NOTE: it is not defined
            result = (const Stream *) make_ZeroStream(alloc);
        } break;
        case LAZY_ADD_STREAM:
        case LAZY_SUB_STREAM:
        case LAZY_MUL_STREAM:
        {
            _stream_ok(stream);
            result = (const Stream *) make_LazyInvStream(alloc, stream);
        } break;
        case LAZY_INV_STREAM:
        {
            _lazyinvstream_ok((const LazyInvStream *) stream);
            result = stream->stream1;
        } break;
        case STREAMTYPE_COUNT:
        DEFAULT_STREAM_CASE(stream);
    }
    assert(result != NULL);
    return result;
}

void print_Stream(FILE* out, const Stream stream[static 1], const size_t depth) {
    switch (stream->typ.e) {
        case ZERO_STREAM:
        {
            print_ZeroStream(out, (const ZeroStream *) stream);
        } break;
        case ONE_STREAM:
        {
            print_OneStream(out, (const OneStream *) stream);
        } break;
        case NUMBER_STREAM:
        {
            print_NumberStream(out, (const NumberStream *) stream);
        } break;
        case SHIFT_STREAM:
        {
            _shiftstream_ok((const ShiftStream *) stream);
            if (depth == 0) {
                fprintf(out, "(& ^%"PRIuPTR")", (stream->data2) + 1);
            } else {
                fprintf(out, "(");
                print_Stream(out, stream->stream1, depth-1);
                fprintf(out, " ");
                fprintf(out, "^%"PRIuPTR")", (stream->data2) + 1);
            }
        } break;
        case LAZY_ADD_STREAM:
        {
            _lazyaddstream_ok((const LazyAddStream *) stream);
            if (depth == 0) {
                fprintf(out, "(& & +)");
            } else {
                fprintf(out, "(");
                print_Stream(out, stream->stream1, depth-1);
                fprintf(out, " ");
                print_Stream(out, stream->stream2, depth-1);
                fprintf(out, " +)");
            }
        } break;
        case LAZY_SUB_STREAM:
        {
            _lazysubstream_ok((const LazySubStream *) stream);
            if (depth == 0) {
                fprintf(out, "(& & -)");
            } else {
                fprintf(out, "(");
                print_Stream(out, stream->stream1, depth-1);
                fprintf(out, " ");
                print_Stream(out, stream->stream2, depth-1);
                fprintf(out, " -)");
            }
        } break;
        case LAZY_MUL_STREAM:
        {
            _lazymulstream_ok((const LazyMulStream *) stream);
            if (depth == 0) {
                fprintf(out, "(& & -)");
            } else {
                fprintf(out, "(");
                print_Stream(out, stream->stream1, depth-1);
                fprintf(out, " ");
                print_Stream(out, stream->stream2, depth-1);
                fprintf(out, " *)");
            }
        } break;
        case LAZY_INV_STREAM:
        {
            _lazyinvstream_ok((const LazyInvStream *) stream);
            if (depth == 0) {
                fprintf(out, "(& %%)");
            } else {
                fprintf(out, "(");
                print_Stream(out, stream->stream1, depth-1);
                fprintf(out, " %%)");
            }
        } break;
        case STREAMTYPE_COUNT:
        DEFAULT_STREAM_CASE(stream);
    }
}

void print_Streamln(FILE* out, const Stream stream[static 1], const size_t depth) {
    print_Stream(out, stream, depth);
    fprintf(out, "\n");
}

void debug_Stream(FILE* out, const Stream stream[static 1]) {
    fprintf(out, "{%s, %zu, 0x%016"PRIXPTR", 0x%016"PRIXPTR": ",
        S_NAME(stream->typ.e), stream->len, stream->data, stream->data2
    );
    print_Stream(out, stream, 0xFFFF);
    fprintf(out, "}");
}

void debug_Streamln(FILE* out, const Stream stream[static 1]) {
    debug_Stream(out, stream);
    fprintf(out, "\n");
}

void print_N_Stream(FILE* out, Alloc alloc,
    const Stream stream[static 1],
    const size_t count,
    const size_t depth
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
    print_Stream(out, curr, depth);
    fprintf(out, "]");
}

void print_N_Streamln(FILE* out, Alloc alloc,
    const Stream stream[static 1],
    const size_t count,
    const size_t depth
) {
    print_N_Stream(out, alloc, stream, count, depth);
    fprintf(out, "\n");
}

#undef S_NAME
#undef DEFAULT_STREAM_CASE
#undef DEFAULT_STREAM_CASE2

#endif // STREAM_IMPLEMENTED
#endif // STREAM_IMPLEMENTATION
