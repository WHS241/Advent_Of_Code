#!/usr/bin/groovy

if (this.args.length == 0) {
    return
}

def input = this.args[0]

def paths = []
def search = [[input, 0, 0]]

while (!search.isEmpty()) {
    def suffixes = [ 'U', 'D', 'L', 'R' ]
    def dirs = [[ -1, 0 ], [ 1, 0 ], [ 0, -1 ], [ 0, 1 ]]
    def curr = search[0]
    def key = curr[0].md5()
    search = search.drop(1)

    for (i in 0..3) {
        def next = [curr[0] + suffixes[i], curr[1] + dirs[i][0], curr[2] + dirs[i][1]]
        if (next[1] >= 0 && next[1] <= 3 &&
                next[2] >= 0 && next[2] <= 3 &&
                ['b', 'c', 'd', 'e', 'f'].contains(key[i])) {
            if (next[1] == 3 && next[2] == 3) {
                paths.add(next[0].drop(input.length()))
            } else {
                search.add(next)
            }
                }
    }
}

def part1 = paths[0]
def part2 = paths[0]

for (p in paths) {
    if (p.length() < part1.length()) {
        part1 = p
    }
    if (p.length() > part2.length()) {
        part2 = p
    }
}

println part1
println part2.length()
