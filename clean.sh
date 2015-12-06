cd server
make clean
make 
cd ..
cd overlord
make clean
make 
cd ..
cd client
make clean
make -j8

echo "cleaned"
