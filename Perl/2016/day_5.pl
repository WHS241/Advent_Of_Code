#!/usr/bin/perl
use strict;
use warnings;

use List::Util qw(notall);
use Scalar::Util qw(looks_like_number);
use Digest::MD5 qw(md5_hex);

die if (scalar @ARGV < 1);

my $pass1 = "";
my @pass2 = (undef) x 8; 

my $i = 0;
while (length($pass1) != 8 || notall { defined $_ } @pass2) {
    my $hash = md5_hex($ARGV[0].$i);
    if ($hash =~ m/^0{5}(.)(.).*/) {
        $pass1 .= $1 if length($pass1) != 8;
        my $pos = $1;
        my $val = $2;
        $pass2[$pos] = $val if (($pos =~ m/[0-7]/) && (!defined $pass2[$pos]));
    }
    ++$i;
}

print "Part 1: $pass1\n";
my $pass2 = join("", @pass2);
print "Part 2: $pass2\n";
