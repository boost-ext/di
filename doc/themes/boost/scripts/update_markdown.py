#!/usr/bin/env python
#update_readme_toc.py doc/mkdocs.yml README.md http://boost-experimental.github.io/msm-lite

import fileinput, sys, os

for line in fileinput.input(sys.argv[1], inplace=True):
  print(line)
