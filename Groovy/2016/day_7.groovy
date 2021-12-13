#!/usr/bin/groovy

if (this.args.length == 0)
    return

def input = new File(this.args[0]).readLines()

def parsed = input*.split("[\\[\\]]")

def part1 = parsed.count{ partitions ->
    def oddEvenAbbas = (0..<2).toList().collect{ curr ->
        (0..<partitions.size()).findAll{it % 2 == curr}.any{
            def substr = partitions[it]

            (0..substr.size() - 4).any{
                substr.charAt(it) == substr.charAt(it + 3) && substr.charAt(it + 1) == substr.charAt(it + 2) && substr.charAt(it) != substr.charAt(it + 1)
            }
        }
    }

    return oddEvenAbbas[0] && !oddEvenAbbas[1]
}

println part1

def part2 = parsed.count{ partitions ->
    def oddEvenAbas = (0..<2).toList().collect{ curr ->
        def occurrences = [:]
        (0..<partitions.size()).findAll{it % 2 == curr}.each{
            def substr = partitions[it]

            (0..substr.size() - 3).findAll{ substr.charAt(it) == substr.charAt(it + 2) && substr.charAt(it) != substr.charAt(it + 1)}.each{
                if (!occurrences.containsKey(substr.charAt(it)))
                    occurrences[substr.charAt(it)] = []
                occurrences[substr.charAt(it)].add(substr.charAt(it + 1))
            }
        }

        occurrences
    }

    oddEvenAbas[0].any{key, val -> val.any{oddEvenAbas[1].containsKey(it) && oddEvenAbas[1][it].contains(key)}}
}

println part2
