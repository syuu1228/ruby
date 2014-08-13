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

#include "method_access.h"

#ifdef METHOD_DEBUG
#define DEBUG0(msg) fprintf(stderr, msg)
#define DEBUG1(msg, arg1) fprintf(stderr, msg, arg1)
#define DEBUG2(msg, arg1, arg2) fprintf(stderr, msg, arg1, arg2)
#else
#define DEBUG0(msg) 
#define DEBUG1(msg, arg1)
#define DEBUG2(msg, arg1, arg2)
#endif

#define ARRAY_SIZE 100
static struct method_information *mi_array[ARRAY_SIZE] = {};
static int    mi_blacklist = 1;

/* Prototype defines */
static int mi_is_equal(struct method_information *, struct method_information *);
static struct method_information *search_mi_element(struct method_information *);
/* static void dump_mi_array(); */

int
mi_is_equal(struct method_information *a, struct method_information *b)
{
  if(a == NULL || b == NULL ||
       a->classname == NULL || a->methodname == NULL ||
       b->classname == NULL || b->methodname == NULL)
    return -1;

  if (strcmp(a->classname, b->classname) == 0 &&
      strcmp(a->methodname, b->methodname) == 0) {
    return 0;
  }

  return -1;
}

struct method_information*
search_mi_element(struct method_information *mi)
{
  int i;

  for (i = 0; i < ARRAY_SIZE; i++) {
    if(mi_array[i] != NULL){
      DEBUG2("[COMPARE] class  %s - %s\n", mi_array[i]->classname, mi->classname);
      DEBUG2("[COMPARE] method %s - %s\n", mi_array[i]->methodname, mi->methodname);
    }

    if (!mi_is_equal(mi_array[i], mi)) {
      return mi_array[i];
    } 
   
  }

  return NULL;
}

int
insert_mi_element(const char *classname, const char *methodname)
{
  int i;

  for (i = 0; i < ARRAY_SIZE; i++) {
    if (mi_array[i] == NULL) {
      mi_array[i] = (struct method_information*)malloc(sizeof(struct method_information*));
      mi_array[i]->classname  = classname;
      mi_array[i]->methodname = methodname;

      return 0;
    }
  }
  DEBUG0("[Error] : Can not insert rule into array; array size is 100(MI_MAX)\n");
  return -1;
}

void
get_method_info(struct method_information *mi, rb_thread_t *th, VALUE klazz, ID id)
{
  const char *classname  = NULL;
  const char *methodname = NULL;
 /* const char *filename; */

  if (!klazz) {
    rb_thread_method_id_and_class((th), &id, &klazz);
  }
  if (klazz) {
    if (RB_TYPE_P(klazz, T_ICLASS)) {
      klazz = RBASIC(klazz)->klass;
    }
    else if (FL_TEST(klazz, FL_SINGLETON)) {
      klazz = rb_iv_get(klazz, "__attached__");
    }
    switch (TYPE(klazz)) {
      case T_CLASS:
      case T_ICLASS:
      case T_MODULE:
        {
          VALUE name = rb_class_path_no_cache(klazz);
          if (!NIL_P(name)) {
            classname = StringValuePtr(name);
          }
          else {
            classname = "<unknown>";
          }
          methodname = rb_id2name(id);
         /* filename   = rb_sourcefile(); */

          break;
        }
    }
  }

  mi->classname  = classname;
  mi->methodname = methodname;
}

void
show_method_info(struct method_information *mi)
{
    fprintf(stderr, "class : %s, method : %s\n",
        mi->classname, mi->methodname);
}

int
method_granted(struct method_information *mi)
{
  struct method_information *matched;

  matched = search_mi_element(mi);
  DEBUG0("[COMPARE]");
#ifdef METHOD_DEBUG
  show_method_info(mi);
#endif

  if (matched != NULL) {
    DEBUG0(mi_blacklist ? "ACCESS REJECT\n" : "ACCESS GRANTED\n");
    return mi_blacklist ? -1 : 0;
  } else {
    DEBUG0(mi_blacklist ? "ACCESS GRANTED\n" : "ACCESS REJECT\n");
    return mi_blacklist ?  0 : -1;
  }
}

/*
voi
dump_mi_array()
{
  int i;

#ifdef METHOD_DEBUG
  fprintf(stderr, "[DEBUG][ARRAY DUMP]\n");
  for (i = 0; i < ARRAY_SIZE; i++) {
    if (mi_array[i] != NULL) {
      fprintf(stderr, "mi_array[%d]:\n", i);
      show_method_info(mi_array[i]);
    }
  }

  fprintf(stderr, "======================\n");
#endif
}
*/
