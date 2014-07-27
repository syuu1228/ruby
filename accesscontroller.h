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
           *removenode;
} ListStruct;

ListStruct blackList;

void insertNode(ListStruct *list, ListNode *newdata){
  ListNode *newnode = (ListNode*)malloc(sizeof(ListNode));
  newnode->data = newdata->data;
  newnode->next = NULL;

  if(list->lastnode != NULL){
    list->lastnode->next = newdata;
    newnode->prev  = list->lastnode;
    list->lastnode = newnode;
  } else {
    list->firstnode = list->lastnode = newnode;
    newnode->prev = NULL;
  }
}

void removeNode(ListStruct *list, ListNode *node){
  ListNode *thisnode;
  for(thisnode = list->firstnode; thisnode != NULL;
      thisnode = thisnode->next){

    if(thisnode->data.classname == node->data.classname &&
        thisnode->data.methodname == node->data.methodname){
      if(thisnode->prev != NULL)
        thisnode->prev->next = thisnode->next;
      else
        list->firstnode = thisnode->next;

      if(thisnode->next != NULL)
        thisnode->next->prev = thisnode->prev;

      free(thisnode);
      break;
    }
  }
}

void showListElement(ListStruct *list){
  ListNode *thisnode;
  fprintf(stderr, "begin\n");
  for(thisnode = list->firstnode; thisnode != NULL;
      thisnode = thisnode->next){
        fprintf(stderr, "ONE OF THE ELEMENT DATA IS\n\tclass : %s method : %s\n",
        thisnode->data.classname, thisnode->data.methodname);
   }
  fprintf(stderr, "end\n");
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
  fprintf(stderr, "class : %s, method : %s\n",
      method_information->classname, method_information->methodname);
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
  ListNode *thisnode;
  int return_status = 0;
#ifdef NOW_DEBUG
  fprintf(stderr, "===============================\n");
  
  fprintf(stderr, "TARGET DATA IS\n\tclass : %s method : %s\n",
          target->classname, target->methodname);
#endif
  
  for(thisnode = list->firstnode; thisnode != NULL;
      thisnode = thisnode->next){
    
#ifdef NOW_DEBUG
    fprintf(stderr, "THIS   DATA IS\n\tclass : %s method : %s\n",
        thisnode->data.classname, thisnode->data.methodname);
#endif

    if(strcmp(thisnode->data.classname, target->classname) == 0 &&
        strcmp(thisnode->data.methodname, target->methodname) == 0 ){
      return_status = 1;
      break;
    }
    
    if(thisnode->prev != NULL)
      thisnode->prev->next = thisnode->next;
    else
      list->firstnode = thisnode->next;
    
    if(thisnode->next != NULL)
      thisnode->next->prev = thisnode->prev;
  }

#ifdef NOW_DEBUG
  fprintf(stderr, "===============================\n");
#endif


  return return_status;
}

void distinction_method(ListStruct *list, METHOD_INFORMATION *method_information){
  int found = search_black_list_element(list, method_information);
  showListElement(list);
  if(strcmp(method_information->classname, "BlackList") == 0){
    printf("--------BLACKLIST-------\n");
    exit(EXIT_FAILURE);
  }

  if(found == 1){
#ifdef NOW_DEBUG
    fprintf(stderr, "Found!\n");
#endif
/*  rb_raise(rb_eRuntimeError, "Error this method is registered to blackList");*/
  } else {
#ifdef NOW_DEBUG
    fprintf(stderr, "Not Found\n");
    add_black_list_element(list, method_information->classname, method_information->methodname);
#endif
  }
}

void preSetBlackList(ListStruct *list){
  add_black_list_element(list, "BlackList", "blackFunction");
}
