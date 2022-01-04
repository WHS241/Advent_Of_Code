#!/usr/bin/groovy

if (this.args.length == 0) {
    return
}

def input = new File(this.args[0]).readLines()*.split(' ')

def registers = ['a': 0, 'b':0, 'c': 0, 'd': 0]

def i = 0

while (i < input.size()) {
    if (input[i][0] == 'jnz' && registers[input[i][1]] != 0) {
        i += input[i][2].toInteger()
    } else {
        if (input[i][0] == 'cpy') {
            def in = input[i][1].isInteger() ? input[i][1].toInteger() : registers[input[i][1]]
            registers[input[i][2]] = in
        } else if (input[i][0] == 'inc') {
            ++registers[input[i][1]]
        } else if (input[i][0] == 'dec') {
            --registers[input[i][1]]
        }
        ++i
    }
}

println registers.a

registers = ['a': 0, 'b':0, 'c': 1, 'd': 0]

i = 0

while (i < input.size()) {
    if (input[i][0] == 'jnz' && registers[input[i][1]] != 0) {
        i += input[i][2].toInteger()
    } else {
        if (input[i][0] == 'cpy') {
            def in = input[i][1].isInteger() ? input[i][1].toInteger() : registers[input[i][1]]
            registers[input[i][2]] = in
        } else if (input[i][0] == 'inc') {
            ++registers[input[i][1]]
        } else if (input[i][0] == 'dec') {
            --registers[input[i][1]]
        }
        ++i
    }
}

println registers.a
