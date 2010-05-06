#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>

#include "urlparser.h"

#define assert(x) do{if(!(x)) fprintf(stderr,"%s:%d: %s\n", \
    __FILE__,__LINE__,#x);fflush(stderr);}while(0)
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
  assert(!cmpURLField("http://google.com", storage.scheme, "http"));
  assert(parseURL("ftp://google.com", &storage) == 0) ;
  assert(!strcmp(readURLField("ftp://google.com", storage.scheme), "ftp"));
  assert(!cmpURLField("ftp://google.com", storage.scheme, "ftp"));
  assert(parseURL("ssh://google.com", &storage) == 0) ;
  assert(!strcmp(readURLField("ssh://google.com", storage.scheme), "ssh"));
  assert(parseURL("ssh-http-ftp://google.com", &storage) == 0) ;
  assert(!strcmp(readURLField("ssh-http-ftp://google.com", storage.scheme), "ssh-http-ftp"));
  assert(parseURL("google.com", &storage) == 0) ;
  assert(!strcmp(readURLField("google.com", storage.scheme), ""));
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
  assert(!strcmp(readURLField("www.google.com:8080", storage.authority),""));

  assert(            parseURL("http://www.google.com:8080", &storage) == 0);
  assert(!strcmp(readURLField("http://www.google.com:8080", storage.authority),"www.google.com:8080"));
}

void test5() {
  URL storage;
  assert(    parseURL("http://www.google.com", &storage) == 0);
  assert(!strcmp(readURLField("http://www.google.com", storage.path),""));

  assert(            parseURL("http://www.google.com/some/path/", &storage) == 0);
  assert(!strcmp(readURLField("http://www.google.com/some/path/", storage.path),"/some/path/"));

  assert(            parseURL("http://www.google.com:port/some/path/", &storage) == 0);
  assert(!strcmp(readURLField("http://www.google.com:port/some/path/", storage.path),"/some/path/"));

  assert(            parseURL("http://www.google.com:port/some/path/?query", &storage) == 0);
  assert(!strcmp(readURLField("http://www.google.com:port/some/path/?query", storage.path),"/some/path/"));

  assert(            parseURL("http://www.google.com:port/some/path/#hash", &storage) == 0);
  assert(!strcmp(readURLField("http://www.google.com:port/some/path/#hash", storage.path),"/some/path/"));
  assert(            parseURL("http://www.google.com:port/some/path/#hash/looks/like/more", &storage) == 0);
  assert(!strcmp(readURLField("http://www.google.com:port/some/path/#hash/looks/like/more", storage.path),"/some/path/"));
}
void test6() {
  URL storage;
  assert(    parseURL("http://www.google.com", &storage) == 0);
  assert(!strcmp(readURLField("http://www.google.com", storage.query),""));
  assert(    parseURL("http://www.google.com/some/path", &storage) == 0);
  assert(!strcmp(readURLField("http://www.google.com/some/path", storage.query),""));
  assert(            parseURL("http://www.google.com:port/some/path/?thequery=true", &storage) == 0);
  assert(!strcmp(readURLField("http://www.google.com:port/some/path/?thequery=true", storage.query),"thequery=true"));
  assert(!cmpURLField("http://www.google.com:port/some/path/?thequery=true", storage.query,"thequery=true"));
  assert(            parseURL("http://www.google.com:port/some/path/?thequery=true#withhash", &storage) == 0);
  assert(!strcmp(readURLField("http://www.google.com:port/some/path/?thequery=true#withhash", storage.query),"thequery=true"));
  assert(            parseURL("http://www.google.com:port/some/path/?thequery=true#withhash?another", &storage) == 0);
  assert(!strcmp(readURLField("http://www.google.com:port/some/path/?thequery=true#withhash?another", storage.query),"thequery=true"));
}
void test7() {
  URL storage;
  assert(    parseURL("http://www.google.com", &storage) == 0);
  assert(!strcmp(readURLField("http://www.google.com", storage.fragment),""));
  assert(    parseURL("http://www.google.com/some/path", &storage) == 0);
  assert(!strcmp(readURLField("http://www.google.com/some/path", storage.fragment),""));
  assert(            parseURL("http://www.google.com:port/some/path/?thefragment=true", &storage) == 0);
  assert(!strcmp(readURLField("http://www.google.com:port/some/path/?thefragment=true", storage.fragment),""));
  assert(            parseURL("http://www.google.com:port/some/path/?thefragment=true#withhash", &storage) == 0);
  assert(!strcmp(readURLField("http://www.google.com:port/some/path/?thefragment=true#withhash", storage.fragment),"withhash"));
  assert(            parseURL("http://www.google.com:port/some/path/?thefragment=true#withhash?another", &storage) == 0);
  assert(!strcmp(readURLField("http://www.google.com:port/some/path/?thefragment=true#withhash?another", storage.fragment),"withhash?another"));
}

void test8() {
  URL storage;
  char *path = "http://www.google.com/some/path/whatev?somequery#hash";
  assert(parseURLField(path, &storage, URL_SCHEME) == 0);
  assert(!cmpURLField(path, storage.scheme, "http"));
  assert(parseURLField(path, &storage, URL_AUTHORITY) == 0);
  assert(!cmpURLField(path, storage.authority, "www.google.com"));
  assert(parseURLField(path, &storage, URL_PATH) == 0);
  assert(!cmpURLField(path, storage.path, "/some/path/whatev"));
  assert(parseURLField(path, &storage, URL_QUERY) == 0);
  assert(!cmpURLField(path, storage.query, "somequery"));
  assert(parseURLField(path, &storage, URL_FRAGMENT) == 0);
  assert(!cmpURLField(path, storage.fragment, "hash"));

}

int main(int argc, char** argv) {
  test1();
  test2();
  test3();
  test4();
  test5();
  test6();
  test7();
  test8();
  return 0;
}
