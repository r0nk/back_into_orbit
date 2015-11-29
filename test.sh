cd server
make clean
make 
cd ..
cd overlord
make clean
make 
./overlord &
cd ..
cd client
make clean
make -j8
./dr0ne

echo "killing overlord"
pkill overlord
