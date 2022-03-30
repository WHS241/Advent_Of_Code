#!/usr/bin/groovy
import groovy.transform.EqualsAndHashCode

if (this.args.length == 0) {
    return
}

def input = new File(this.args[0]).readLines()

def pos = (0..9).collect { [-1] * 2 }

for (i in 0..<input.size()) {
    for (j in 0..<input[i].size()) {
        if (input[i][j].isInteger()) {
            pos[input[i][j].toInteger()] = [i, j]
        }
    }
}

pos = pos.findAll { it != [-1] * 2 }

def dists = (0..<pos.size()).collect {
    def row = [-1] * pos.size()
    row[it] = 0
    def frontier = [pos[it]]
    def visited = (0..<input.size()).collect {
        [-1] * input[it].size()
    }
    visited[pos[it][0]][pos[it][1]] = 0

    while (frontier) {
        def curr = frontier.removeAt(0)
        def neighbors = (0..3).collect { curr.clone() }
        --neighbors[0][0]
        ++neighbors[1][0]
        --neighbors[2][1]
        ++neighbors[3][1]
        for (n in neighbors) {
            if (n[0] >= 0 && n[0] < input.size() && n[1] >= 0 && n[1] < input[0].size() &&
                    input[n[0]][n[1]] != '#' && visited[n[0]][n[1]] == -1) {
                visited[n[0]][n[1]] = visited[curr[0]][curr[1]] + 1
                if (input[n[0]][n[1]].isInteger()) {
                    row[input[n[0]][n[1]].toInteger()] = visited[n[0]][n[1]]
                } else {
                    frontier << n
                }
                    }
        }
    }
    row
}

@EqualsAndHashCode(excludes='cost')
class SearchState implements Comparable<SearchState>, Cloneable {

    int cost
    int current
    List<Boolean> visited

    boolean terminal() { visited.every { it } && current == 0 }
    int compareTo(SearchState s) { this.cost <=> s.cost }

    @Override
    SearchState clone() { new SearchState(cost: this.cost, current: this.current, visited: this.visited.clone()) }

}

def visited = [] as Set
def frontier = new PriorityQueue<SearchState>()
frontier.add(new SearchState(cost: 0, current: 0, visited: [true] + [false] * (pos.size() - 1)))
def found1 = false
def found2 = false

while (!found1 || !found2) {
    def curr = frontier.poll()
    if (!visited.contains(curr)) {
        visited.add(curr)
        if (curr.terminal()) {
            println curr.cost
            found2 = true
        } else {
            if (!found1 && curr.visited.every { it }) {
                println curr.cost
                found1 = true
            }
            for (i in 0..<pos.size()) {
                if (i != curr.current) {
                    def next = curr.clone()
                    next.cost += dists[curr.current][i]
                    next.current = i
                    next.visited[i] = true
                    frontier.add(next)
                }
            }
        }
    }
}
