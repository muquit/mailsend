#!/bin/bash
########################################################################
# Converted README.as to README.doc using downdoc
# Add TOC using https://github.com/muquit/markdown-toc-go
########################################################################
markdown-toc-go -i doc/README.md  \
    -pre-toc-file doc/badges.md \
    -o README.md -f
