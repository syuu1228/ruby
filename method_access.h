/**
  Access Controller Module for Secure OS based on OSv/CRuby
  
  Author : alphaKAI
*/

#define METHOD_DEBUG
#ifndef METHOD_ACCESS_DEFINED
#define METHOD_ACCESS_DEFINED
struct method_information {
  const char *classname;
  const char *methodname;
};

struct mi_ruleset {
  struct method_information *rules;
  int rule_len;
  int blacklist;
};

/* Prototype defines */

void get_method_info(struct method_information *, rb_thread_t *, VALUE, ID);
void show_method_info(struct method_information *);
int method_granted(int, struct method_information *);

/*
struct method_information mi_array[];
int mi_blacklist;
int mi_ary_len;*/
#endif
