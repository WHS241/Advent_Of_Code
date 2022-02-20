#!/usr/bin/groovy

if (this.args.length == 0) {
    return
}

def gcd(int x, int y) {
    return (y == 0) ? x : gcd(y, x % y)
}

def lcm(int x, int y) {
    return (x * y) / gcd(x, y)
}

def input = new File(this.args[0]).readLines()*.split().withIndex().collect { line, i ->
    int mod = Integer.valueOf(line[3])
    [mod, (Integer.valueOf(line[-1][0..(line[-1].size() - 2)]) + i + 1) % mod]
}

int min = 0
int inc = 1
for (arr in input) {
    while ((min + arr[1]) % arr[0] != 0) {
        min += inc
    }

    inc = lcm(inc, arr[0])
}

println min
