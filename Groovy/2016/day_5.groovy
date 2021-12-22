#!/usr/bin/groovy

if (this.args.length < 1) {
    return
}

def prefix = this.args[0]

def password1 = ''
def password2 = ['g'] * 8

int i = 0
while (password1.size() < 8 || password2.indexOf('g') != -1) {
    def value = prefix + i
    def hash = value.md5()

    if (hash.indexOf(hash.find(/[^0]/)) >= 5) {
        if (password1.size() < 8) {
            password1 += hash.charAt(5)
        }

        def pos = (int)hash.charAt(5) - (int)'0'
        if (pos >= 0 && pos < 8 && password2[pos] == 'g') {
            password2[pos] = hash.charAt(6)
        }
    }
    ++i
}

println password1
println password2.join('')
