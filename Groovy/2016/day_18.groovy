#!/usr/bin/groovy

if (this.args.length < 2) {
    return
}

def line = [false] + new File(this.args[0]).readLines()[0].getChars().collect { it == '^' } + false

def result = 0

for (i in 1..(Integer.valueOf(this.args[1]))) {
    result += line.count(false) - 2
    line = (0..<line.size()).collect { it != 0 && it != line.size() - 1 && (line[it - 1] ^ line[it + 1]) }
}

println result
