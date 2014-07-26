/**
  Access Controller Module for Secure OS based on OSv/CRuby
  
  Author : alphaKAI
*/

typedef struct METHOD_INFORMATION{
  const char * classname;
  const char * methodname;
} METHOD_INFORMATION;

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
