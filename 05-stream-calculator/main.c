#include <stdio.h>
#include <stdarg.h>

#include "hashi.h"
#define STREAM_IMPLEMENTATION
#include "streams.h"

#define LEXER_IMPLEMENTATION
#include "lexer.h"

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
            buffer[i] = (byte) (c + ('a' - 'A'));
        } else {
            buffer[i] = (byte) c;
        }
        i += 1;
    }
    return i;
}

size_t read(size_t len, byte buffer[len], FILE *stream) {
    return fread(buffer, sizeof(buffer[0]), len, stream);
}

void warning(FILE *stream, const char *format, ...) {
    va_list ap;
    va_start(ap, format);
    fprintf(stream, "WARNING: ");
    vfprintf(stream, format, ap);
    fprintf(stream, "\n");
    va_end(ap);
}

#define REGISTER_COUNT 10
#define STACK_LEN 0x10

typedef struct {
    Alloc alloc;
    const Stream *reg_list[REGISTER_COUNT];
    const Stream *stack[STACK_LEN];
    size_t stack_size;
    b8 skip_newline;
} ReplState;

ReplState push_number_repl(
    FILE *stream,
    const ReplState old_repl,
    const fword number
) {
    ReplState repl = old_repl;
    if (!(repl.stack_size < STACK_LEN)) {
        warning(stream,
            "stack overflow: attempt on push number_stream '%"PRIfPTR"',"
            " ignoring push",
            number
        );
    } else {
        const Stream *n = (const Stream *) make_NumberStream(repl.alloc, number);
        repl.stack[repl.stack_size] = n;
        repl.stack_size += 1;
    }
    return repl;
}

ReplState push_register_repl(
    FILE *stream,
    const ReplState old_repl,
    const size_t reg
) {
    assert(reg < REGISTER_COUNT);
    ReplState repl = old_repl;
    const Stream *r = repl.reg_list[reg];
    if (!(repl.stack_size < STACK_LEN)) {
        warning(stream,
            "stack overflow: attempt on push register '$%zu',"
            " ignoring push",
            reg
        );
    } else {
        if (r == NULL) {
            repl.stack[repl.stack_size] =
                (const Stream *) make_ZeroStream(repl.alloc);
        } else {
            repl.stack[repl.stack_size] = r;
        }
        repl.stack_size += 1;
    }
    return repl;
}

ReplState pop_register_repl(
    FILE *stream,
    const ReplState old_repl,
    const size_t reg
) {
    assert(reg < REGISTER_COUNT);
    ReplState repl = old_repl;
    if (repl.stack_size < 1) {
        warning(stream,
            "stack underflow: attempt on pop to register '$%zu',"
            " ignoring store",
            reg
        );
    } else {
        repl.reg_list[reg] = repl.stack[repl.stack_size-1];
        repl.stack_size -= 1;
    }
    return repl;
}

ReplState shift_by_repl(FILE *stream, const ReplState old_repl, const size_t count) {
    ReplState repl = old_repl;
    if (repl.stack_size < 1) {
        warning(stream,
            "stack underflow: attempt on shifting, but stack size is '%zu',"
            " ignoring shift",
            repl.stack_size
        );
    } else {
        if (count <= 0) {
            warning(stream,
                "count is '%zu' on shifting,"
                " ignoring shift",
                count
            );
        } else {
            const Stream *s1 = repl.stack[repl.stack_size-1];
            repl.stack[repl.stack_size-1] = (const Stream *) make_ShiftStream(repl.alloc, s1, count - 1);
        }
    }
    return repl;
}

ReplState add_repl(FILE *stream, const ReplState old_repl) {
    ReplState repl = old_repl;
    if (repl.stack_size < 2) {
        warning(stream,
            "stack underflow: attempt on adding, but stack size is '%zu',"
            " ignoring add",
            repl.stack_size
        );
    } else {
        const Stream *s1 = repl.stack[repl.stack_size-2];
        const Stream *s2 = repl.stack[repl.stack_size-1];
        repl.stack[repl.stack_size-2] = add_Stream(repl.alloc, s1, s2);
        repl.stack_size -= 1;
    }
    return repl;
}

