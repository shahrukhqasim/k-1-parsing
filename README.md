This folder contains code for parsing K-1 form. Here are the instructions to set it up on a Linux box, build dependencies and run the code

# Install Ubuntu 16.04 LTS
# Install basic dev packages
sudo apt-get install gcc vim emacs xzgv imagemagick automake1.9 python-scitools python-matplotlib python-numpy python-scipy subversion mercurial pdftk libtool autoconf zip unzip cmake scons

# Build and install Leptonica 1.73
sudo apt-get install libpng12-dev libtiff5-dev zlib1g-dev libjpeg-dev
wget http://www.leptonica.com/source/leptonica-1.73.tar.gz
tar -xvf leptonica-1.73.tar.gz 
cd leptonica-1.73/
autoreconf 
autoreconf -fvi

#automake --add-missing


./configure 
make -j 4
sudo make install

cd

# Build and install Tesseract 3.05dev
git clone https://github.com/tesseract-ocr/tesseract.git
cd tesseract
./autogen.sh 
./configure
make -j 4
sudo make install

# Build and install OpenCV 3.1
wget https://github.com/Itseez/opencv/archive/3.1.0.zip
unzip opencv-3.1.0.zip
cd opencv-3.1.0/
mkdir release
cd release
cmake ..
./configure
make -j 4
sudo make install

cd

# Build the code
git clone https://gitlab.com/fshafait/k1-parsing.git
cd k1-parsing
mkdir release
cd release
cmake ..
./configure
make

# To run accuracy tests of FineReader OCR result already present in json files
./k1_parsing -a ../data/accuracy/

# To run Tesseract OCR with necessary pre-processing and then run accuracy tests
./k1_parsing -a ../data/accuracy/
./k1_parsing -o ../data/ocr_and_accuracy/

