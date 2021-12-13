#!/usr/bin/perl
use strict;
use warnings;

use List::Util qw(sum);

die if (scalar @ARGV < 1);

my @rooms;
{
    open(my $reader, "<", $ARGV[0]) or die $!;
    while (<$reader>) {
        chomp;

        # Verification
        my @info = $_ =~ m/(\D+)(\d+)\[(\w+)\]/;
        my ($room, $id, $check) = @info;
        my %freq;

        foreach my $c (split(//, $room)) {
            ++$freq{$c} if $c ne "-";
        }
        my @ordered = sort { ($freq{$b} <=> $freq{$a}) || ($a cmp $b) } keys %freq;
        push @rooms, \@info if ($check eq join("", @ordered[0..length($check) - 1]));
    }
}

my $part1 = sum map {$_->[1]} @rooms;

print "Part 1: $part1\n";

my $a = ord("a");
foreach my $info (@rooms) {
    my ($ciphertext, $id) = @{$info};
    my @plainchars;
    foreach my $c (split(//, $ciphertext)) {
        if ($c eq "-") {
            push @plainchars, " ";
        } else {
            my $val = ord($c) - $a;
            push @plainchars, chr($a + (($val + $id) % 26));
        }
    }

    my $str = join("", @plainchars);
    print("$str: $id\n");
}
