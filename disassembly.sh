#!/bin/sh

mips64-elf-objdump --disassemble-all --source --wide --all-header --line-numbers game.out > disassembly.txt