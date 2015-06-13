function scr(){
 cat tetra.ply | \
 ./plytoc.sh  | \
 grep "struct vertice" | \
 grep -o "[[:digit:]]*" 
}

function pcr(){
 cat tetra.ply | \
 ./plytoc.sh  | \
 grep "struct polygon" | \
 grep -o "[[:digit:]]*" 
}

cat tetra.ply | \
./plytoc.sh | \
 grep -v struct | \
 for i in $(seq 1 $(scr)) ;
	 do echo "va[$i] = (struct vertice) {$(read l ; echo $l | tr ' ' ',' )};";
 done;

cat tetra.ply | \
./plytoc.sh | \
 grep -v struct | \
 for i in $(seq $(scr) $(($(scr) + $(pcr)))) ;
	 do echo "pa[$i] = (struct polygon) {$(read l ; echo $l | tr ' ' ',' )};";
 done 

