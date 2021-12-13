#!/usr/bin/groovy

if (this.args.length < 1)
    return

def prefix = this.args[0]

def password_1 = ""
def password_2 = ['g']*8

int i = 0
while (password_1.size() < 8 || password_2.indexOf('g') != -1) {
    def value = prefix + i
    def hash = value.md5()

    if (hash.indexOf(hash.find(/[^0]/)) >= 5) {
        if (password_1.size() < 8)
            password_1 += hash.charAt(5)

        def pos = (int)hash.charAt(5) - (int)'0'
        if (pos >= 0 && pos < 8 && password_2[pos] == 'g') {
            password_2[pos] = hash.charAt(6)
        }
    }
    ++i
}

println password_1
println password_2.join("")
