#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>

#include "urlparser.h"

#define assert(x) do{if(!(x)) fprintf(stderr,"%s:%d: %s\n",__FILE__,__LINE__,#x);fflush(stderr);}while(0)
#define assertValue(url, x,v) assert(bcmp(v, url+x.start, x.end - x.start) == 0)

void printURLGroups(const char *url, URL *storage) {

  printf("scheme    %s\n", readURLField(url, storage->scheme));
  printf("authority %s\n", readURLField(url, storage->authority));
  printf("path      %s\n", readURLField(url, storage->path));
  printf("query     %s\n", readURLField(url, storage->query));
  printf("fragment  %s\n", readURLField(url, storage->fragment));
}

void test1() {
  /* empty string and null pointer
  */
  URL storage;
  assert(parseURL("", &storage) != 0) ;
  assert(parseURL(0, &storage)  != 0) ;
}

void test2() {
  /* various schemes */
  URL storage;
  assert(parseURL("http://google.com", &storage) == 0) ;
  assert(storage.scheme.start == 0);
  assert(storage.scheme.end == 4);
  assertValue("http://google.com",storage.scheme, "http");
  assert(parseURL("ftp://google.com", &storage) == 0) ;
  assert(storage.scheme.start == 0);
  assert(storage.scheme.end == 3);
  assertValue("ftp://google.com",storage.scheme, "ftp");
  assert(parseURL("ssh://google.com", &storage) == 0) ;
  assert(storage.scheme.start == 0);
  assert(storage.scheme.end == 3);
  assertValue("ssh://google.com",storage.scheme, "ssh");

  assert(parseURL("ssh-http-ftp://google.com", &storage) == 0) ;
  assert(storage.scheme.start == 0);
  assert(storage.scheme.end == 12);
  assertValue("ssh-http-ftp://google.com",storage.scheme, "ssh-http-ftp");

  assert(parseURL("google.com", &storage) == 0) ;
  assert(storage.scheme.start == 0);
  assert(storage.scheme.end == 0);
  assertValue("",storage.scheme, "");
}

void test3() {
  /* same as test2 but use readURLField
   */
  URL storage;
  assert(parseURL("http://google.com", &storage) == 0) ;
  assert(!strcmp(readURLField("http://google.com", storage.scheme), "http"));
  assert(parseURL("ftp://google.com", &storage) == 0) ;
  assert(!strcmp(readURLField("ftp://google.com", storage.scheme), "ftp"));
  assert(parseURL("ssh://google.com", &storage) == 0) ;
  assert(!strcmp(readURLField("ssh://google.com", storage.scheme), "ssh"));
  assert(parseURL("ssh-http-ftp://google.com", &storage) == 0) ;
  assert(!strcmp(readURLField("ssh-http-ftp://google.com", storage.scheme), "ssh-http-ftp"));
  assert(parseURL("google.com", &storage) == 0) ;
  assert(readURLField("google.com", storage.scheme) == 0);
}

void test4() {
  URL storage;
  assert(            parseURL("http://google.com", &storage) == 0);
  assert(!strcmp(readURLField("http://google.com", storage.authority),"google.com"));
  assert(            parseURL("http://www.google.com", &storage) == 0);
  assert(!strcmp(readURLField("http://www.google.com", storage.authority),"www.google.com"));

  /* In this case, www.google.com should be treated as a path.  If you have a
   * hostname, then it MUST be preceeded by a scheme.
   */
  assert(            parseURL("www.google.com", &storage) == 0);
  assert(readURLField("www.google.com", storage.authority) == 0);

  assert(            parseURL("http://www.google.com:8080", &storage) == 0);
  assert(!strcmp(readURLField("http://www.google.com:8080", storage.authority),"www.google.com:8080"));
}

int main(int argc, char** argv) {
  test1();
  test2();
  test3();
  test4();
  return 0;
}
