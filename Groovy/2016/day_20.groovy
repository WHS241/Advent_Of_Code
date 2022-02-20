#!/usr/bin/groovy

if (this.args.length == 0) {
    return
}

def input = new File(this.args[0]).readLines()*.split('-')*.collect { new Long(it) }

TreeMap<Long, Long> blocked = new TreeMap<Long, Long>()

input.each {
    def first = new Long(it[0])
    def second = new Long(it[1])
    def low = Math.min(first, second)
    def high = Math.max(first, second)

    if (blocked.containsKey(low)) {
        def tmp = blocked[low]
        if (tmp < low) {
            blocked[tmp] = high
            blocked.remove(low)
            low = tmp
        } else if (tmp < high) {
            blocked.remove(tmp)
            blocked[low] = high
        } else {
            return
        }
    } else {
        blocked[low] = high
        def lower = blocked.lowerEntry(low)
        if (lower != null) {
            if (lower.value >= high) {
                blocked.remove(low)
                return
            }
            if (lower.value > low) {
                blocked.remove(low)
                blocked.remove(lower.value)
                blocked[lower.key] = high
                low = lower.key
            }
        }
    }

    if (blocked.containsKey(high)) {
        def tmp = blocked[high]
        if (tmp > high) {
            blocked[tmp] = low
            blocked[low] = tmp
            blocked.remove(high)
            high = tmp
        } else {
            blocked.remove(tmp)
            blocked[high] = low
        }
    } else {
        def higher = blocked.higherEntry(high)
        if (higher != null && higher.value <= high) {
            blocked[low] = higher.key
            blocked.remove(higher.value)
            blocked[higher.key] = low
            high = higher.key
        } else {
            blocked[high] = low
        }
    }

    if (low != high) {
        def mid = blocked.higherKey(low)
        while (mid != high) {
            blocked.remove(mid)
            mid = blocked.higherKey(mid)
        }
    }
}

def count = 0
def foundMin = false
Long i = 0
while (i <= 4294967295) {
    if (blocked[i] == null) {
        if (!foundMin) {
            println i
            foundMin = true
        }
        ++count
    } else {
        i = blocked[i]   
    }
    ++i
}

println count
