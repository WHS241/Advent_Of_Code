#!/usr/bin/env wolframscript
(* ::Package:: *)

input := ToExpression[$ScriptCommandLine[[2]]]


coords[x_] := Module[
	{c = Floor[Sqrt[x - 1]]},
	Module[
		{s = c^2},
		If[EvenQ[c], {1 - c/2, c/2} + {-1 + Max[0, x - s - c - 1], -Min[x - s - 1, c]}, {(c - 1) / 2, (1 - c) / 2} + {1 - Max[0, x - s - c - 1], Min[x - s - 1, c]}]
		]]


part1[x_] := Total[Abs[coords[x]]]


nextCoords[x_, y_] := 
	Which[y <= 0 && Abs[y] >= Abs[x], {x + 1, y}, x <= 0 && Abs[x] >= Abs[y], {x, y - 1}, Abs[y] >= Abs[x], {x - 1, y}, True, {x, y + 1}]


getValue[x_, y_, table_] := ReleaseHold[If[table[[x, y]] != 0, table[[x, y]], Hold[Sum[table[[x + i, y + j]], {i, -1, 1}, {j, -1, 1}]]]]


findThreshold[x_, y_, centerX_, centerY_, table_, target_] := Module[{
curr = ReleaseHold[If[x == 0 && y == 0, 1, Hold[getValue[x + centerX, y + centerY, table]]]], temp = table},
temp[[x + centerX, y + centerY]] = curr;
ReleaseHold[If[curr > target, curr, Hold[findThreshold@@Join[nextCoords[x, y], {centerX, centerY, temp, target}]]]]]


part2[x_] := Module[
{squareSize = Floor[Log[2, x]] + 5},
Module[
{c = Floor[(squareSize + 1) / 2], $IterationLimit = squareSize^2},
findThreshold[0, 0, c, c, Table[0, {i, squareSize}, {j, squareSize}], x]]]


Print[part1[input]]


Print[part2[input]]
