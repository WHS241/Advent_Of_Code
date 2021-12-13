#!/usr/bin/groovy

import java.util.stream.Collectors

if (this.args.length < 1)
    return

def input = GroovyCollections.transpose(new File(this.args[0]).readLines()*.toCharArray())

def counts = input*.countBy{it}*.groupBy{it.value}

def part_1 = counts*.max{it.key}*.value*.keySet().flatten().stream().map{it.toString()}.collect(Collectors.joining())
def part_2 = counts*.min{it.key}*.value*.keySet().flatten().stream().map{it.toString()}.collect(Collectors.joining())

println part_1
println part_2
