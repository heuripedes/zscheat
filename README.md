ZSCHEAT
==========================================================================

zscheat is a ASCII to .cht compiler that accepts Pro Action Relay (PAR)
codes as input. The input file format is very simple:

* One cheat per line.
* Lines begining with `#` or `;` are assumed to be comments.
* White space is ignored everywhere but in the description.
* Cheat syntax is `code_list : description`, `code_list` can be one or more
  (up to 4) PAR codes separated by `+`.

The generated CHT file has only been tested with ZSNES.

Sample cheat file:
---------------------------------------------------------------------------

	# Front Mission - Gun Harzard (SNES)
	7E129C7F + 7E129D96 + 7E129E98 : Infinite money
	7ECF1463 + 7ECF1563 + 7ECF1663 + 7ECF1763 : Zenith V Parts
	7ED01563 + 7ED01663 : Zenith V-SP Arms

Usage
---------------------------------------------------------------------------

	./zscheat cheats.txt cheats.cht

>If you are using ZSNES, you will want the output file to have the same
>name of your ROM file, e.g. `Super Mario World (U).cht`.

vim: tw=75

