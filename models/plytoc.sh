#!/bin/bash
#convert a ply file to a C function

function nv(){
	cat temp | grep "element $1" | grep -o "[[:digit:]]*"
}

function vertices(){
	cat temp | \
		grep -v element | \
		sed -n "1,$(nv vertex)p" | \
		sed "s/ /\n/3" | \
		sed "s/ /\n/3" | \
		tr ' ' ',' | \
		sed "s/^/\{/g" | \
		sed "s/$/\}/g" | \
		sed "N;N;s/\n/,/g"  | \
		for i in $(seq 0 $(($(nv vertex)-1))) ;
	do echo "va[$i] = (struct vertice) {$(read l ; echo $l | tr ' ' '\n' )};";
	done;
}

function faces(){
	cat temp | \
		grep -v element | \
		sed -n "$(($(nv vertex)+1)),$(($( nv vertex)+$( nv face)))p" | \
		sed "s/^.//g" | \
		sed "s/ /]  va[/g" | \
		sed "s/^.  //g" | \
		sed "s/  /,/g"  | \
		for i in $(seq 0 $(($(nv face)-1))) ;
	do echo "model.poly[$i] = (struct polygon) {{$(read l ; echo $l | tr ' ' '\n' )]}};";
	done;
}

name=`echo $1 | sed "s/\.ply//g"`

cat $1 | grep -v -e comment -e property -e ply -e format -e end_header > temp
echo "/*automatically generated by plytoc.sh*/"
echo "struct model $(echo $name)_model(){"
echo "	struct model model;"
echo "	struct vertice va[$( nv vertex)];"
echo "	model.cardinality=$( nv face);"
echo "	model.poly = calloc(sizeof(struct polygon),model.cardinality);"
vertices 
#round it down to floating point
echo "	int i;"
echo "	for(i=0;i<$(nv vertex);i++) {"
echo "		va[i].c.x/=255;"
echo "		va[i].c.y/=255;"
echo "		va[i].c.z/=255;"
echo "	}"
faces
echo "	return model;"
echo "}"

rm temp
