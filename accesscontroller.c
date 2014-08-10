/**
  Access Controller Module for Secure OS based on OSv/CRuby

Author : alphaKAI
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MI_MAX_ARRAY_SIZE 100
static struct method_information *mi_array[MI_MAX_ARRAY_SIZE];

/* Prototype defines */
static bool mi_is_euqal(struct method_information *, struct method_information *);
static bool  mi_is_null(struct method_information *);
static void mi_clear(struct method_information *);
static struct method_information *search_mi_element(struct method_information *);
static void insert_mi_element(struct method_information *);
static void remove_mi_element(struct method_information *);

int mi_array_idx = 0; 

bool mi_is_equal(struct method_information *a, struct method_information *b){

  if(strcmp(a->classname, b->classname) == 0 &&
      strcmp(a->methodname, b->methodname) == 0){
    return true;
  }

  return false;
}

bool mi_is_null(struct method_information *mi){

  if(mi->classname == NULL
      && mi->methodname == NULL) {
    return true;
  }

  return false;
}

void mi_clear(struct method_information *mi) {
  mi->classname = NULL;
  mi->methodname = NULL;
}

struct method_information * search_mi_element(struct method_information *mi) {
  int i;

  for(i = 0; i < MI_MAX_ARRAY_SIZE; i++){
    if(mi_array[i] != NULL){
#ifdef ACCESSCONTROLLER_DEBUG
      fprintf(stderr, "[COMPARE] class  %s - %s\n", mi_array[i]->classname, mi->classname);
      fprintf(stderr, "[COMPARE] method %s - %s\n", mi_array[i]->methodname, mi->methodname);
#endif
      if(mi_is_equal(mi_array[i], mi)){
#ifdef ACCESSCONTROLLER_DEBUG
        fprintf(stderr, "[COMPARE] match!!!!!!!!!!!\n");
#endif
        return mi_array[i];
      }
#ifdef ACCESSCONTROLLER_DEBUG
      fprintf(stderr, "[COMPARE] not match\n");
#endif
    }
  }

  return NULL;
}

void insert_mi_element(struct method_information *mi) {
  int i;
  bool flag = false;

#ifdef ACCESSCONTROLLER_DEBUG
  fprintf(stderr, "[insert_mi_element] first step then\n");
#endif
  if(search_mi_element(mi) != NULL || mi_is_null(mi)){
    fprintf(stderr, "Alrady exists\n");
    return;
  }
#ifdef ACCESSCONTROLLER_DEBUG
  fprintf(stderr, "[insert_mi_element] first step end\n");
  
  fprintf(stderr, "[insert_mi_element] seound step then\n");
#endif
  
  for(i = mi_array_idx; i < MI_MAX_ARRAY_SIZE; i++){
    if(mi_array[i] == NULL){
      flag = true;
      break;
    }
  }

  if(flag == false) {
    fprintf(stderr, "[Error] : Can not insert rule into array; array size is 100(MI_MAX)\n");
    rb_fatal("Can't insert more rule; array is already full");
    return;
  }

#ifdef ACCESSCONTROLLER_DEBUG
  fprintf(stderr, "[insert_mi_element] thuard step end\n");

  fprintf(stderr, "[insert_mi_element] for loop then\n");
#endif
  for(; mi_array_idx < MI_MAX_ARRAY_SIZE; mi_array_idx++){
    if(mi_array[mi_array_idx] == NULL){
#ifdef ACCESSCONTROLLER_DEBUG
      fprintf(stderr, "Will Allocate\n");
#endif
      mi_array[mi_array_idx] = (struct method_information*)malloc(sizeof(struct method_information*));
#ifdef ACCESSCONTROLLER_DEBUG
      fprintf(stderr, "Complate Allocate\n");

      fprintf(stderr, "Will Pull\n");
#endif
      mi_array[mi_array_idx] = mi;
#ifdef ACCESSCONTROLLER_DEBUG
      fprintf(stderr, "Ok\n");
#endif
      break;
    }
  }

#ifdef ACCESSCONTROLLER_DEBUG
  fprintf(stderr, "[insert_mi_element] for loop end\n");
#endif
}

void remove_mi_element(struct method_information *mi) {
  int i;

  struct method_information *tmp;
  tmp = search_mi_element(mi);
  if(tmp != NULL){
    free(tmp);
    mi_array_idx--;
  } else if(tmp == NULL){
    fprintf(stderr, "Not Found\n");
  }
}

void get_method_info(struct method_information *mi, rb_thread_t *th, VALUE klazz, ID id) {
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

void show_method_info(struct method_information *mi) {
    fprintf(stderr, "class : %s, method : %s\n",
        mi->classname, mi->methodname);
}

void access_granted(struct method_information *mi) {
#ifdef ACCESSCONTROLLER_DEBUG
  fprintf(stderr, "[ACCESS CONTROLLER] THEN\n");
#endif
  if(search_mi_element(mi) != NULL){
    fprintf(stderr, "ERROR\n");
    fprintf(stderr, "This method is not available\n");
    rb_raise(rb_eRuntimeError, "This method is not available");
    exit(EXIT_FAILURE);
  }
#ifdef ACCESSCONTROLLER_DEBUG
  fprintf(stderr, "[ACCESS CONTROLLER] END\n");
#endif
}

void dump_mi_array(){
  int i;

#ifdef ACCESSCONTROLLER_DEBUG
  fprintf(stderr, "[DEBUG][ARRAY DUMP]\n");
  for(i = 0; i < MI_MAX_ARRAY_SIZE; i++){
    if(mi_array[i] != NULL){
      fprintf(stderr, "mi_array[%d]:\n", i);
      show_method_info(mi_array[i]);
    }
  }
  
  fprintf(stderr, "======================\n");
#endif
}

void pre_set_rule(){
  struct method_information *tmp_mi;
  tmp_mi = (struct method_information*)malloc(sizeof(struct method_information));

  tmp_mi->classname = "BlackList";
  tmp_mi->methodname = "blackFunction";

  insert_mi_element(tmp_mi);
}
