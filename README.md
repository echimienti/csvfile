# csvfile
CSV File handling with multi dimensional vector <br />
This module is used as submodule in addressbook but can be used by anyone <br />

# For make test install/configure first:
sudo apt-get install libgtest-dev <br />
sudo apt install cmake <br />
cd /usr/src/gtest <br />
sudo cmake CMakeLists.txt <br />
sudo make <br />
sudo cp *.a /usr/lib <br />
<br />
make test <br />



# Checkout code:
git clone https://github.com/echimienti/csvfile.git <br />


# For compiling:
on Ubuntu: sudo apt install g++ <br />
on Centos: sudo yum group install "Development Tools" <br />
cd addressbook <br />
make <br />


# For make test install/configure first:
sudo apt-get install libgtest-dev <br />
sudo apt install cmake <br />
cd /usr/src/gtest <br />
sudo cmake CMakeLists.txt <br />
sudo make <br />
sudo cp *.a /usr/lib <br />
<br />
make test <br />


# UTF16-LE encoded csv files
UTF16-LE encoded csv file are automatically detected and converted to UTF8 <br />
