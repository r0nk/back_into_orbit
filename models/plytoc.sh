grep -v -e comment -e property -e ply -e format -e end_header | \
 sed "s/element vertex/struct vertice va\[/g" | \
 sed "s/element face/struct polygon pa\[/g"
