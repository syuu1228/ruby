#include "ruby.h"

void blackFunction(void);

VALUE wrap_black_function(self)
  VALUE self;
{
  blackFunction();
  return Qnil;
}

void Init_tctrl(){
  VALUE class;
  class = rb_define_class("BlackList", rb_cObject);
  rb_define_method(class, "black", wrap_black_function, 0);
}
