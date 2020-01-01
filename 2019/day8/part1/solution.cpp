#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<climits>

typedef std::string st;

class ImageDecoder {
  public:
    ImageDecoder(st*);
    ~ImageDecoder() = default;

    int getWeight();

  private:
    st* input{nullptr};
    int weight{0};

    void run();

};

inline int ImageDecoder::getWeight(){
  return weight;
}

inline void ImageDecoder::run(){
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

ImageDecoder::ImageDecoder(st* i):input(i){
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
}

