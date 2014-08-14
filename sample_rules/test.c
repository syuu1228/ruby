#include <stdio.h>
#include <stdlib.h>

struct method_information {
  const char *classname;
  const char *methodname;
};

struct mi_ruleset {
  struct method_information *rules;
  int rule_len;
  int blacklist;
};

struct method_information mi_array0[] = {
  {"BlackClass", "blackFunction"}
};

struct method_information mi_array1[] = {
  {"BlackClass2", "blackFunction2"},
  {"BlackClass3", "blackFunction3"}
};

struct mi_ruleset mi_rule_array[] = {
  { mi_array0, 1, 1},
  { mi_array1, 2 , 0}
};

struct require_information {
  const char *filename;
};

struct rq_ruleset {
  struct require_information *rules;
  int rule_len;
  int blacklist;
};

struct require_information rq_array0[] = {
  {"twitter"}
};
struct require_information rq_array1[] = {
  {"oauth"},
  {"json"}
};

struct rq_ruleset rq_rule_array[] = {
  { rq_array0, 1, 1},
  { rq_array1, 2, 0}
};

int main(){
  return 0;
}
