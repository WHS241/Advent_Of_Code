#!/usr/bin/node
const fs = require('fs')

let sum = x => {
    if (Array.isArray(x)) {
        return x.reduce((accumulator, currentValue) => accumulator + sum(currentValue), 0)
    } else if (typeof x === 'object') {
        let total = 0;
        for (const k in x) {
            if (x[k] === "red")
                return 0;

            total += sum(x[k]);
        }
        return total;
    } else if (typeof x === 'number') {
        return x;
    } else {
        return 0;
    }
}

fs.readFile("input/day12", 'utf8', (err, jsonStr) => {
    if (!err) {
        const json = JSON.parse(jsonStr)
        console.log(sum(json))
    }
})
