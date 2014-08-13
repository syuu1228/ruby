/**
  Access Controller Module for Secure OS based on OSv/CRuby
  
  Author : alphaKAI
*/

#define ACCESSCONTROLLER_DEBUG

struct method_information {
  const char *classname;
  const char *methodname;
};

/* Prototype defines */

void get_method_info(struct method_information *, rb_thread_t *, VALUE, ID);
void show_method_info(struct method_information *);
int method_granted(struct method_information *);
int insert_mi_element(const char*, const char*);

