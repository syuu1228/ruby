/**
  Access Controller Module for Secure OS based on OSv/CRuby

Author : alphaKAI
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define true 1
#define false 0

int init_flag = 0;

void init_mi_array(){
  int i;
  if(init_flag == 0) {
    for(i = 0; i < MAX_ARRAY_SIZE; i++) {
      data_clear(&mi_array[i]);
    }

    init_flag = 1;
  }
}

void set_data(struct method_information * to, struct method_information * from){
  *to = *from;
}

int data_is_euqal(struct method_information * a, struct method_information * b){
  int t = false;

  if(a->classname == b->classname &&
      a->methodname == b->methodname) {
    t = true;
  }

  return t;
}

int data_is_null(struct method_information * data){
  int t = false;

  if(data->classname == NULL 
      && data->methodname == NULL) {
    t = true;
  }

  return t;
}

void data_clear(struct method_information * data) {
  data->classname = NULL;
  data->methodname = NULL;
}

int mi_array_has_empty() {
  int i;
  int flag = false;

  for(i = 0; i < MAX_ARRAY_SIZE; i++){
    if(data_is_null(&mi_array[i]) == true){
      flag = true;
      break;
    }
  }

  return flag;
}

int search_mi_element(struct method_information * mi) {
  int i;
  int r = false;
 
  for(i = 0; i < MAX_ARRAY_SIZE; i++){
    if(data_is_euqal(&mi_array[i], mi) == true){
      r = true;
      break;
    }
  }

  return r;
}

void insert_mi_element(struct method_information * mi) {
  int i;

  if(search_mi_element(mi) == true){
    /*fprintf(stderr, "Alrady exists\n"); */
    return;
  }
  
  if(mi_array_has_empty() == false) {
    fprintf(stderr, "[Error] : Can not insert rule into array; array size is 100(MAX)\n");
    rb_raise(rb_eRuntimeError, "Can't insert more rule; array is already full");
    return;
  }

  for(i = 0; i < MAX_ARRAY_SIZE; i++){
    if(data_is_null(&mi_array[i]) == true){
      set_data(&mi_array[i], mi);
      break;
    }
  }
}

void remove_mi_element(struct method_information * mi) {
  int i;

  for(i = 0; i < MAX_ARRAY_SIZE; i++){
    if(data_is_euqal(&mi_array[i], mi) == true){
      data_clear(&mi_array[i]);
      break;
    }
  }

  if(i == MAX_ARRAY_SIZE - 1){
    fprintf(stderr, "Not Found\n");
  }
}

void get_method_info(struct method_information * mi, rb_thread_t * th, VALUE klazz, ID id) {
  const char * classname; 
  const char * methodname; 
  const char * filename;

  do { 
    { 
      VALUE _klass = (klazz); 
      VALUE _id = (id); 
      if (!_klass) { 
        rb_thread_method_id_and_class((th), &_id, &_klass); 
      } 
      if (_klass) { 
        if (RB_TYPE_P(_klass, T_ICLASS)) { 
          _klass = RBASIC(_klass)->klass; 
        } 
        else if (FL_TEST(_klass, FL_SINGLETON)) { 
          _klass = rb_iv_get(_klass, "__attached__"); 
        } 
        switch (TYPE(_klass)) { 
          case T_CLASS: 
          case T_ICLASS: 
          case T_MODULE: 
            { 
              VALUE _name = rb_class_path_no_cache(_klass); 
              if (!NIL_P(_name)) { 
                classname = StringValuePtr(_name); 
              } 
              else {                     
                classname = "<unknown>"; 
              } 
              methodname = rb_id2name(_id); 
              filename   = rb_sourcefile(); 

              break; 
            } 
        } 
      } 
    }
  } while (0);

  mi->classname = classname;
  mi->methodname = methodname;
}

void show_method_info(struct method_information * mi) {
    fprintf(stderr, "class : %s, method : %s\n",
        mi->classname, mi->methodname);
}

void accesscontroller(struct method_information * mi) {
  
  if(search_mi_element(mi) == true){
    fprintf(stderr, "This method is not available");
    rb_raise(rb_eRuntimeError, "This method is not available");
  }
}

void dump_mi_array(){
  int i;

  fprintf(stderr, "[DEBUG][ARRAY DUMP]\n");
  for(i = 0; i < MAX_ARRAY_SIZE; i++){
    if(data_is_null(&mi_array[i]) == 0){
      fprintf(stderr, "mi_array[%d]:\n", i);
      fprintf(stderr, "\tclassname  : %s\n", mi_array[i].classname);
      fprintf(stderr, "\tmethodname : %s\n", mi_array[i].methodname);
    }
  }
  
  fprintf(stderr, "======================\n");
}

void pre_set_rule(){
  struct method_information tmp_mi;
  tmp_mi.classname = "BlackList";
  tmp_mi.methodname = "blackFunction";

  insert_mi_element(&tmp_mi);
}
