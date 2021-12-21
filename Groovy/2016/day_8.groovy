#!/usr/bin/groovy

if (this.args.length == 0)
    return

def input = new File(this.args[0])

def screen = (1..6).collect({ [' '] * 50 })

input.eachLine({ line ->
    def parsed = line.split(" ")
    if (parsed[0] == "rect") {
        def vals = parsed[1].split("x")*.toInteger()
        for (i in (0..<vals[1]))
            for (j in (0..<vals[0]))
                screen[i][j] = '\u2588'
    } else if (parsed[1] == "column") {
        def colNum = parsed[2].split("=")[1].toInteger()
        def shiftVal = parsed[4].toInteger() % 6
        def numRows = screen.size()
        while (shiftVal > 0 && numRows > 1) {
            for (i in (numRows - 1)..shiftVal) {
                char tmp = screen[i][colNum]
                screen[i][colNum] = screen[i - shiftVal][colNum]
                screen[i - shiftVal][colNum] = tmp
            }
            def nextRows = shiftVal
            shiftVal = ((shiftVal - 1) * numRows) % shiftVal
            numRows = nextRows
        }
    } else if (parsed[1] == "row") {
        def rowNum = parsed[2].split("=")[1].toInteger()
        def shiftVal = parsed[4].toInteger() % 50
        Collections.rotate(screen[rowNum], shiftVal)
    }
})

for (row in screen)
    println row.join("")


println screen*.count { it != ' ' }.sum()
