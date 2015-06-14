#!/bin/bash
#convert a cube.ply file to a C function

function nv(){
	grep "element $1" | grep -o "[[:digit:]]*"
}

function vertices(){
	cat cube.ply | \
		grep -v element | \
		sed -n "1,$(cat cube.ply | nv vertex)p" | \
		sed "s/ /\n/3" | \
		sed "s/ /\n/3" | \
		tr ' ' ',' | \
		sed "s/^/\{/g" | \
		sed "s/$/\}/g" | \
		sed "N;N;s/\n/,/g"  | \
		for i in $(seq 0 $(($(cat cube.ply | nv vertex)-1))) ;
	do echo "va[$i] = (struct vertice) {$(read l ; echo $l | tr ' ' '\n' )};";
	done;
}

function faces(){
	cat cube.ply | \
		grep -v element | \
		sed -n "$(($(cat cube.ply | nv vertex)+1)),$(($(cat cube.ply | nv vertex)+$(cat cube.ply | nv face)))p" | \
		sed "s/^.//g" | \
		sed "s/ /]  va[/g" | \
		sed "s/^.  //g" | \
		sed "s/  /,/g"  | \
		for i in $(seq 0 $(($(cat cube.ply | nv face)-1))) ;
	do echo "model.poly[$i] = (struct polygon) {{$(read l ; echo $l | tr ' ' '\n' )]}};";
	done;
}


grep -v -e comment -e property -e ply -e format -e end_header > cube.ply
echo "/*automatically generated*/"
echo "struct model $(echo $1){"
echo "	struct model model;"
echo "	struct vertice va[$(cat cube.ply | nv vertex)];"
echo "	model.cardinality=$(cat cube.ply | nv face);"
echo "	model.poly = calloc(sizeof(struct polygon),model.cardinality);"
vertices 
#round it down to floating point
echo "	int i;"
echo "	for(i=0;i<$(cat cube.ply | nv vertex);i++) {"
echo "		va[i].c.x/=255;"
echo "		va[i].c.y/=255;"
echo "		va[i].c.z/=255;"
echo "	}"
faces
echo "	return model;"
echo "}"

rm cube.ply


