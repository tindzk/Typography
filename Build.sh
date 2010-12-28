#!/bin/bash

mkdir -p Build/{docsgen,docslib,Jivai}

../tplgen/tplgen.bin              \
	name=Template                 \
	itf=no                        \
	add=Main:Templates/Main.html  \
	add=Body:../docslib/Body.html \
	out=Source/Template           \
	|| exit 1

../Jivai-Utils/jutils.bin        \
	build                        \
	output=docsgen.bin           \
	main=Source/Main.c           \
	manifest=Manifest.h          \
	include=..                   \
	include=../Jivai/src         \
	include=../tplgen/Include    \
	map=Source:Build/docsgen     \
	map=../Jivai/src:Build/Jivai \
	map=../docslib:Build/docslib \
	optimlevel=0                 \
	inclhdr=../Jivai/config.h    \
	dbgsym=yes                   \
	link=@bfd
