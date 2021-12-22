#!/usr/bin/groovy

if (this.args.length == 0) {
    return
}

def input = new File(this.args[0]).readLines().collect {
    it.split(' ')*.trim().findAll { !it.isEmpty() }*.toInteger()
}

def part1 = input.count { it.sum() > 2 * it.max() }

def transposed = input.collate(3).collect { GroovyCollections.transpose(it) }.flatten().collate(3)
def part2 = transposed.count { it.sum() > 2 * it.max() }

println part1
println part2
