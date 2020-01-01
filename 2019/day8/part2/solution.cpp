#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<climits>
#include<array>

typedef std::string st;
typedef std::array<char, 150> ac;

class ImageDecoder {
  public:
    ImageDecoder(const st* const);
    ImageDecoder operator=(ImageDecoder) = delete;
    ImageDecoder operator=(st) = delete;
    ~ImageDecoder() = default;

    int getWeight();
    void printImage();

  private:
    const st* const input{nullptr};
    int weight{0};
    ac image;

    void run();
    void findWeight();
    void generateImage();

};

inline int ImageDecoder::getWeight(){
  return weight;
}

inline void ImageDecoder::printImage(){
  for(const char& pixel : image)
    std::cout<<pixel;
  std::cout<<'\n';
}

inline void ImageDecoder::findWeight(){
  int itemCount{0};
  int oneCount{0}, twoCount{0};
  int globalZero{INT_MAX}, localZero{0};


  for(int i{0}; i<input->size(); i++){
    ++itemCount;

    switch(input->at(i)){
      case '0':
        ++localZero;
        break;
      case '1':
        ++oneCount;
        break;
      case '2':
        ++twoCount;
        break;
    }

    if(itemCount == 150){
      if(localZero < globalZero) {
        weight = oneCount * twoCount;
        globalZero = localZero;
      }
      localZero = 0;
      oneCount = 0;
      twoCount = 0;
      itemCount = 0;
    }
  }
}

inline void ImageDecoder::generateImage(){

  // initilize layer as invisible
  for(char& pixel : image) pixel = '2';

  // overwrite layer based on pixel density
  for(int i{0}, pixel{0}; i<input->size(); i++, pixel++){
    if(image[pixel] == '2') image[pixel] = input->at(i);
    if(pixel == 149) pixel = -1;
  }
}

inline void ImageDecoder::run(){
  findWeight();
  generateImage();
}

inline ImageDecoder::ImageDecoder(const st* const i):input(i){
  run();
};


int main(){
  st line{""};
  std::fstream file;

  file.open("input.txt");
  std::getline(file, line);
  file.close();

  ImageDecoder id{&line};
  std::cout<<id.getWeight()<<'\n';
  id.printImage();
}

