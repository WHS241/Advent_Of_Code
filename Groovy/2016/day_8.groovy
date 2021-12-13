#!/usr/bin/groovy

if (this.args.length == 0)
    return

def input = new File(this.args[0]).readLines()*.split(" ")

def screen = new char[6][50]

