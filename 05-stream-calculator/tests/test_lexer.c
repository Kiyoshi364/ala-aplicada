#include <stdio.h>

#define LEXER_IMPLEMENTATION
#include "../lexer.h"

#include "test.h"

typedef struct {
    const char *input;
    const struct Token {
        TokenType type;
        const char *text;
    } tokens[32];
} LexerExample;

typedef union {
    fword f;
    uword u;
} fuword;

typedef struct {
    const char *input;
    const fuword number;
} ReadNumberExample;

typedef union {
    iword i;
    uword u;
} iuword;

typedef struct {
    const char *input;
    const iuword number;
} ReadInumberExample;

int eq_str_view(const char *str, const StrView view) {
    size_t i = 0;
    for (; str[i] != '\0' && i < view.len; i += 1) {
        if (str[i] != view.buffer[i]) {
            break;
        }
    }
    return (str[i] == '\0') && (i == view.len);
}

int test_lexer_examples(TestCtx ctx) {
    int errs = 0;
    const LexerExample examples[] = {
        {
            .input = "1 2 _3_ ; add sub mul $12 x tail\n",
            .tokens = {
                { TK_NUMBER, "1" },
                { TK_NUMBER, "2" },
                { TK_NUMBER, "_3_" },
                { TK_SEMI_COLON, ";" },
                { TK_KW_ADD, "add" },
                { TK_KW_SUB, "sub" },
                { TK_KW_MUL, "mul" },
                { TK_LOAD_REGISTER, "$12" },
                { TK_KW_X, "x" },
                { TK_KW_TAIL, "tail" },
                { TK_NEW_LINE, "\n" },
                { TK_EOF, "" },
            },
        }, {
            .input = "x'!0@#$0%&*-+=()[]{},.<>:~^/\\|?",
            .tokens = {
                { TK_KW_X, "x" },
                { TK_KW_TAIL, "'" },
                { TK_STORE_REGISTER, "!0" },
                { TK_PUNCTUATION, "@" },
                { TK_BEGIN_COMMENT, "#" },
                { TK_LOAD_REGISTER, "$0" },
                { TK_KW_INV, "%" },
                { TK_PUNCTUATION, "&" },
                { TK_KW_MUL, "*" },
                { TK_KW_SUB, "-" },
                { TK_KW_ADD, "+" },
                { TK_PUNCTUATION, "=" },
                { TK_PUNCTUATION, "(" },
                { TK_PUNCTUATION, ")" },
                { TK_PUNCTUATION, "[" },
                { TK_PUNCTUATION, "]" },
                { TK_PUNCTUATION, "{" },
                { TK_PUNCTUATION, "}" },
                { TK_PUNCTUATION, "," },
                { TK_PUNCTUATION, "." },
                { TK_PUNCTUATION, "<" },
                { TK_PUNCTUATION, ">" },
                { TK_PUNCTUATION, ":" },
                { TK_PUNCTUATION, "~" },
                { TK_PUNCTUATION, "^" },
                { TK_PUNCTUATION, "/" },
                { TK_PUNCTUATION, "\\" },
                { TK_PUNCTUATION, "|" },
                { TK_PUNCTUATION, "?" },
                { TK_EOF, "" },
            },
        }, {
            .input = "$_\t$0_0\r\x1b$123___4\x1b$_3",
            .tokens = {
                { TK_LOAD_REGISTER, "$_" },
                { TK_LOAD_REGISTER, "$0_0" },
                { TK_LOAD_REGISTER, "$123___4" },
                { TK_LOAD_REGISTER, "$_3" },
                { TK_EOF, "" },
            },
        }, {
            .input = "!_\t!0_0\r\x1b!123___4\x1b!_3",
            .tokens = {
                { TK_STORE_REGISTER, "!_" },
                { TK_STORE_REGISTER, "!0_0" },
                { TK_STORE_REGISTER, "!123___4" },
                { TK_STORE_REGISTER, "!_3" },
                { TK_EOF, "" },
            },
        }, {
            .input = "asdf_asdf # _asdf",
            .tokens = {
                { TK_ID, "asdf_asdf" },
                { TK_BEGIN_COMMENT, "#" },
                { TK_NUMBER, "_" },
                { TK_ID, "asdf" },
                { TK_EOF, "" },
            },
        }, {
            .input = "1234 56.78 _9_.___0_ 3. __ a_",
            .tokens = {
                { TK_NUMBER, "1234" },
                { TK_NUMBER, "56.78" },
                { TK_NUMBER, "_9_.___0_" },
                { TK_NUMBER, "3." },
                { TK_NUMBER, "__" },
                { TK_ID, "a_" },
                { TK_EOF, "" },
            },
        }
    };

    for (size_t i = 0; i < ARRAY_SIZE(examples); i += 1) {
        const StrView input = from_str_view(examples[i].input);
        LexerState lex = new_lexer(input);
        const struct Token *tokens = examples[i].tokens;
        for (size_t j = 0; ARRAY_SIZE(examples[i].tokens); j += 1) {
            const NextToken nt = next_token(lex);
            const Token token = nt.token;
            TEST (tokens[j].type == token.type
                && eq_str_view(tokens[j].text, token.text)
            ) {
                errs += 1;
                fprintf(ctx.out,
                    "(%zu) Expected token is '%s' (%s),"
                    " but got '%"PRIVIEW"' (%s)\n",
                    j,
                    tokens[j].text, TK_NAME(tokens[j].type),
                    STRVIEW(token.text), TK_NAME(token.type));
            }
            lex = nt.state;
            if (tokens[j].type == TK_EOF || token.type == TK_EOF) {
                break;
            }
        }
    }

    return errs;
}

