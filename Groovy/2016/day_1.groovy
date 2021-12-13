#!/usr/bin/groovy

import java.io.File

class Segment {
    int x1
    int y1
    int x2
    int y2

    Segment(int x1, int y1, int x2, int y2) {
        this.x1 = x1
        this.y1 = y1
        this.x2 = x2
        this.y2 = y2
    }

    boolean intersects(Segment other) {
        if (this.x1 == this.x2) {
            if (other.x1 == other.x2)
                return this.x1 == other.x1 && ((this.y1 <= other.y1 && other.y1 <= this.y2) || (this.y1 <= other.y2 && other.y2 <= this.y2) || (other.y1 <= this.y1 && this.y2 <= other.y2))
            else
                return this.y1 <= other.y1 && other.y1 <= this.y2 && other.x1 <= this.x1 && this.x1 <= other.x2
        } else {
            if (other.y1 == other.y2)
                return this.y1 == other.y1 && ((this.x1 <= other.x1 && other.x1 <= this.x2) || (this.x1 <= other.x2 && other.x2 <= this.x2) || (other.x1 <= this.x1 && this.x2 <= other.x2))
            else
                return this.x1 <= other.x1 && other.x1 <= this.x2 && other.y1 <= this.y1 && this.y1 <= other.y2
        }
    }
}

if (this.args.length == 0)
    return

def input = new File(this.args[0]).text.split(',')*.trim()

def x = 0, y = 0, dir = 0
List<Segment> visited = new ArrayList<>()
boolean found = false

int intersection = 0

for (String step in input) {
    def lowX = x, lowY = y, highX = x, highY = y;
    def dist = Integer.valueOf(step.substring(1))
    switch (step.charAt(0)) {
        case 'R':
            dir = (dir + 1) % 4
            break

        case 'L':
            dir = (dir + 3) % 4
            break
    }

    switch (dir) {
        case 0:
        x += dist
        highX = x
        break

        case 1:
        y += dist
        highY = y
        break

        case 2:
        x -= dist
        lowX = x
        break

        case 3:
        y -= dist
        lowY = y
        break

        default:
        break
    }

    def curr = new Segment(lowX, lowY, highX, highY)

    if (!found) {
        def ix = x, iy = y
        for (Segment s in visited) {
            if (s != visited.last() && curr.intersects(s)) {
                found = true

                switch (dir) {
                    case 0:
                    if (s.x1 < curr.x1)
                        ix = curr.x1
                    else if (s.x1 < ix)
                        ix = s.x1
                    break;

                    case 1:
                    if (s.y1 < curr.y1)
                        iy = curr.y1
                    else if (s.y1 < iy)
                        iy = s.y1
                    break;

                    case 2:
                    if (s.x2 > curr.x2)
                        ix = curr.x2
                    else if (s.x2 > ix)
                        ix = s.x2
                    break;

                    case 3:
                    if (s.y2 > curr.y2)
                        iy = curr.y2
                    else if (s.y2 > iy)
                        iy = s.y2
                    break;
                }
            }
        }

        if (found) {
            intersection = Math.abs(ix) + Math.abs(iy)
        }

        visited.add(curr)
    }
}

println "Part 1: " + (Math.abs(x) + Math.abs(y))
println "Part 2: " + intersection
