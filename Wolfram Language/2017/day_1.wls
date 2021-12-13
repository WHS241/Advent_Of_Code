#!/usr/bin/env wolframscript
(* ::Package:: *)

input := IntegerDigits[Read[OpenRead[$ScriptCommandLine[[2]]], Number]]


part1[l_] := Total[Table[If[l[[i]] == RotateLeft[l][[i]], l[[i]], 0], {i, Length[l]}]]


part2[l_] := Total[Table[If[l[[i]] == RotateLeft[l, Length[l] / 2][[i]], l[[i]], 0], {i, Length[l]}]]





Print[part1[input]]


Print[part2[input]]
