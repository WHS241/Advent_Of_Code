#!/usr/bin/perl
use strict;
use warnings;

use List::Util qw(sum min max);
use List::MoreUtils 'pairwise';

if(scalar @ARGV < 1) {
    die;
}

my @raw;
{
    open(my $fh, '<', $ARGV[0]) or die $!;
    while (<$fh>) {
        chomp;
        push @raw, $_;
    }
}
my @input = split(', ', $raw[0]);

my @dirs = ([1, 0], [0, 1], [-1, 0], [0, -1]);
my $curr_dir = 1;

sub turn {
    my %turn_m = ('L' => 1, 'R' => 3);
    return ($_[0] + $turn_m{$_[1]}) % 4;
}

my @curr_pos = (0, 0);
my $intersected = 0;
my @intersect_point;
my @past_segments;

foreach my $step (@input) {
    my ($turn_d, $dist) = $step =~ m/(\w)(\d+)/;
    my @last_pos = @curr_pos;
    $curr_dir = turn($curr_dir, $turn_d);
    @curr_pos = pairwise {$dist * $a + $b} @{$dirs[$curr_dir]}, @curr_pos;

    # PART 2
    if (!$intersected) {
        my @segment = $curr_dir >= 2 ? (@curr_pos, @last_pos) : (@last_pos, @curr_pos);
        @intersect_point = @curr_pos;
        foreach my $prev (@past_segments) {
            my @prev = @{$prev};
            if ($curr_dir % 2 == 0 && $prev[0] == $prev[2] && $segment[0] <= $prev[0] && $prev[0] <= $segment[2] && $prev[1] <= $segment[1] && $segment[1] <= $prev[3]) {
                my $int_x = $curr_dir == 0 ? min($intersect_point[0], $prev[0]) : max($intersect_point[0], $prev[2]);
                if ($int_x != $last_pos[0]) {
                    $intersected = 1;
                    $intersect_point[0] = $int_x;
                }
            } elsif ($curr_dir % 2 == 1 && $prev[1] == $prev[3] && $segment[1] <= $prev[1] && $prev[1] <= $segment[3] && $prev[0] <= $segment[0] && $segment[0] <= $prev[2]) {
                my $int_y = $curr_dir == 1 ? min($intersect_point[1], $prev[1]) : max($intersect_point[1], $prev[1]);
                if ($int_y != $last_pos[1]) {
                    $intersected = 1;
                    $intersect_point[1] = $int_y;
                }
            }
        }
        push @past_segments, \@segment;
    }
}

my $part1 = sum map { abs } @curr_pos;
my $part2 = sum map { abs } @intersect_point;

print "Part 1: $part1\n";
print "Part 2: $part2\n";
