/**
  Access Controller Module for Secure OS based on OSv/CRuby

Author : alphaKAI
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "ruby/ruby.h"
#include "ruby/vm.h"
#include "ruby/st.h"
#include "ruby/encoding.h"
#include "internal.h"

#include "gc.h"
/* XXX:Need to fix. */
#define NSIG 32
#include "vm_core.h"
#include "iseq.h"
#include "eval_intern.h"
#include "probes.h"
#include "probes_helper.h"

#include "require_access.h"

#ifdef REQUIRE_DEBUG
#define DEBUG0(msg) fprintf(stderr, msg)
#define DEBUG1(msg, arg1) fprintf(stderr, msg, arg1)
#define DEBUG2(msg, arg1, arg2) fprintf(stderr, msg, arg1, arg2)
#else
#define DEBUG0(msg) 
#define DEBUG1(msg, arg1)
#define DEBUG2(msg, arg1, arg2)
#endif

#define ARRAY_SIZE 100
static struct require_information *rq_array[ARRAY_SIZE] = {};
static int    rq_blacklist = 1;

/* Prototype defines */
static int rq_is_equal(struct require_information *, struct require_information *);
static struct require_information *search_rq_element(struct require_information *);
/* static void dump_rq_array(); */

int
rq_is_equal(struct require_information *a, struct require_information *b)
{
  if(a == NULL || b == NULL ||
       a->filename == NULL || b->filename == NULL)
    return -1;

  if (strcmp(a->filename, b->filename) == 0){
    return 0;
  }

  return -1;
}

struct require_information*
search_rq_element(struct require_information *rq)
{
  int i;

  for (i = 0; i < ARRAY_SIZE; i++) {
    if(rq_array[i] != NULL){
      DEBUG2("[REQURIE QCOMPARE] filename  %s - %s\n", rq_array[i]->filename, rq->filename);
    }

    if (!rq_is_equal(rq_array[i], rq)) {
      return rq_array[i];
    } 
   
  }

  return NULL;
}

int
insert_rq_element(const char *filename)
{
  int i;

  for (i = 0; i < ARRAY_SIZE; i++) {
    if (rq_array[i] == NULL) {
      rq_array[i] = (struct require_information*)malloc(sizeof(struct require_information*));
      rq_array[i]->filename  = filename;

      return 0;
    }
  }
  DEBUG0("[Error] : Can not insert rule into array; array size is 100(MAX ARRAY SIZE)\n");
  return -1;
}

void
show_require_info(struct require_information *rq)
{
    fprintf(stderr, "filename : %s\n", rq->filename);
}

int
require_granted(struct require_information *rq)
{
  struct require_information *matched;

  matched = search_rq_element(rq);
  DEBUG0("[REQUIRE COMPARE]\n");
#ifdef REQUIRE_DEBUG
  show_require_info(rq);
#endif

  if (matched != NULL) {
    DEBUG0(rq_blacklist ? "ACCESS REJECT\n" : "ACCESS GRANTED\n");
    return rq_blacklist ? -1 : 0;
  } else {
    DEBUG0(rq_blacklist ? "ACCESS GRANTED\n" : "ACCESS REJECT\n");
    return rq_blacklist ?  0 : -1;
  }
}

/*
void
dump_rq_array()
{
  int i;

#ifdef REQUIRE_DEBUG
  fprintf(stderr, "[DEBUG][ARRAY DUMP]\n");
  for (i = 0; i < ARRAY_SIZE; i++) {
    if (rq_array[i] != NULL) {
      fprintf(stderr, "rq_array[%d]:\n", i);
      show_require_info(rq_array[i]);
    }
  }

  fprintf(stderr, "======================\n");
#endif
}
*/
