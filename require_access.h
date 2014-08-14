/**
  Access Controller Module for Secure OS based on OSv/CRuby
  
  Author : alphaKAI
*/
#ifndef REQUIRE_ACCESS_DEFINED
#define REQUIRE_ACCESS_DEFINED

#define REQUIRE_DEBUG

struct require_information {
  const char *filename;
};

/* Prototype defines */

void get_require_info(struct require_information *, rb_thread_t *, VALUE, ID);
void show_require_info(struct require_information *);
int require_granted(int, struct require_information *);

/*
struct require_information *rq_array;
int rq_blacklist;
int rq_ary_len;
*/
#endif
