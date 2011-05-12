#!/bin/bash

mkdir -p Build/{docsgen,Domain,Jivai}

../tplgen/tplgen.bin             \
	name=Template                \
	itf=no                       \
	add=Main:Templates/Main.html \
	add=Body:Domain/Body.html    \
	out=Source/Template          \
	|| exit 1

../Depend/Depend.bin             \
	build                        \
	output=docsgen.bin           \
	main=Source/Main.c           \
	manifest=Manifest.h          \
	include=.                    \
	include=..                   \
	include=../Jivai/src         \
	include=../tplgen/Include    \
	map=Source:Build/docsgen     \
	map=../Jivai/src:Build/Jivai \
	map=Domain:Build/Domain      \
	optimlevel=0                 \
	inclhdr=../Jivai/config.h    \
	dbgsym=yes                   \
	link=@bfd                    \
	link=@dl
