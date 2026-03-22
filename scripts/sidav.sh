
# sh scripts/sidav.sh
# ...Ubuntu/Kubuntu LTS...  (~Debian)

# sudo apt update
# sudo apt upgrade
# sudo apt install git
# sudo apt install cmake
# sudo apt install build-essential autoconf automake libtool libsdl2-dev libsdl2-mixer-dev libsdl2-image-dev libsdl2-ttf-dev libmagic-dev
# make clean
sh autogen.sh
./configure
make
sudo make install
# cd assets && ReMoMber

# cp ~/MPS/MAGIC.LBX assets/
# cp ~/MPS/MAGIC.MOM assets/
# cp ~/MPS/MAGIC.SET assets/
# cp ~/MPS/MAGIC.GAM assets/

# ...if SIGSEGV, then:
# gdb -batch -ex run -ex bt ReMoMber
