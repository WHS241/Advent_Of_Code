#!/usr/bin/groovy

if (this.args.length == 0) {
    return
}

def input = new File(this.args[0]).readLines()*.split()

def registers = ['a': 7, 'b':0, 'c': 0, 'd': 0]
def i = 0

def getVal = {
    it.isInteger() ? it.toInteger() : registers[it]
}

while (i < input.size()) {
    def instr = input[i]
    if (instr[0] == 'jnz' && getVal(instr[1]) != 0) {
        i += getVal(instr[2])
    } else {
        if (instr[0] == 'tgl') {
            def target = i + getVal(instr[1])
            if (0 <= target && target < input.size()) {
                instr = input[target]
                if (instr.size() == 2) {
                    instr[0] = instr[0] == 'inc' ? 'dec' : 'inc'
                } else {
                    instr[0] = instr[0] == 'jnz' ? 'cpy' : 'jnz'
                }
            }
        } else if (instr[0] == 'cpy' && !instr[2].isInteger()) {
            registers[instr[2]] = getVal(instr[1])
        } else if (instr[0] == 'inc' && !instr[1].isInteger()) {
            ++registers[instr[1]]
        } else if (instr[0] == 'dec' && !instr[1].isInteger()) {
            --registers[input[i][1]]
        }
        ++i
    }
}

println registers.a
