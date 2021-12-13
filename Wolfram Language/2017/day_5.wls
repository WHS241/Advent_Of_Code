#!/usr/bin/env wolframscript
(* ::Package:: *)

input := ReadList[OpenRead[$ScriptCommandLine[[2]]], Number]





part1Helper[l_, pos_, acc_] := If[pos <= 0 || pos > Length[l], acc, part1Helper[l + UnitVector[Length[l], pos], pos + l[[pos]], acc + 1]]


part1[l_] := Block[{$IterationLimit = 2^32}, part1Helper[l, 1, 0]]


Print[part1[input]]


part2Helper[l_, pos_, acc_] :=
    If[pos <= 0 || pos > Length[l],
        acc
        ,
        part2Helper[l + UnitVector[Length[l], pos] * If[l[[pos]] >= 3,
                -1
                ,
                1
            ], pos + l[[pos]], acc + 1
        ]
    ]


part2[l_] := Block[{$IterationLimit = 2^32}, part2Helper[l, 1, 0]]


Print[part2[input]]
