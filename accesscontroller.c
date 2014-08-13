/**
  Access Controller Module for Secure OS based on OSv/CRuby

Author : alphaKAI
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

<<<<<<< HEAD
#define ARRAY_SIZE 100
static struct method_information *mi_array[ARRAY_SIZE] = {};

/* Prototype defines */
static bool mi_is_euqal(struct method_information *, struct method_information *);
static struct method_information *search_mi_element(struct method_information *);
static void dump_mi_array();

bool
mi_is_equal(struct method_information *a, struct method_information *b)
{

=======
#include "ruby/ruby.h"
#include "ruby/vm.h"
#include "ruby/st.h"
#include "ruby/encoding.h"
#include "internal.h"

#include "gc.h"
#include "vm_core.h"
#include "iseq.h"
#include "eval_intern.h"
#include "probes.h"
#include "probes_helper.h"

#include "accesscontroller.h"

#define ARRAY_SIZE 100
static struct method_information *mi_array[ARRAY_SIZE] = {};

/* Prototype defines */
static bool mi_is_euqal(struct method_information *, struct method_information *);
static struct method_information *search_mi_element(struct method_information *);
static void dump_mi_array();

bool
mi_is_equal(struct method_information *a, struct method_information *b)
{

>>>>>>> 32ee6b3025dd0ee6a64aed97fe7e1533a0b6f570
  if (strcmp(a->classname, b->classname) == 0 &&
      strcmp(a->methodname, b->methodname) == 0) {
    return true;
  }

  return false;
}

struct method_information*
search_mi_element(struct method_information *mi)
{
  int i;

  for (i = 0; i < ARRAY_SIZE; i++) {
#ifdef ACCESSCONTROLLER_DEBUG
    if (mi_array[i] != NULL) {
      fprintf(stderr, "[COMPARE] class  %s - %s\n", mi_array[i]->classname, mi->classname);
      fprintf(stderr, "[COMPARE] method %s - %s\n", mi_array[i]->methodname, mi->methodname);

      if(mi_is_equal(mi_array[i], mi)){
        fprintf(stderr, "[COMPARE] match!!!!!!!!!!!\n");
        return mi_array[i];
      }
      fprintf(stderr, "[COMPARE] not match\n");
    }
#else
  if (mi_array[i] != NULL)
    if(mi_is_equal(mi_array[i], mi))
      return mi_array[i];
#endif
  }

  return NULL;
}

void
insert_mi_element(const char *classname, const char *methodname)
{
  int i;
  bool flag = false;
  int offset;

  for (i = 0; i < ARRAY_SIZE; i++) {
    if (mi_array[i] == NULL) {
      flag = true;
      offset = i;
      break;
    }
  }

  if (flag == false) {
    fprintf(stderr, "[Error] : Can not insert rule into array; array size is 100(MI_MAX)\n");
    rb_fatal("Can't insert more rule; array is already full");
    return;
  }

  mi_array[offset] = (struct method_information*)calloc(1, sizeof(struct method_information*));

  if (mi_array[offset] == NULL) {
    fprintf(stderr, "[Error] : Failure to allocate memory\n");
    rb_fatal("Failure to allocate memory");
    return;
  }

  mi_array[offset]->classname  = classname;
  mi_array[offset]->methodname = methodname;
}

void
get_method_info(struct method_information *mi, rb_thread_t *th, VALUE klazz, ID id)
{
  const char *classname;
  const char *methodname;
  const char *filename;

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
          filename   = rb_sourcefile();

          break;
        }
    }
  }

  mi->classname = classname;
  mi->methodname = methodname;
}

void
show_method_info(struct method_information *mi)
{
    fprintf(stderr, "class : %s, method : %s\n",
        mi->classname, mi->methodname);
}

int
access_granted(struct method_information *mi)
{

  if (search_mi_element(mi) != NULL) {
    fprintf(stderr, "ERROR\n");
    fprintf(stderr, "This method is not available\n");
    show_method_info(mi);
    return 0;
  }

  return 1;
}

void
dump_mi_array()
{
  int i;

#ifdef ACCESSCONTROLLER_DEBUG
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

