/**
  Access Controller Module for Secure OS based on OSv/CRuby
  
  Author : alphaKAI
*/

#define NOW_DEBUG
#define MAX_ARRAY_SIZE 100

struct method_information {
  const char * classname;
  const char * methodname;
};

struct method_information mi_array[MAX_ARRAY_SIZE];

/* Prototype defines */

void init_mi_array();
void set_Data(struct method_information *, struct method_information *);
int data_is_euqal(struct method_information *, struct method_information *);
int data_is_null(struct method_information *);
void data_clear(struct method_information *);
int mi_array_has_empty();
int search_mi_element(struct method_information *);
void insert_mi_element(struct method_information *);
void remove_mi_element(struct method_information *);
void get_method_info(struct method_information *, rb_thread_t *, VALUE, ID);
void show_method_info(struct method_information *);
void accesscontroller(struct method_information *);
void dump_mi_array();
void pre_set_rule();

