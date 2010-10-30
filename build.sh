#!/bin/sh

mkdir -p build/{docsgen,Jivai}

../tplgen/tplgen.bin       \
	name=Template          \
	itf=no                 \
	add=Main:tpl/Main.html \
	add=Body:tpl/Body.html \
	out=src/Template       \
	|| exit 1

../Jivai-Utils/jutils.bin        \
	build                        \
	output=docsgen.bin           \
	main=src/Main.c              \
	manifest=Manifest.h          \
	include=../Jivai/src         \
	include=../tplgen/include    \
	map=src:build/docsgen        \
	map=../Jivai/src:build/Jivai \
	optimlevel=0                 \
	inclhdr=config.h             \
	dbgsym=yes                   \
	link=@bfd
