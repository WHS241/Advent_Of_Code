#!/usr/bin/env wolframscript
(* ::Package:: *)

input = 
	Module[{raw = ReadList[OpenRead[$ScriptCommandLine[[2]]], String]},
	Table[ReadList[StringToStream[raw[[i]]], Number], {i, Length[raw]}]]


part1[x_] := Total[Table[Max[x[[i]]] - Min[x[[i]]], {i, Length[x]}]]


part2[x_] := Total[Table[Max[Table[Max[Table[If[j != k && Divisible[x[[i]][[j]], x[[i]][[k]]], x[[i]][[j]] / x[[i]][[k]], 0], {k, Length[x[[i]]]}]], {j, Length[x[[i]]]}]], {i, Length[x]}]]


Print[part1[input]]


Print[part2[input]]
