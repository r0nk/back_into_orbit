cd server
make clean
make 
./dr0ne_server &
cd ..
cd client
make clean
make -j8
./dr0ne
