#!/usr/bin/groovy

if (this.args.length == 0) {
    return
}

class Assembunny {

    List<List<String>> instrs
    int start

    Assembunny(File f, int in) {
        this.instrs = f.readLines()*.split()
        this.start = in
    }

    boolean run() {
        def checked = 0
        def last = -1
        def i = 0
        def registers = ['a': start, 'b':0, 'c': 0, 'd': 0]

        def getVal = {
            it.isInteger() ? it.toInteger() : registers[it]
        }

        while (i < instrs.size()) {
            def instr = instrs[i]
            if (instr[0] == 'jnz' && getVal(instr[1]) != 0) {
                i += getVal(instr[2])
            } else {
                if (instr[0] == 'out') {
                    def output = getVal(instr[1])
                    if (output == last) {
                        return false
                    }
                    ++checked
                    if (checked == 20) {
                        return true
                    }
                    last = output
                } else if (instr[0] == 'tgl') {
                    def target = i + getVal(instr[1])
                    if (0 <= target && target < instrs.size()) {
                        instr = instrs[target]
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
                    --registers[instr[1]]
                }
                ++i
            }
        }
        return false
    }

}

def input = new File(this.args[0])

def i = 0
while (!new Assembunny(input, i).run()) {
    ++i
}
println i
