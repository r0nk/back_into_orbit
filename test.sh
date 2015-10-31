cd server
make clean
make 
./lost_lair_server &
cd ..
cd client
make clean
make
./lost_lair
