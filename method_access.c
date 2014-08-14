/**
  Access Controller Module for Secure OS based on OSv/CRuby

Author : alphaKAI
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef METHOD_DEBUG
#define DEBUG0(msg) fprintf(stderr, msg)
#define DEBUG1(msg, arg1) fprintf(stderr, msg, arg1)
#define DEBUG2(msg, arg1, arg2) fprintf(stderr, msg, arg1, arg2)
#else
#define DEBUG0(msg) 
#define DEBUG1(msg, arg1)
#define DEBUG2(msg, arg1, arg2)
#endif

/* Prototype defines */
static int mi_is_equal(struct method_information *, struct method_information *);
static struct method_information *search_mi_element(struct mi_ruleset*, struct method_information *);
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
search_mi_element(struct mi_ruleset *ruleset, struct method_information *mi)
{
  int i;

  for (i = 0; i < ruleset->rule_len; i++) {
    DEBUG2("[COMPARE] class  %s - %s\n", ruleset->rules[i].classname, mi->classname);
    DEBUG2("[COMPARE] method %s - %s\n", ruleset->rules[i].methodname, mi->methodname);

    if (!mi_is_equal(&ruleset->rules[i], mi)) {
      return &ruleset->rules[i];
    } 
   
  }

  return NULL;
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
method_granted(int id, struct method_information *mi)
{
  struct mi_ruleset *ruleset;
  struct method_information *matched;

  if (id == -1)
    return 0;
  if (id >= mi_rulesets_len)
    return -1;
  ruleset = &mi_rulesets[id];

  matched = search_mi_element(ruleset, mi);
  DEBUG0("[COMPARE]");
#ifdef METHOD_DEBUG
  show_method_info(mi);
#endif
  DEBUG1("[COMPARE] matched = %p\n", matched);

  if (matched != NULL) {
    DEBUG0(ruleset->blacklist ? "ACCESS REJECT\n" : "ACCESS GRANTED\n");
    return ruleset->blacklist ? -1 : 0;
  } else {
    DEBUG0(ruleset->blacklist ? "ACCESS GRANTED\n" : "ACCESS REJECT\n");
    return ruleset->blacklist ?  0 : -1;
  }
}

/*
void
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
