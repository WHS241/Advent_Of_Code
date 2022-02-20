#!/usr/bin/groovy

if (this.args.length < 2) {
    return
}

def input = this.args[0].collect { Integer.valueOf(it) }
def size = Integer.valueOf(this.args[1])

while (input.size() < size) {
    def tail = input.reverse().collect { 1 - it }
    input = input + 0 + tail
}

input = input.take(size)

while (input.size() % 2 == 0) {
    input = input.collate(2).collect { it[0] == it[1] ? 1 : 0 }
}

println input.join()
