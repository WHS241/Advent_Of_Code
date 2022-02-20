#!/usr/bin/groovy

if (this.args.length == 0) {
    return
}

def input = Integer.valueOf(this.args[0])

def power = 1
while (power <= input) {
    power *= 2
}
power /= 2
def part1 = 2 * (input - power) + 1

def part2 = 1
for (i in 1..<input) {
    ++part2
    if (part2 > i) {
        part2 = 1
    }
    if (part2 > i / 2 + 1) {
        ++part2
    }
    if (part2 > i) {
        part2 = 1
    }
}

println part1
println part2
