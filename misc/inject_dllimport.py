#!/usr/bin/env python2.7
import sys
import re

replace_map = [ \
  (r'external global', 'external dllimport global'), \
  (r'external constant', 'external dllimport constant'), \
  (r'@_TWVBo = external dllimport', '@_TWVBo = external'), \
  (r'@_TWVBo', '@__imp__TWVBo') \
  ]

newf = open(sys.argv[2], "w")
with open(sys.argv[1], "r") as sources:
  lines = sources.readlines()
  for line in lines:
    newline = line
    for (exp, newstr) in replace_map:
      newline = re.sub(exp, newstr, newline)
#    print newline,
    newf.write(newline)
newf.close()
