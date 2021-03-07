#!/bin/sh

mips-n64-objdump --disassemble-all --source --wide --all-header --line-numbers game.out > disassembly.txt