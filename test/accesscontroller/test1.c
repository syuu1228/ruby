#include "ruby.h"

void black(void);

VALUE wrap_black(self)
  VALUE self;
{
  black();
  return Qnil;
}

void Init_tctrl(){
  VALUE class;
  class = rb_define_class("BlackList", rb_cObject);
  rb_define_method(class, "black", wrap_black, 0);
}
