#include <stdio.h>

#include "hashi.h"
#define STREAM_IMPLEMENTATION
#include "streams.h"

#define LEXER_IMPLEMENTATION
#include "parser/lexer.h"

#define MAX_LINE 0x400

size_t readln_low(size_t len, byte buffer[len], FILE *stream) {
    size_t i = 0;
    int c = '\0';
    while (
        i < len
        && !(i > 0 && buffer[i-1] == '\n')
        && !(c = getc(stream), feof(stream))
    ) {
        assert(0 <= c && c < 0x100);
        if ('A' <= c && c <= 'Z') {
            buffer[i] = (u8) (c + ('a' - 'A'));
        } else {
            buffer[i] = (u8) c;
        }
        i += 1;
    }
    return i;
}

size_t read(size_t len, byte buffer[len], FILE *stream) {
    return fread(buffer, sizeof(buffer[0]), len, stream);
}

typedef struct {
    int dummy;
} ReplState;

typedef struct {
    union {
        struct {
            ReplState repl;
        } ok;
        struct {
            u8 dummy;
        } err;
    } as;
    b8 is_ok;
} NextState;

NextState advance_state_with_input(
    Alloc alloc,
    FILE *stream,
    const ReplState repl,
    const StrView input
) {
    (void) alloc;
    (void) repl;
    NextToken nt = next_token(new_lexer(input));
    while (
        nt.token.type != TK_NEW_LINE
        && nt.token.type != TK_EOF
    ) {
        const Token token = nt.token;
        fprintf(stream, "%s '%"PRIVIEW"'\n",
            TK_NAME(token.type),
            STRVIEW(token.text)
        );
        nt = next_token(nt.state);
    }

    if (nt.token.type == TK_NEW_LINE) {
        nt = next_token(nt.state);
    }
    assert(nt.token.type == TK_EOF
        && nt.token.text.len == 0
        && nt.token.text.buffer == NULL
    );
    // TODO
    return (NextState) {
        .as.ok = {
            .repl = repl,
        },
    };
}

int main(void) {
    FILE *finp = stdin;
    FILE *fout = stdout;
    FILE *ferr = stderr;
    Alloc alloc = MAKE_ALLOC(0x100);

    static byte buffer[MAX_LINE] = {0};
    size_t len = 1;

    ReplState repl = (ReplState) {
        0
    };
    while (len > 0) {
        // Read
        len = readln_low(ARRAY_SIZE(buffer), buffer, finp);
        assert(len > 0 || feof(finp));
        fprintf(fout, "D: len = %zu, feof(finp) = %d\n", len, feof(finp));
        fprintf(fout, "<%.*s>\n", (int) len, buffer);

        // Eval
        const NextState ns = advance_state_with_input(
            alloc,
            ferr,
            repl,
            (StrView) {
                .buffer = (const byte *) buffer,
                .len = len,
            }
        );

        // Print
        // TODO

        // Loop
        repl = ns.as.ok.repl;
    }

    return 0;
}
