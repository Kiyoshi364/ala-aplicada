#ifndef _LEXER_H
#define _LEXER_H

#include "../hashi.h"

typedef enum {
    TK_NUMBER,
    TK_REGISTER,
    TK_KW_ADD,
    TK_KW_MUL,
    TK_KW_INV,
    TK_BEGIN_COMMENT,
    TK_NEW_LINE,
    TK_SEMI_COLON,
    TK_ID,
    TK_PUNCTIUATION,
    TK_EOF,
} TokenType;

typedef struct {
    const char *buffer;
    size_t len;
} StrView;
#define PRIVIEW ".*s"
#define STRVIEW(s) ((int) (s).len), (s).buffer

typedef struct {
    TokenType type;
    StrView text;
} Token;

typedef struct {
    StrView view;
} LexerState;

typedef struct {
    LexerState state;
    Token token;
} NextToken;

#endif // _LEXER_H

#ifdef LEXER_IMPLEMENTATION
#ifndef LEXER_IMPLEMENTED
#define LEXER_IMPLEMENTED

static char *token_to_name[] = {
    "TK_NUMBER",
    "TK_REGISTER",
    "TK_KW_ADD",
    "TK_KW_MUL",
    "TK_KW_INV",
    "TK_BEGIN_COMMENT",
    "TK_NEW_LINE",
    "TK_SEMI_COLON",
    "TK_ID",
    "TK_PUNCTIUATION",
    "TK_EOF",
};
CASSERT(ARRAY_SIZE(token_to_name) == TK_EOF+1, TOKENS_FIT);
#define TK_NAME(t) (token_to_name[(t)])

static struct {
    TokenType token;
    const char *key;
} keywords[] = {
    { TK_KW_ADD, "add" },
    { TK_KW_MUL, "mul" },
    { TK_KW_INV, "inv" },
    { TK_NEW_LINE, "\n" },
};

static struct {
    TokenType type;
    char key;
} punctuations[] = {
    { TK_KW_ADD, '+' },
    { TK_KW_MUL, '*' },
    { TK_KW_INV, '%' },
    { TK_SEMI_COLON, ';' },
    { TK_BEGIN_COMMENT, '#' },
    { TK_NEW_LINE, '\n' },
    // Ignored
    { TK_PUNCTIUATION, '!' },
    { TK_PUNCTIUATION, '@' },
    { TK_PUNCTIUATION, '&' },
    { TK_PUNCTIUATION, '-' },
    { TK_PUNCTIUATION, '=' },
    { TK_PUNCTIUATION, '(' },
    { TK_PUNCTIUATION, ')' },
    { TK_PUNCTIUATION, '[' },
    { TK_PUNCTIUATION, ']' },
    { TK_PUNCTIUATION, '{' },
    { TK_PUNCTIUATION, '}' },
    { TK_PUNCTIUATION, ',' },
    { TK_PUNCTIUATION, '.' },
    { TK_PUNCTIUATION, '<' },
    { TK_PUNCTIUATION, '>' },
    { TK_PUNCTIUATION, ':' },
    { TK_PUNCTIUATION, '~' },
    { TK_PUNCTIUATION, '^' },
    { TK_PUNCTIUATION, '/' },
    { TK_PUNCTIUATION, '\\' },
    { TK_PUNCTIUATION, '|' },
    { TK_PUNCTIUATION, '?' },
};

StrView from_str_view(const char *buffer) {
    size_t len = 0;
    while (buffer[len] != '\0') {
        len += 1;
    }
    return (StrView) {
        .buffer = buffer,
        .len = len,
    };
}

StrView consume_view(const StrView v, const char c) {
    assert(v.len > 0);
    assert(v.buffer[0] == c);
    return (StrView) {
        .buffer = v.buffer + 1,
        .len = v.len - 1,
    };
}

StrView take_view(const StrView v, const size_t count) {
    assert(count <= v.len);
    return (StrView) {
        .buffer = v.buffer,
        .len = count,
    };
}

StrView drop_view(const StrView v, const size_t count) {
    assert(count <= v.len);
    if (count < v.len) {
        return (StrView) {
            .buffer = v.buffer + count,
            .len = v.len - count,
        };
    } else {
        assert(count == v.len);
        return (StrView) {
            .buffer = NULL,
            .len = 0,
        };
    }
}

b8 should_skip(const char c) {
    return c == ' '
        || c == '\t'
        || c == '\r'
    ;
}