int test_read_number(TestCtx ctx) {
    int errs = 0;
    const ReadNumberExample examples[] = {
        {
            .input = "1",
            .number = { .f = 1.0L },
        }, {
            .input = "2",
            .number = { .f = 2.0L },
        }, {
            .input = "_3_",
            .number = { .f = -3.0L },
        }, {
            .input = "0",
            .number = { .f = 0.0L },
        }, {
            .input = "_",
            .number = { .f = -0.0L },
        }, {
            .input = "__",
            .number = { .f = -0.0L },
        }, {
            .input = "0_0",
            .number = { .f = 0.0L },
        }, {
            .input = "123___4",
            .number = { .f = 1234.0L },
        }, {
            .input = "56.78",
            .number = { .f = 56.78L },
        }, {
            .input = "_9_.___0_",
            .number = { .f = -9.0L },
        }, {
            .input = "3.",
            .number = { .f = 3.0L },
        }, {
            .input = "0.123456789",
            .number = { .f = 0.123456789L },
        }
    };

    for (size_t i = 0; i < ARRAY_SIZE(examples); i += 1) {
        const StrView input = from_str_view(examples[i].input);
        const LexerState lex = new_lexer(input);
        const NextToken nt = next_token(lex);
        const Token token = nt.token;
        const fuword number = {
            .f = read_number(token.text),
        };
        TEST (token.text.len == input.len
            && token.text.buffer == input.buffer
            && token.type == TK_NUMBER
        ) {
            errs += 1;
            fprintf(ctx.out,
                "(%zu) Expected '%"PRIVIEW"' to be lexed as %s"
                " but got lexed as '%"PRIVIEW"' (%s)\n",
                i,
                STRVIEW(input), TK_NAME(TK_NUMBER),
                STRVIEW(token.text), TK_NAME(token.type));
        } else {
            const NextToken nt_eof = next_token(nt.state);
            assert(nt_eof.token.type == TK_EOF);
        }
        TEST (number.f - examples[i].number.f < EPSILON) {
            errs += 1;
            fprintf(ctx.out,
                "(%zu) Expected number to be is %"PRIfPTR" (0x%016"PRIXPTR"),"
                " but got %"PRIfPTR" (0x%016"PRIXPTR"),"
                " with input '%"PRIVIEW"'\n",
                i,
                examples[i].number.f, examples[i].number.u,
                number.f, number.u,
                STRVIEW(input));
        }
    }

    return errs;
}

int test_read_inumber(TestCtx ctx) {
    int errs = 0;
    const ReadInumberExample examples[] = {
        {
            .input = "1",
            .number = { .i = 1 },
        }, {
            .input = "2",
            .number = { .i = 2 },
        }, {
            .input = "_3_",
            .number = { .i = -3 },
        }, {
            .input = "0",
            .number = { .i = 0 },
        }, {
            .input = "_",
            .number = { .i = -0 },
        }, {
            .input = "__",
            .number = { .i = -0 },
        }, {
            .input = "0_0",
            .number = { .i = 0 },
        }, {
            .input = "123___4",
            .number = { .i = 1234 },
        }, {
            .input = "56_78",
            .number = { .i = 5678 },
        }, {
            .input = "_9___0_",
            .number = { .i = -90 },
        }, {
            .input = "3",
            .number = { .i = 3 },
        }, {
            .input = "123_456_789",
            .number = { .i = 123456789L },
        }
    };

    for (size_t i = 0; i < ARRAY_SIZE(examples); i += 1) {
        const StrView input = from_str_view(examples[i].input);
        const LexerState lex = new_lexer(input);
        const NextToken nt = next_token(lex);
        const Token token = nt.token;
        const iuword number = {
            .i = read_inumber(token.text),
        };
        TEST (token.text.len == input.len
            && token.text.buffer == input.buffer
            && token.type == TK_NUMBER
        ) {
            errs += 1;
            fprintf(ctx.out,
                "(%zu) Expected '%"PRIVIEW"' to be lexed as %s"
                " but got lexed as '%"PRIVIEW"' (%s)\n",
                i,
                STRVIEW(input), TK_NAME(TK_NUMBER),
                STRVIEW(token.text), TK_NAME(token.type));
        } else {
            const NextToken nt_eof = next_token(nt.state);
            assert(nt_eof.token.type == TK_EOF);
        }
        TEST (number.i == examples[i].number.i) {
            errs += 1;
            fprintf(ctx.out,
                "(%zu) Expected number to be is %"PRIdPTR" (0x%016"PRIXPTR"),"
                " but got %"PRIdPTR" (0x%016"PRIXPTR"),"
                " with input '%"PRIVIEW"'\n",
                i,
                examples[i].number.i, examples[i].number.u,
                number.i, number.u,
                STRVIEW(input));
        }
    }

    return errs;
}

int test_all(TestCtx ctx) {
    int errs = 0;

    RUN_TEST(test_lexer_examples, ctx, errs);
    RUN_TEST(test_read_number, ctx, errs);
    RUN_TEST(test_read_inumber, ctx, errs);
    (void) first_err;

    return errs;
}

#include "main_test.h"
