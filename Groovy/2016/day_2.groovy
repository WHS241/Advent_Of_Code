#!/usr/bin/groovy

if (this.args.length == 0)
    return

def input = new File(this.args[0])
Character[][] PAD_1 = [['1', '2', '3'], ['4', '5', '6'], ['7', '8', '9']]
Character[][] PAD_2 = [['0', '0', '1', '0', '0'], ['0', '2', '3', '4', '0'], ['5', '6', '7', '8', '9'], ['0', 'A', 'B', 'C', '0'], ['0', '0', 'D', '0', '0']]

def pass_1 = "", pass_2 = ""

input.eachLine({ line ->
    int x1 = 1, y1 = 1, x2 = 2, y2 = 0

    line.each({
        switch(it) {
            case 'U':
            if (x1 != 0) {
                --x1
            }
            if (x2 != 0 && PAD_2[x2 - 1][y2] != '0') {
                --x2
            }
            break

            case 'D':
            if (x1 + 1 != PAD_1.length) {
                ++x1
            }
            if (x2 + 1 != PAD_2.length && PAD_2[x2 + 1][y2] != '0') {
                ++x2
            }
            break

            case 'L':
            if (y1 != 0) {
                --y1
            }
            if (y2 != 0 && PAD_2[x2][y2 - 1] != '0') {
                --y2
            }
            break

            case 'R':
            if (y1 + 1 != PAD_1.length) {
                ++y1
            }
            if (y2 + 1 != PAD_2.length && PAD_2[x2][y2 + 1] != '0') {
                ++y2
            }
            break

            default:
            break
        }
    })

    pass_1 += PAD_1[x1][y1]
    pass_2 += PAD_2[x2][y2]
})

println pass_1
println pass_2
