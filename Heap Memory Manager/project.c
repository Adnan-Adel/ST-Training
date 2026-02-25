#include "Hmm.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static int pass = 0, fail = 0;

#define CHECK(expr, msg)                                                       \
  do {                                                                         \
    if (expr) {                                                                \
      printf("  PASS: %s\n", msg);                                             \
      pass++;                                                                  \
    } else {                                                                   \
      printf("  FAIL: %s\n", msg);                                             \
      fail++;                                                                  \
    }                                                                          \
  } while (0)

void test_basic_alloc_free(void) {
  printf("\n-- test_basic_alloc_free --\n");
  char *p = HmmAlloc(100);
  CHECK(p != NULL, "alloc 100 bytes returns non-NULL");
  strcpy(p, "hello");
  CHECK(strcmp(p, "hello") == 0, "write then read back");
  HmmFree(p);
  CHECK(1, "free did not crash");
}

void test_double_free(void) {
  printf("\n-- test_double_free --\n");
  char *p = HmmAlloc(64);
  CHECK(p != NULL, "alloc 64 bytes");
  HmmFree(p);
  HmmFree(p); /* should be silently ignored */
  CHECK(1, "double free did not crash");
}

void test_null_free(void) {
  printf("\n-- test_null_free --\n");
  HmmFree(NULL);
  CHECK(1, "free(NULL) did not crash");
}

void test_alignment(void) {
  printf("\n-- test_alignment --\n");
  void *p = HmmAlloc(1);
  CHECK(p != NULL, "alloc 1 byte");
  CHECK(((uintptr_t)p % 8) == 0, "pointer is 8-byte aligned");
  HmmFree(p);
  void *q = HmmAlloc(3);
  CHECK(((uintptr_t)q % 8) == 0, "alloc 3 bytes is 8-byte aligned");
  HmmFree(q);
}

void test_multiple_alloc(void) {
  printf("\n-- test_multiple_alloc --\n");
  void *ptrs[20];
  for (int i = 0; i < 20; i++) {
    ptrs[i] = HmmAlloc(128);
    CHECK(ptrs[i] != NULL, "alloc 128 bytes in loop");
  }
  for (int i = 0; i < 20; i++)
    HmmFree(ptrs[i]);
  CHECK(1, "freed all 20 blocks without crash");
}

void test_large_alloc(void) {
  printf("\n-- test_large_alloc --\n");
  /* Larger than one default block – forces heap growth or merging */
  void *p = HmmAlloc(4096);
  CHECK(p != NULL, "alloc 4096 bytes");
  memset(p, 0xAB, 4096);
  unsigned char *cp = (unsigned char *)p;
  int ok = 1;
  for (int i = 0; i < 4096; i++)
    if (cp[i] != 0xAB) {
      ok = 0;
      break;
    }
  CHECK(ok, "4096-byte write/read intact");
  HmmFree(p);
}

void test_calloc(void) {
  printf("\n-- test_calloc --\n");
  int *p = HmmCalloc(10, sizeof(int));
  CHECK(p != NULL, "calloc 10 ints");
  int zeroed = 1;
  for (int i = 0; i < 10; i++)
    if (p[i] != 0) {
      zeroed = 0;
      break;
    }
  CHECK(zeroed, "calloc memory is zero-initialised");
  HmmFree(p);
}

void test_realloc(void) {
  printf("\n-- test_realloc --\n");
  char *p = HmmAlloc(32);
  CHECK(p != NULL, "initial alloc 32");
  strcpy(p, "test_data");
  char *q = HmmRealloc(p, 128);
  CHECK(q != NULL, "realloc to 128 bytes");
  CHECK(strcmp(q, "test_data") == 0, "data preserved after realloc");
  HmmFree(q);

  /* realloc(NULL, n) == malloc(n) */
  char *r = HmmRealloc(NULL, 64);
  CHECK(r != NULL, "realloc(NULL, 64) acts like malloc");
  HmmFree(r);

  /* realloc(ptr, 0) == free(ptr) */
  char *s = HmmAlloc(64);
  void *t = HmmRealloc(s, 0);
  CHECK(t == NULL, "realloc(ptr, 0) returns NULL");
}

void test_no_overlap(void) {
  printf("\n-- test_no_overlap --\n");
  void *a = HmmAlloc(256);
  void *b = HmmAlloc(256);
  CHECK(a != NULL && b != NULL, "two allocs succeed");
  char *ea = (char *)a + 255;
  char *sb = (char *)b;
  CHECK(ea < sb || sb + 255 < (char *)a, "allocations do not overlap");
  HmmFree(a);
  HmmFree(b);
}

int main(void) {
  Hmm_init();

  test_basic_alloc_free();
  test_double_free();
  test_null_free();
  test_alignment();
  test_multiple_alloc();
  test_large_alloc();
  test_calloc();
  test_realloc();
  test_no_overlap();

  printf("\n=== Results: %d passed, %d failed ===\n", pass, fail);
  return fail > 0 ? 1 : 0;
}
