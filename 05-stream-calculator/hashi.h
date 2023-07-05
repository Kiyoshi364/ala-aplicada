#ifndef _HASHI_H
#define _HASHI_H

#include <assert.h>
#include <inttypes.h>
#include <stdalign.h>
#include <stddef.h>
#include <stdlib.h>

// Helpers

// Compiletime assert

#define CASSERT3(pred, name, line) \
  typedef char assertion_failed_##name##_##line [2*(!!(pred))-1]
#define CASSERT2(pred, name, line) CASSERT3(pred, name, line)
#define CASSERT(pred, name) CASSERT2(pred, name, __LINE__)

#define TODO(...) CASSERT(0, TODO)
#define RUNTIME_TODO(...) assert(0 && "TODO")

// int types
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef uintptr_t uword;

#define UWORD_MAX UINTPTR_MAX

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef intptr_t iword;

// float types

typedef double fword;
#define PRIfPTR "lf"

// Allocator

typedef struct{ u8 dummy; } Alloc;
#define MAKE_ALLOC(size) ((void) size, (Alloc){ .dummy = 0, })
#define ALLOCATE_ONE(alloc, typ) ((void) alloc, (typ *) malloc(sizeof(typ)))
#define ALLOCATE_MANY(alloc, typ, cnt) ((void) alloc, (typ [cnt]) malloc(sizeof(typ[cnt])))
#define FREE(alloc, typ, ptr) ((void) alloc, free(ptr))

#endif // _HASHI_H
