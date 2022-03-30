#!/usr/bin/groovy

if (this.args.length == 0) {
    return
}

def input = new File(this.args[0]).readLines()*.split(' ')

def part1 = ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h']
def part2 = ['f', 'b', 'g', 'd', 'c', 'e', 'a', 'h']

input.each {
    if (it[0] == 'swap') {
        if (it[1] == 'position') {
            def x = Integer.valueOf(it[2])
            def y = Integer.valueOf(it[5])

            part1.swap(x, y)
        } else {
            part1 = part1.collect { c ->
                c == it[2] ? it[5] : (c == it[5] ? it[2] : c)
            }
        }
    } else if (it[0] == 'rotate') {
        def dir = it[1]
        if (dir == 'based') {
            def i = part1.indexOf(it[6])
            if (i >= 4) {
                ++i
            }
            ++i
            i %= part1.size()

            part1 = part1.takeRight(i) + part1.dropRight(i)
        } else {
            def dist = Integer.valueOf(it[2]) % part1.size()
            if (it[1] == 'left') {
                part1 = part1.drop(dist) + part1.take(dist)
            } else {
                part1 = part1.takeRight(dist) + part1.dropRight(dist)
            }
        }
    } else if (it[0] == 'reverse') {
        def lower = Integer.valueOf(it[2])
        def higher = Integer.valueOf(it[4])
        part1 = part1.take(lower) + part1[lower..higher].reverse() + part1.drop(higher + 1)
    } else if (it[0] == 'move') {
        def x = Integer.valueOf(it[2])
        def y = Integer.valueOf(it[5])

        def val = part1.removeAt(x)
        part1 = part1.take(y) + val + part1.drop(y)
    }
}

input.reverse().each {
    if (it[0] == 'swap') {
        if (it[1] == 'position') {
            def x = Integer.valueOf(it[2])
            def y = Integer.valueOf(it[5])

            part2.swap(x, y)
        } else {
            part2 = part2.collect { c ->
                c == it[2] ? it[5] : (c == it[5] ? it[2] : c)
            }
        }
    } else if (it[0] == 'rotate') {
        def dir = it[1]
        if (dir == 'based') {
            int i = part2.indexOf(it[6])

            int dist = i
            if (i % 2 != 0 && i <= 7) {
                dist = i - (i - 1) / 2
            } else {
                while (i <= 8) {
                    i += part2.size()
                }
                dist = i - (i - 2) / 2
            }
            dist %= part2.size()
            part2 = part2.drop(dist) + part2.take(dist)
        } else {
            def dist = Integer.valueOf(it[2]) % part2.size()
            if (it[1] == 'left') {
                part2 = part2.takeRight(dist) + part2.dropRight(dist)
            } else {
                part2 = part2.drop(dist) + part2.take(dist)
            }
        }
    } else if (it[0] == 'reverse') {
        def lower = Integer.valueOf(it[2])
        def higher = Integer.valueOf(it[4])
        part2 = part2.take(lower) + part2[lower..higher].reverse() + part2.drop(higher + 1)
    } else if (it[0] == 'move') {
        def x = Integer.valueOf(it[2])
        def y = Integer.valueOf(it[5])

        def val = part2.removeAt(y)
        part2 = part2.take(x) + val + part2.drop(x)
    }
}

println part1
println part2
