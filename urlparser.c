#include <stdio.h>
#include <regex.h>
#include <strings.h>
#include <stdlib.h>

#include "urlparser.h"

/**
 *  This library implements the following regular expression from    
 *  http://tools.ietf.org/html/rfc3986
 *
 *  /^(([^:\/?#]+):)?(\/\/([^\/?#]*))?([^?#]*)(\?([^#]*))?(#(.*))?/
 *    12              3   4           5       6  7        8 9
 *
*/

int parseURL(const char *url, URL *storage) {
  return parseURLField(url, storage, 0);
}

int parseURLField(const char *url, URL *storage, URLField field) {
  URLGroup groups[10];
  const char *c = url;

  bzero(groups, sizeof(URLGroup)*10);

  if(!c || *c == '\0') {
    return 1;
  } 

  /* URLGroups 1 and 2
  */
  while(*c != ':' && *c != '/' && *c != '?' && *c != '#' && *c != '\0') {
    groups[1].end++;
    c++;
  }

  groups[2].end = groups[1].end;

  if(*c == ':') {
    groups[1].end++;
    c++;
  } else {
    groups[1].start = groups[2].start = 0;
    groups[1].end   = groups[2].end   = 0;
    c = url;
  }

  storage->scheme    = groups[2];
  if(field == URL_SCHEME) {
    return 0;    
  }

  /* URLGroups 3 and 4 
  */
  groups[3].end  = groups[3].start = groups[1].end; 
  groups[4].end  = groups[4].start = groups[1].end; 

  if(*c == '/' && *(c+1) == '/') {
    groups[4].start += 2;
    groups[3].end += 2;
    c += 2; 
    while(*c != '/' && *c != '?' && *c != '#' && *c != '\0') {
      groups[3].end++;
      c++;
    }
    groups[4].end = groups[3].end;
  }

  storage->authority = groups[4];
  if(field == URL_AUTHORITY) {
    return 0;
  }
  /* URLGroup 5
  */ 
  groups[5].start = groups[5].end = groups[4].end; 

  while(*c != '?' && *c != '#' && *c != '\0') {
    groups[5].end++;
    c++;
  }

  storage->path = groups[5];
  if(field == URL_PATH) {
    return 0;
  }
  /* URLGroups 6 and 7
  */ 
  groups[6].end  = groups[6].start = groups[5].end; 
  groups[7].end  = groups[7].start = groups[5].end; 

  if(*c == '?') {
    groups[7].start++;
    groups[6].end++;
    c++;
    while(*c != '#' && *c != '\0') {
      groups[6].end ++;
      c++;
    } 
    groups[7].end = groups[6].end;
  } 

  storage->query = groups[7];
  if(field == URL_QUERY) {
    return 0;
  }
  /* URLGroups 8 and 9
  */
  groups[8].end  = groups[8].start = groups[7].end; 
  groups[9].end  = groups[9].start = groups[7].end; 

  if(*c == '#') {
    groups[9].start++;
    groups[8].end++;
    c++;
    while(*c != '\0') {
      groups[8].end ++;
      c++;
    } 
    groups[9].end = groups[8].end;
  } 

  storage->fragment  = groups[9];
  return 0;
}

char *readURLField(const char* url, URLGroup field) {
  char *response;

  if(field.end - field.start <= 0) 
    return "";

  response = (char*) malloc((sizeof(char)) * (field.end - field.start + 1));
  bcopy(url + field.start, response, field.end - field.start);
  response[field.end - field.start] = '\0';

  return response;
}


int cmpURLField(const char *url, URLGroup field, const char *to) {
  if(field.start > field.end) {
    return -1;
  }

  if(field.start == field.end) {
    return !(to == 0 || *to == '\0');
  }

  return bcmp(to, url + field.start, field.end - field.start);
}
