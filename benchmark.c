#include "urlparser.h"
#include "benchmark.h"

int benchmark_parseURL(int exp) {
  char *url ="http://www.google.com:8080/this/is/the/path?query=string&more=data&even=more#this-is-the-hash";
  URL storage;
  int i;
  for(i = 0; i < exp; i++) {
    parseURL(url, &storage);
  }
  return 0;
}
