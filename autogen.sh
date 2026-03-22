#!/bin/sh
# Run this once after cloning to generate ./configure and Makefile.in.
# Requires: autoconf, automake, pkg-config

mkdir -p autotools
autoreconf -iv
