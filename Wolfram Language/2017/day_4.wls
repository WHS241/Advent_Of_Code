#!/usr/bin/env wolframscript
(* ::Package:: *)

input := Map[StringSplit, StringSplit[Import[$ScriptCommandLine[[2]]], "\n"]]


sortChars := Sort[Characters[#]]&


part1[l_] := Length[Select[l, (CountDistinct[#] == Length[#])&]]


part2[l_] := Length[Select[l, (CountDistinctBy[#, sortChars] == Length[#])&]]


Print[part1[input]]


Print[part2[input]]
