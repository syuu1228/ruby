/**
  Access Controller Module for Secure OS based on OSv/CRuby
  
  Author : alphaKAI
*/
#include <stdlib.h>
#include <string.h>

#define NOW_DEBUG

typedef struct METHOD_INFORMATION{
  const char * classname;
  const char * methodname;
} METHOD_INFORMATION;

typedef struct ListNode{
  struct ListNode *prev;
  struct ListNode *next;
  METHOD_INFORMATION data;
} ListNode;

typedef struct ListStruct{
  ListNode *firstnode,
           *lastnode,
           *newnode,
           *thisnode,
           *removenode;
} ListStruct;

ListStruct blackList;

void insertNode(ListStruct *list, ListNode *newdata){
  list->newnode = (ListNode*)malloc(sizeof(ListNode));
  list->newnode->data = newdata->data;
  list->newnode->next = NULL;

  if(list->lastnode != NULL){
    list->lastnode->next = newdata;
    list->newnode->prev  = list->lastnode;
    list->lastnode       = list->newnode;
  } else {
    list->firstnode = list->lastnode = list->newnode;
    list->newnode->prev = NULL;
  }
}

void removeNode(ListStruct *list, ListNode *node){
  for(list->thisnode = list->firstnode; list->thisnode != NULL;
      list->thisnode = list->thisnode->next){
    if(list->thisnode->data.classname == node->data.classname &&
        list->thisnode->data.methodname == node->data.methodname){
      if(list->thisnode->prev != NULL)
        list->thisnode->prev->next = list->thisnode->next;
      else
        list->firstnode = list->thisnode->next;

      if(list->thisnode->next != NULL)
        list->thisnode->next->prev = list->thisnode->prev;

      free(list->thisnode);
      break;
    }
  }
}

void get_method_info(METHOD_INFORMATION* method_information, rb_thread_t* th, VALUE klazz, ID id) {
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

  method_information->classname = classname;
  method_information->methodname = methodname;
}

void show_method_info(METHOD_INFORMATION *method_information){
  fprintf(stderr, "class : %s, method : %s\n", method_information->classname, method_information->methodname);
}

void add_black_list_element(ListStruct *list, const char * classname, const char * methodname){
  ListNode currentNode;
  currentNode.data.classname = classname;
  currentNode.data.methodname = methodname;

  insertNode(list, &currentNode);
}

void remove_black_list_element(ListStruct *list, const char * classname, const char * methodname){
  ListNode currentNode;
  currentNode.data.classname = classname;
  currentNode.data.methodname = methodname;

  removeNode(list, &currentNode);
}

int search_black_list_element(ListStruct *list, METHOD_INFORMATION *target){
  int return_status = 0;
#ifdef NOW_DEBUG
  fprintf(stderr, "===============================\n");
  
  fprintf(stderr, "TARGET DATA IS\n\tclass : %s method : %s\n",
          target->classname, target->methodname);
#endif

  for(list->thisnode = list->firstnode; list->thisnode != NULL;
      list->thisnode = list->thisnode->next){
    
#ifdef NOW_DEBUG
    fprintf(stderr, "THIS   DATA IS\n\tclass : %s method : %s\n",
        list->thisnode->data.classname, list->thisnode->data.methodname);
#endif

    if(strcmp(list->thisnode->data.classname, target->classname) == 0 &&
        strcmp(list->thisnode->data.methodname, target->methodname) == 0 ){
      return_status = 1;
      break;
    }
    
    if(list->thisnode->prev != NULL)
      list->thisnode->prev->next = list->thisnode->next;
    else
      list->firstnode = list->thisnode->next;
    
    if(list->thisnode->next != NULL)
      list->thisnode->next->prev = list->thisnode->prev;

  }

#ifdef NOW_DEBUG
  fprintf(stderr, "===============================\n");
#endif

  return return_status;
}

void distinction_method(ListStruct *list, METHOD_INFORMATION *method_information){
  int found = search_black_list_element(list, method_information);
  if(found == 1){
#ifdef NOW_DEBUG
    fprintf(stderr, "Found!\n");
#endif
    rb_raise(rb_eRuntimeError, "Error this method is registered to blackList");
  } else {
#ifdef NOW_DEBUG
    fprintf(stderr, "Not Found\n");
    /*add_black_list_element(list, method_information->classname, method_information->methodname);*/
#endif
  }
}

void preSetBlackList(ListStruct *list){
  add_black_list_element(list, "blackList", "blackFunction");
}
