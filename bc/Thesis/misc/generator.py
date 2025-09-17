# @ Author: Ondřej Koumar
# @ Email: xkouma02@stud.fit.vutbr.cz
# @ Create Time: 2024-04-02 12:31
# @ Modified time: 2024-05-02 20:15

import os
nonterminals = [
    "program      ",
    "statementList",
    "statement    ",
    "if2          ",
    "declOrExp    ",
    "returnExp    ",
    "functionDef  ",
    "params       ",
    "params2      ",
    "expression   ",
    "args         ",
    "args2        ",
    "codeBlock    ",
    "statements   ",
    "volType      ",
    "type         "
]

with open(os.path.dirname(os.path.abspath(__file__)) + "/lltable.txt", 'r') as f1:
    with open(os.path.dirname(os.path.abspath(__file__)) + "/vector.txt", 'w') as f2:
        f2.write(
            "const std::vector<std::vector<LLTableIndex>> LLTable::table = {\n")
        lines = f1.readlines()
        f2.write(f"//\t\t\t\t\t{lines[0]}")
        for i in range(1, len(lines)):
            f2.write(f"/* {nonterminals[i-1]} */")
            f2.write("\t{ ")
            words = lines[i].split('\t')
            for word in words:
                end = False
                if word.endswith("\n"):
                    word = word.rstrip("\n")
                    end = True
                x, y = map(int, word.strip("()").split(','))
                if end:
                    f2.write(f"{{ {x}, {y} }} ")
                else:
                    f2.write(f"{{ {x}, {y} }}, ")
            f2.write("},\n")
        f2.write("};")
