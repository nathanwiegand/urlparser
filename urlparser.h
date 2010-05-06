#ifndef __URL_PARSER__
#define __URL_PARSER__

/* 
  This library implements the following regular expression from    
  http://tools.ietf.org/html/rfc3986

  /^(([^:\/?#]+):)?(\/\/([^\/?#]*))?([^?#]*)(\?([^#]*))?(#(.*))?/
    12              3   4           5       6  7        8 9
*/


typedef struct {
  int start;
  int end;
} URLGroup;

typedef struct {
  URLGroup scheme; 
  URLGroup authority;
  URLGroup path;
  URLGroup query;
  URLGroup fragment;
} URL;

typedef enum {
  URL_SCHEME    = 2,
  URL_AUTHORITY = 4,
  URL_PATH      = 5,
  URL_QUERY     = 7,
  URL_FRAGMENT  = 9
} URLField;

int parseURL(const char *url, URL *storage);
int parseURLField(const char *url, URL *storage, URLField field);

char *readURLField(const char *url, URLGroup field);

int cmpURLField(const char *url, URLGroup field, const char *to);
#endif