ReplState sub_repl(FILE *stream, const ReplState old_repl) {
    ReplState repl = old_repl;
    if (repl.stack_size < 2) {
        warning(stream,
            "stack underflow: attempt on subtracting, but stack size is '%zu',"
            " ignoring sub",
            repl.stack_size
        );
    } else {
        const Stream *s1 = repl.stack[repl.stack_size-2];
        const Stream *s2 = repl.stack[repl.stack_size-1];
        repl.stack[repl.stack_size-2] = sub_Stream(repl.alloc, s1, s2);
        repl.stack_size -= 1;
    }
    return repl;
}

ReplState mul_repl(FILE *stream, const ReplState old_repl) {
    ReplState repl = old_repl;
    if (repl.stack_size < 2) {
        warning(stream,
            "stack underflow: attempt on multiplying, but stack size is '%zu',"
            " ignoring mul",
            repl.stack_size
        );
    } else {
        const Stream *s1 = repl.stack[repl.stack_size-2];
        const Stream *s2 = repl.stack[repl.stack_size-1];
        repl.stack[repl.stack_size-2] = mul_Stream(repl.alloc, s1, s2);
        repl.stack_size -= 1;
    }
    return repl;
}

ReplState inv_repl(FILE *stream, const ReplState old_repl) {
    ReplState repl = old_repl;
    if (repl.stack_size < 1) {
        warning(stream,
            "stack underflow: attempt on inverting, but stack size is '%zu',"
            " ignoring inv",
            repl.stack_size
        );
    } else {
        const Stream *s1 = repl.stack[repl.stack_size-1];
        repl.stack[repl.stack_size-1] = inv_Stream(repl.alloc, s1);
    }
    return repl;
}

ReplState tail_repl(FILE *stream, const ReplState old_repl) {
    ReplState repl = old_repl;
    if (repl.stack_size < 1) {
        warning(stream,
            "stack underflow: attempt on tail, but stack size is '%zu',"
            " ignoring tail",
            repl.stack_size
        );
    } else {
        const Stream *s1 = repl.stack[repl.stack_size-1];
        repl.stack[repl.stack_size-1] = tail_Stream(repl.alloc, s1);
    }
    return repl;
}

ReplState push_x_repl(
    FILE *stream,
    const ReplState old_repl
) {
    ReplState repl = old_repl;
    if (!(repl.stack_size < STACK_LEN)) {
        warning(stream,
            "stack overflow: attempt on push 'x',"
            " ignoring push"
        );
    } else {
        const Stream *x = (const Stream *) make_XStream(repl.alloc);
        repl.stack[repl.stack_size] = x;
        repl.stack_size += 1;
    }
    return repl;
}

size_t parse_register(
    FILE *stream,
    const StrView view,
    const char start_char
) {
    assert(view.len > 0 && view.buffer[0] == start_char);
    const iword i_reg = read_inumber(
        drop_view(view, 1)
    );
    size_t reg = (size_t) (i_reg < 0 ? -i_reg : i_reg);
    if (i_reg < 0) {
        warning(stream,
            "register is negative, using '%c%zu' instead",
            start_char, reg
        );
    }
    if (!(reg < REGISTER_COUNT)) {
        reg = REGISTER_COUNT - 1;
        warning(stream,
            "register is out of bounds (%zu), using '%c%zu' instead",
            (size_t) REGISTER_COUNT,
            start_char, reg
        );
    }
    return reg;
}

size_t parse_count(
    FILE *stream,
    const StrView view
) {
    assert(view.len > 0 && view.buffer[0] == '^');
    const iword i_count = read_inumber(
        drop_view(view, 1)
    );
    size_t count = (size_t) (i_count < 0 ? -i_count : i_count);
    if (i_count < 0) {
        warning(stream,
            "count is negative, using '^%zu' instead",
            count
        );
    }
    return count;
}

