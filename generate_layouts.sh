lim=$(ls layouts/*.layout -1 | wc -l)


echo "#include \"layouts.h\""
echo "#include \"room.h\""
echo "/*AUTOMATICALLY GENERTED BY generate_layouts.sh */"

echo "void pick_layout(struct room * room)"
echo "{"
echo "#define N_LAYOUTS " $(echo $lim)
echo "int r = (rand()%N_LAYOUTS); "
echo "	switch (r) {"
ls layouts/*.layout -1 | sed 's/.*/case magic: get_layout(room,"&"); break;/' \
	| awk '{gsub("magic",NR,$0);print}'
echo "	}"
echo "}"
