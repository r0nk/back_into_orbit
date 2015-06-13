#!/bin/bash
#convert a cube.ply file to a C function
cat $1 > cube.ply

echo "/*automatically generated*/"
echo "struct model $(echo $1){"
echo "	struct model model;"
echo "	struct vertice va[$(cat cube.ply | ./nvertices)];"
echo "	model.cardinality=$(cat cube.ply | ./nfaces);"
echo "	model.poly = calloc(sizeof(struct polygon),model.cardinality);"
./vertices 
#round it down to floating point
echo "int i;"
echo "for(i=0;i<$(cat cube.ply | ./nvertices);i++) {"
echo "	va[i].c.x/=255;"
echo "	va[i].c.y/=255;"
echo "	va[i].c.z/=255;"
echo "}"
./faces
echo "	return model;"
echo "}"

rm cube.ply

