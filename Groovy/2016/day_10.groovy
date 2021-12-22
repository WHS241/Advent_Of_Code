#!/usr/bin/groovy

if (this.args.length == 0) {
    return
}

def input = new File(this.args[0]).readLines()*.split(' ')

def output = [:]
def compare = [:]
def pass = [:]
def stack = []
input.each {
    if (it[0] == 'value') {
        def botNum = it[5].toInteger()
        if (!compare.containsKey(botNum)) {
            compare[botNum] = []
        }
        compare[botNum] += it[1].toInteger()
        if (compare[botNum].size() == 2) {
            stack += botNum
        }
    } else {
        def destLow = it[6].toInteger()
        def destHigh = it[11].toInteger()
        if (it[5] == 'output') {
            destLow = - (1 + destLow)
        }
        if (it[10] == 'output') {
            destHigh *= - (1 + destHigh)
        }
        pass[it[1].toInteger()] = [destLow, destHigh]
    }
}

while (stack) {
    def curr = stack.removeAt(stack.size() - 1)
    if (compare[curr].containsAll([61, 17])) {
        println curr
    }
    if (pass[curr][0] < 0) {
        output[-pass[curr][0] - 1] = compare[curr].min()
    }
    else {
        if (!compare.containsKey(pass[curr][0])) {
            compare[pass[curr][0]] = []
        }
        compare[pass[curr][0]] += compare[curr].min()
        if (compare[pass[curr][0]].size() == 2) {
            stack += pass[curr][0]
        }
    }

    if (pass[curr][1] < 0) {
        output[-pass[curr][1] - 1] = compare[curr].max()
    } else {
        if (!compare.containsKey(pass[curr][1])) {
            compare[pass[curr][1]] = []
        }
        compare[pass[curr][1]] += compare[curr].max()
        if (compare[pass[curr][1]].size() == 2) {
            stack += pass[curr][1]
        }
    }
}

println output[0] * output[1] * output[2]