b8 starts_number(const char c) {
    return (c == '_') || ('0' <= c && c <= '9');
}

b8 continues_number(const char c) {
    return starts_number(c);
}

b8 starts_register(const char c) {
    return c == '$';
}

size_t is_punctuation(const char c) {
    size_t i = 0;
    for (; i < ARRAY_SIZE(punctuations); i += 1) {
        if (c == punctuations[i].key) {
            break;
        }
    }
    return i;
}

b8 ends_id(const char c) {
    return !(
        ('a' <= c && c <= 'z')
        || ('0' <= c && c <= '9')
        || c == '_'
    );
}

LexerState new_lexer(StrView view) {
    return (LexerState) {
        .view = view,
    };
}

LexerState _consume_lexer(LexerState s, const char c) {
    return (LexerState) {
        .view = consume_view(s.view, c),
    };
}

LexerState trim_left_lexer(LexerState s) {
    size_t i = 0;
    while (i < s.view.len && should_skip(s.view.buffer[i])) {
        i += 1;
    }
    return (LexerState) {
        .view = (StrView) {
            .buffer = s.view.buffer + i,
            .len = s.view.len - i,
        },
    };
}

NextToken next_token_number(const LexerState s) {
    size_t i = 0;
    while (i < s.view.len && continues_number(s.view.buffer[i])) {
        i += 1;
    }
    return (NextToken) {
        .state = (LexerState) {
            .view = drop_view(s.view, i),
        },
        .token = (Token) {
            .type = TK_NUMBER,
            .text = take_view(s.view, i),
        },
    };
}

NextToken next_token_register(const LexerState s) {
    const LexerState rest = _consume_lexer(s, '$');
    const NextToken nt = next_token_number(rest);
    assert(nt.token.text.buffer == s.view.buffer + 1);
    assert(nt.token.type == TK_NUMBER);
    return (NextToken) {
        .state = nt.state,
        .token = (Token) {
            .type = TK_REGISTER,
            .text = (StrView) {
                .buffer = s.view.buffer,
                .len = nt.token.text.len + 1,
            },
        },
    };
}

size_t find_keyword(StrView view) {
    size_t i = 0;
    for (; i < ARRAY_SIZE(keywords); i += 1) {
        b8 ok = 1;
        size_t j = 0;
        for (; ok && keywords[i].key[j]; j += 1) {
            if (view.buffer[j] != keywords[i].key[j]) {
                ok = 0;
            }
        }
        if (ok) {
            break;
        } else {
            continue;
        }
    }
    return i;
}

NextToken next_token_id(const LexerState s) {
    assert(s.view.len > 0);

    size_t i = 0;
    while (i < s.view.len && !ends_id(s.view.buffer[i])) {
        i += 1;
    }
    if (i > 0) {
        const StrView text = take_view(s.view, i);
        const StrView next = drop_view(s.view, i);

        const size_t key_idx = find_keyword(text);
        if (key_idx < ARRAY_SIZE(keywords)) {
            return (NextToken) {
                .state = (LexerState) {
                    .view = next,
                },
                .token = (Token) {
                    .type = keywords[key_idx].token,
                    .text = text,
                },
            };
        } else {
            return (NextToken) {
                .state = (LexerState) {
                    .view = next,
                },
                .token = (Token) {
                    .type = TK_ID,
                    .text = text,
                },
            };
        }
    } else {
        assert(0 && "empty token_id");
        return (NextToken) {0};
    }
}

NextToken next_token(const LexerState s) {
    LexerState trimed = trim_left_lexer(s);
    if (trimed.view.len > 0) {
        const char c = trimed.view.buffer[0];
        size_t index;
        if (starts_number(c)) {
            return next_token_number(trimed);
        } else if (starts_register(c)) {
            return next_token_register(trimed);
        } else if ((index = is_punctuation(c)) < ARRAY_SIZE(punctuations)) {
            return (NextToken) {
                .state = (LexerState) {
                    .view = drop_view(trimed.view, 1)
                },
                .token = (Token) {
                    .type = punctuations[index].type,
                    .text = take_view(trimed.view, 1),
                },
            };
        } else {
            return next_token_id(trimed);
        }
    } else {
        return (NextToken) {
            .state = s,
            .token = (Token) {
                .type = TK_EOF,
                .text = (StrView) {
                    .buffer = NULL,
                    .len = 0,
                },
            },
        };
    }
}

#endif // LEXER_IMPLEMENTED
#endif // LEXER_IMPLEMENTATION
