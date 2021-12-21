#!/usr/bin/groovy

if (this.args.length == 0)
    return

def input = new File(this.args[0]).readLines()

long getDecompressedLength(String compressed, boolean recurse) {
    def startCompress = compressed.indexOf("(")
    if (startCompress == -1)
        return compressed.length()
    
    def endCompress = compressed.indexOf(")")
    def compressInstr = compressed.substring(startCompress + 1, endCompress)
    def parsedInstr = compressInstr.split("x")*.toInteger()
    def compressSize = parsedInstr[0]
    def decompressMult = parsedInstr[1]

    long substrLength = decompressMult * (recurse ? getDecompressedLength(compressed.substring(endCompress + 1, endCompress + compressSize + 1), true) : compressSize)

    return substrLength + getDecompressedLength(compressed.substring(endCompress + compressSize + 1), recurse)
}

println getDecompressedLength(input[0], false)
println getDecompressedLength(input[0], true)