ReplState eval_repl(
    FILE *stream,
    const ReplState old_repl,
    const StrView input
) {
    b8 found_comment = 0;
    ReplState repl = old_repl;
    NextToken nt = next_token(new_lexer(input));
    while (
        !found_comment
        && nt.token.type != TK_NEW_LINE
        && nt.token.type != TK_EOF
    ) {
        const Token token = nt.token;
        repl.skip_newline = 0;

        fprintf(stream, "%s '%"PRIVIEW"'\n",
            TK_NAME(token.type),
            STRVIEW(token.text)
        );
        switch (token.type) {
            case TK_NUMBER: {
                const fword number = read_number(token.text);
                repl = push_number_repl(stream, repl, number);
            } break;
            case TK_LOAD_REGISTER: {
                const size_t reg =
                    parse_register(stream, token.text, '$');
                repl = push_register_repl(stream, repl, reg);
            } break;
            case TK_STORE_REGISTER: {
                const size_t reg =
                    parse_register(stream, token.text, '!');
                repl = pop_register_repl(stream, repl, reg);
            } break;
            case TK_SHIFT_BY: {
                const size_t count =
                    parse_count(stream, token.text);
                repl = shift_by_repl(stream, repl, count);
            } break;
            case TK_KW_ADD: {
                repl = add_repl(stream, repl);
            } break;
            case TK_KW_SUB: {
                repl = sub_repl(stream, repl);
            } break;
            case TK_KW_MUL: {
                repl = mul_repl(stream, repl);
            } break;
            case TK_KW_INV: {
                repl = inv_repl(stream, repl);
            } break;
            case TK_KW_TAIL: {
                repl = tail_repl(stream, repl);
            } break;
            case TK_KW_X: {
                repl = push_x_repl(stream, repl);
            } break;
            case TK_BEGIN_COMMENT: {
                found_comment = 1;
            } break;
            case TK_SEMI_COLON: {
                repl.skip_newline = 1;
            } break;
            case TK_ID: {
                warning(stream,
                    "ignoring unrecognized command '%"PRIVIEW"'",
                    STRVIEW(token.text));
            } break;
            case TK_PUNCTUATION: {
                assert(token.text.len == 1);
                warning(stream,
                    "ignoring unrecognized symbol '%c'\n",
                    token.text.buffer[0]);
            } break;
            case TK_NEW_LINE:
            case TK_EOF:
            {
                assert(0 && "unreachable");
            } break;
        }
        nt = next_token(nt.state);
    }

    if (!found_comment) {
        if (nt.token.type == TK_NEW_LINE) {
            nt = next_token(nt.state);
        }
        assert(nt.token.type == TK_EOF
            && nt.token.text.len == 0
            && nt.token.text.buffer == NULL
        );
    }
    return repl;
}

void print_repl(
    FILE *stream,
    const ReplState repl,
    const size_t count
) {
    if (repl.skip_newline) {
        return;
    }
    for (size_t i = 0; i < repl.stack_size; i += 1) {
        fprintf(stream, "(@_%zu|@%zu): ",
            i + 1,
            repl.stack_size - 1 - i
        );
        print_N_Streamln(stream, repl.alloc, repl.stack[i], count);
    }
}

int main(void) {
    FILE *finp = stdin;
    FILE *fout = stdout;
    FILE *ferr = stderr;

    static byte buffer[MAX_LINE] = {0};
    size_t len = 1;

    ReplState repl = (ReplState) {
        .alloc = MAKE_ALLOC(0x100),
    };
    while (len > 0) {
        // Read
        len = readln_low(ARRAY_SIZE(buffer), buffer, finp);
        assert(len > 0 || feof(finp));
        fprintf(fout, "D: len = %zu, feof(finp) = %d\n", len, feof(finp));
        fprintf(fout, "<%.*s>\n", (int) len, buffer);

        // Eval
        const ReplState next_repl = eval_repl(
            ferr,
            repl,
            (StrView) {
                .buffer = buffer,
                .len = len,
            }
        );

        // Print
        print_repl(fout, next_repl, 5);

        // Loop
        repl = next_repl;
    }

    return 0;
}
