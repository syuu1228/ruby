/**
  Access Controller Module for Secure OS based on OSv/CRuby
  
  Author : alphaKAI
*/

#define REQUIRE_DEBUG

struct require_information {
  const char *filename;
};

/* Prototype defines */

void get_require_info(struct require_information *, rb_thread_t *, VALUE, ID);
void show_require_info(struct require_information *);
int require_granted(struct require_information *);
int insert_rq_element(const char*);

