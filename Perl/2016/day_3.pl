#!/usr/bin/perl
use strict;
use warnings;

use List::Util qw(sum max);

if (scalar @ARGV < 1) {
    die;
}

my @input;
{
    open(my $fh, "<", $ARGV[0]) or die $!;
    while (<$fh>) {
        chomp;
        my @vals = $_ =~ m/\s+(\d+)\s+(\d+)\s+(\d+)/;
        push @input, \@vals;
    }
}

my $part1 = 0;
my $part2 = 0;
for(my $i = 0; $i < scalar @input; ++$i) {
    my @vals1 = @{$input[$i]};
    my @vals2;
    {
        use integer;
        my $index = 3 * ($i / 3);
        @vals2 = map {$_->[$i % 3]} @input[$index..$index + 2];
    }
    ++$part1 if ((sum @vals1) > (2 * max(@vals1)));
    ++$part2 if ((sum @vals2) > (2 * max(@vals2)));
}

print "Part 1: $part1\n";
print "Part 2: $part2\n";
