#!/usr/bin/groovy

if (this.args.length == 0) {
    return
}

def input = new File(this.args[0]).readLines().collect {
    def id = it.find(/\d+/)
    def checkPos = it.indexOf('[') + 1
    [it.substring(0, it.indexOf(id)), id.toInteger(), it.substring(checkPos, checkPos + 5)]
}

def valids = input.findAll {
    def counts = [:]
    it[0].each {
        counts.merge(it, 1, { a, b -> a + b })
    }

    def byCount = new TreeMap(counts.groupBy { -it.value })
    String generatedCheck = ''
    for (def chars in byCount.values()) {
        for (def c in chars.sort()*.key) {
            if (c != '-') {
                generatedCheck += c
                if (generatedCheck.length() == 5) {
                    break
                }
            }
        }
        if (generatedCheck.length() == 5) {
            break
        }
    }

    generatedCheck == it[2]
}

def deciphered = valids.collect {
    def shiftVal = it[1]
    [it[0].collect {
        it == '-' ? ' ' : (char)(((int)it - (int)'a' + shiftVal) % 26 + (int)'a')
    }.join(''), it[1]]
}

deciphered.each {
    println it
}

println valids.collect { it[1] }.sum()
