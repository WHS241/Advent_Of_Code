#!/usr/bin/groovy

if (this.args.length == 0)
    return

def input = this.args[0]

int count = 0
int curr = -1

def hexChars = ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f']

while (count != 64) {
    ++curr
    if (hexChars.any { digit ->
        (input + curr).md5().contains(digit * 3) && ((curr + 1)..(curr + 1000)).any { c ->
            if ((input + c).md5().contains(digit * 5)) {
            }
            (input + c).md5().contains(digit * 5)
        }
    }) {
        println curr
        ++count
    }
}

println curr