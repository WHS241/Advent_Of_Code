#!/usr/bin/groovy

if (this.args.length == 0) {
    return
}

String compoundHash(String base, int numIt) {
    for (i in (1..numIt))
        base = base.md5()
    return base
}

def input = this.args[0]
def numIt = this.args.length > 1 ? Integer.valueOf(this.args[1]) : 1

def potentials = (1..16).collect { [] }
def valid = [] as SortedSet

def curr = 0
def last = Integer.MAX_VALUE

while (curr < last) {
    def hash = compoundHash(input + curr, numIt)
    def foundFirstTriplet = false
    def c = hash[0]
    def count = 1
    def val = Integer.parseInt(c, 16)
    for (i in (1..<hash.size())) {
        if (hash[i] == c) {
            ++count
            if (count == 3 && !foundFirstTriplet) {
                foundFirstTriplet = true
                potentials[val] << curr
            } else if (count == 5) {
                potentials[val].each {
                    if (curr != it && curr - it <= 1000) {
                        valid << it
                    }
                }
                potentials[val] = [potentials[val][-1]]
            }
        } else {
            c = hash[i]
            count = 1
            val = Integer.parseInt(c, 16)
        }
    }

    if (valid.size() >= 64) {
        last = Math.min(last, curr + 1000)
    }
    ++curr
}

println ((valid as List)[63])
