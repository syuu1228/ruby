/**
  Access Controller Module for Secure OS based on OSv/CRuby

Author : alphaKAI
*/
#include <stdio.h>
#include <stdlib.h>

#define ARRAY_SIZE 100

static const char* rq_array[ARRAY_SIZE] = {};

void
insert_rq_element(const char* rq)
{
  int i;

  for (i = 0; i < ARRAY_SIZE; i++) {
    if (rq_array[i] == NULL) {
     rq_array[i] = (const char*)malloc(sizeof(const char*));
     rq_array[i] = rq;
    }
  }
}

void dump_rq_array(){
  int i;

  for (i = 0; i < ARRAY_SIZE; i++) {
    if (rq_array[i] == NULL) {
      break;
    }
    fprintf(stderr, "[REQUIRE]%s\n", rq_array[i]);
  }

}
