#!/usr/bin/perl


$url = $ARGV[0];
$url =~ /^(([^:\/?#]+):)?(\/\/([^\/?#]*))?([^?#]*)(\?([^#]*))?(#(.*))?/;

print "ARGV[1] = ".$url."\n";
print "Protocol With Colon: $1\n";
print "Protocol Without Colon: $2\n";
print "Host (with slashes): $3\n";
print "Host (w/o slashes): $4\n";
print "path: $5\n";
print "query string: $6\n";
print "query: $7\n";
print "hash: $8\n";
print "hash-sans #: $9\n";


print "scheme    ". $2."\n";
print "authority ". $4."\n";
print "path      ". $5."\n";
print "query     ". $7."\n";
print "fragment  ". $9."\n";

