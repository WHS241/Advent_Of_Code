#!/usr/bin/groovy
import groovy.transform.EqualsAndHashCode

if (this.args.length == 0) {
    return
}

@EqualsAndHashCode(excludes='cost')
class State implements Comparable<State>, Cloneable {

    static final int NUM_FLOORS = 4
    int cost
    int current
    List locs

    boolean terminal() { locs.every { it.every { it == NUM_FLOORS } } }
    int heuristic() { (2 * NUM_FLOORS * locs.size() - locs*.sum().sum()) / 2 }
    int compareTo(State s) { heuristic() + cost <=> s.heuristic() + s.cost }
    boolean valid() {
       def floorView = ([0] * 8).collate(2)
       locs.each {
           ++floorView[it[0] - 1][0]
           ++floorView[it[1] - 1][1]
       }
       locs.every { it[0] == it[1] || floorView[it[0] - 1][1] == 0 }
    }

    @Override
    State clone() {
        new State(cost: this.cost, current: this.current, locs: this.locs*.clone())
    }

}

def input = new File(this.args[0]).readLines()*.split(' ')*.drop(4)*.minus('and')*.collate(3)

def initialMap = [:]

input.eachWithIndex { it , i ->
    it.findAll { it[0] != 'nothing' }.each {
        def isMicro = it[-1].startsWith('microchip')
        def element = it[-2]
        if (isMicro) {
            element = element.substring(0, element.indexOf('-'))
        }
        if (!initialMap.containsKey(element)) {
            initialMap[element] = [0, 0]
        }
        initialMap[element][isMicro ? 0 : 1] = i + 1
}
}
def initial = new State(cost:0, current:1, locs:initialMap.values().sort())

def visited = [] as Set
def frontier = new PriorityQueue<State>()
frontier.add(initial)

int result = -1

while (result == -1 && !frontier.isEmpty()) {
    def curr = frontier.poll()
    if (curr.terminal()) {
        result = curr.cost
    } else if (!visited.contains(curr)) {
        visited += curr
        def successors = []
        [(curr.current - 1), (curr.current + 1)].each { targetFloor ->
            if (1 <= targetFloor && targetFloor <= State.NUM_FLOORS) {
                curr.locs.eachWithIndex { l, i ->
                    if (l == [curr.current, curr.current]) {
                        def next = curr.clone()
                        next.locs[i] = [targetFloor, targetFloor]
                        next.locs = next.locs.sort()
                        ++next.cost
                        next.current = targetFloor
                        successors += next
                    }
                    if (l[0] == curr.current) {
                        def next = curr.clone()
                        next.locs[i][0] = targetFloor
                        ++next.cost
                        next.current = targetFloor
                        def tmp = next.clone()
                        tmp.locs = tmp.locs.sort()
                        successors += tmp
                        curr.locs.eachWithIndex { l2, i2 ->
                            if (l2[0] == curr.current && i < i2) {
                                def next2 = next.clone()
                                next2.locs[i2][0] = targetFloor
                                next2.locs = next2.locs.sort()
                                successors += next2
                            }
                        }
                    }

                    if (l[1] == curr.current) {
                        def next = curr.clone()
                        next.locs[i][1] = targetFloor
                        ++next.cost
                        next.current = targetFloor
                        def tmp = next.clone()
                        tmp.locs = tmp.locs.sort()
                        successors += tmp
                        curr.locs.eachWithIndex { l2, i2 ->
                            if (l2[1] == curr.current && i < i2) {
                                def next2 = next.clone()
                                next2.locs[i2][1] = targetFloor
                                next2.locs = next2.locs.sort()
                                successors += next2
                            }
                        }
                    }
                }
            }
        }
        frontier.addAll(successors.findAll { it.valid() && !visited.contains(it) })
    }
}

println result
