#!/usr/bin/groovy

if (this.args.length == 0) {
    return
}

def input = this.args[0].toInteger()

def isValid(int x, int y, int key) {
    x >= 0 && y >= 0 && Integer.toBinaryString(x * x + 3 * x + 2 * x * y + y + y * y + key).count('1') % 2 == 0
}

def target = [31, 39]
def start = [1, 1]

def visited = (1..100).collect { [-1] * 100 }
visited[start[0]][start[1]] = 0

def frontier = [start] as Queue
while (visited[target[0]][target[1]] == -1) {
    def curr = frontier.poll()
    [[curr[0] - 1, curr[1]], [curr[0] + 1, curr[1]], [curr[0], curr[1] - 1], [curr[0], curr[1] + 1]]
        .findAll {
            x, y -> isValid(x, y, input) && visited[x][y] == -1
        }.each { x, y ->
            visited[x][y] = 1 + visited[curr[0]][curr[1]]
            frontier.add([x, y])
        }
}

println visited[target[0]][target[1]]
println((0..<10000).count { 0 <= visited[it / 100][it % 100] && visited[it / 100][it % 100] <= 50 })
