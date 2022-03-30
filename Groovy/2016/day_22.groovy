#!/usr/bin/groovy

if (this.args.length == 0) {
    return
}

def input = new File(this.args[0]).readLines().drop(2)*.split().collect {
    def parsed = it[0].split('-')
    [Integer.valueOf(parsed[1].substring(1)), Integer.valueOf(parsed[2].substring(1))] + it.drop(1).collect {
        Integer.valueOf(it.substring(0, it.length() - 1))
    }
}

def part1Count = 0
for (i in 0..<input.size()) {
    for (j in 0..<input.size()) {
        if (input[i][3] != 0 && i != j && input[i][3] <= input[j][4]) {
            ++part1Count
        }
    }
}

println part1Count

def xMax = input*.getAt(0).max()
def yMax = input*.getAt(1).max()

def costs = (0..xMax).collect {
    (0..yMax).collect {
        (0..xMax).collect {
            [-1] * yMax
        }
    }
}

def frontier = [] as LinkedList
def walls = []

for (l in input) {
    if (l[3] == 0) {
        costs[xMax][0][l[0]][l[1]] = 0
        frontier << [xMax, 0, l[0], l[1]]
    } else if (l[3] > 200) {
        walls << [l[0], l[1]]
    }
}

while (costs[0][0][1][0] < 0) {
    def curr = frontier.removeAt(0)
    def currCost = costs[curr[0]][curr[1]][curr[2]][curr[3]]
    def neighbors = [curr.clone(), curr.clone(), curr.clone(), curr.clone()]
    ++neighbors[0][2]
    --neighbors[1][2]
    ++neighbors[2][3]
    --neighbors[3][3]
    for (state in neighbors) {
        if (state[0] == state[2] && state[1] == state[3]) {
            state[0] = curr[2]
            state[1] = curr[3]
        }
        if (state[0] >= 0 && state[1] >= 0 && state[2] >= 0 && state[3] >= 0 && state[2] <= xMax &&
                state[3] <= yMax && !walls.contains([state[2], state[3]]) &&
                costs[state[0]][state[1]][state[2]][state[3]] == -1) {
            costs[state[0]][state[1]][state[2]][state[3]] = currCost + 1
            frontier << state
                }
    }
}

println costs[0][0][1][0]

