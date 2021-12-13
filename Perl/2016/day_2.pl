#!/usr/bin/perl
use strict;
use warnings;

if (scalar @ARGV == 0) {
    die;
}

my @input;
{
    open(my $fh, "<", $ARGV[0]) or die $!;
    while (<$fh>) {
        chomp;
        push @input, $_;
    }
}

my @pad1 = ([1, 2, 3], [4, 5, 6], [7, 8, 9]);
my @pad2 = ([0, 0, 1, 0, 0], [0, 2, 3, 4, 0], [5, 6, 7, 8, 9], [0, 'A', 'B', 'C', 0], [0, 0, 'D', 0, 0]);

my $pass1 = '';
my $pass2 = '';
my @x1 = (1, 1);
my @x2 = (2, 0);


foreach my $line (@input) {
    foreach my $c (split //, $line) {
        if ($c eq "U") {
            if ($x1[0]) {
                --$x1[0];
            }
            if ($x2[0] && $pad2[$x2[0] - 1][$x2[1]]) {
                --$x2[0];
            }
        } elsif ($c eq "D") {
            if ($x1[0] + 1 != scalar @pad1) {
                ++$x1[0];
            }
            if ($x2[0] + 1 != scalar @pad2 && $pad2[$x2[0] + 1][$x2[1]]) {
                ++$x2[0];
            }
        } elsif ($c eq "L") {
            if ($x1[1]) {
                --$x1[1];
            }
            if ($x2[1] && $pad2[$x2[0]][$x2[1] - 1]) {
                --$x2[1];
            }
        } elsif ($c eq "R") {
            if ($x1[1] + 1 != scalar @{$pad1[$x1[0]]}) {
                ++$x1[1];
            }
            if ($x2[1] + 1 != scalar @{$pad2[$x2[0]]} && $pad2[$x2[0]][$x2[1] + 1]) {
                ++$x2[1];
            }
        }
    }
    $pass1 .= $pad1[$x1[0]][$x1[1]];
    $pass2 .= $pad2[$x2[0]][$x2[1]];
}

print "Part 1: $pass1\n";
print "Part 2: $pass2\n";
