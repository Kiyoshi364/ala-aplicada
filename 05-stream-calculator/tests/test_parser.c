#include <stdio.h>

#define LEXER_IMPLEMENTATION
#include "../parser/lexer.h"

#include "test.h"

typedef struct {
    const char *input;
    const struct Token {
        TokenType type;
        const char *text;
    } tokens[32];
} Example;

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
    const Example examples[] = {
        {
            .input = "1 2 _3_ ; add mul $12 \n",
            .tokens = {
                { TK_NUMBER, "1" },
                { TK_NUMBER, "2" },
                { TK_NUMBER, "_3_" },
                { TK_SEMI_COLON, ";" },
                { TK_KW_ADD, "add" },
                { TK_KW_MUL, "mul" },
                { TK_REGISTER, "$12" },
                { TK_NEW_LINE, "\n" },
                { TK_EOF, "" },
            },
        }, {
            .input = "!@#$0%&*-+=()[]{},.<>:~^/\\|?",
            .tokens = {
                { TK_PUNCTIUATION, "!" },
                { TK_PUNCTIUATION, "@" },
                { TK_BEGIN_COMMENT, "#" },
                { TK_REGISTER, "$0" },
                { TK_KW_INV, "%" },
                { TK_PUNCTIUATION, "&" },
                { TK_KW_MUL, "*" },
                { TK_PUNCTIUATION, "-" },
                { TK_KW_ADD, "+" },
                { TK_PUNCTIUATION, "=" },
                { TK_PUNCTIUATION, "(" },
                { TK_PUNCTIUATION, ")" },
                { TK_PUNCTIUATION, "[" },
                { TK_PUNCTIUATION, "]" },
                { TK_PUNCTIUATION, "{" },
                { TK_PUNCTIUATION, "}" },
                { TK_PUNCTIUATION, "," },
                { TK_PUNCTIUATION, "." },
                { TK_PUNCTIUATION, "<" },
                { TK_PUNCTIUATION, ">" },
                { TK_PUNCTIUATION, ":" },
                { TK_PUNCTIUATION, "~" },
                { TK_PUNCTIUATION, "^" },
                { TK_PUNCTIUATION, "/" },
                { TK_PUNCTIUATION, "\\" },
                { TK_PUNCTIUATION, "|" },
                { TK_PUNCTIUATION, "?" },
                { TK_EOF, "" },
            },
        }, {
            .input = "$_\t$0_0\r\r$123___4",
            .tokens = {
                { TK_REGISTER, "$_" },
                { TK_REGISTER, "$0_0" },
                { TK_REGISTER, "$123___4" },
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
            if (token.type == TK_EOF) {
                break;
            }
        }
    }

    return errs;
}

int test_all(TestCtx ctx) {
    int errs = 0;

    errs += test_lexer_examples(ctx);
    (void) first_err;

    return errs;
}

#include "main_test.h"
