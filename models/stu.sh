	 cat tetra.ply | ./plytoc.sh  | grep struct | tr '\n' ']' | sed "s/\]/\];\n/g" | sed "s/\[ /\[/g"
