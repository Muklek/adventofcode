#include<iostream>
#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<vector>

typedef std::vector<int> vi;


class InstructionsReader{
  public:
    InstructionsReader(vi*);
    ~InstructionsReader() = default;
    void compute();
  private:
    int index{0}, opcode{0};
    int valA{0}, valB{0}, indexVal{0};
    int const size{0};
    vi* list{nullptr};

    void sumOperation();
    void multOperation();
};

void InstructionsReader::multOperation(){
  indexVal = list->at(index+1);
  valA = list->at(indexVal);

  indexVal = list->at(index+2);
  valB = list->at(indexVal);

  indexVal = list->at(index+3);

  list->at(indexVal) = valA * valB;
}

void InstructionsReader::sumOperation(){
  indexVal = list->at(index+1);
  valA = list->at(indexVal);

  indexVal = list->at(index+2);
  valB = list->at(indexVal);

  indexVal = list->at(index+3);

  list->at(indexVal) = valA + valB;
}

void InstructionsReader::compute(){

  do{

    if(opcode == 1) sumOperation();
    else if(opcode == 2) multOperation();
    index += 4;
    opcode = list->at(index);

  } while(index < size && opcode != 99);
}

InstructionsReader::InstructionsReader(vi* l)
  :list(l), size(l->size()), opcode(l->at(index)){
  compute();
};


int main(){
  std::string line;
  std::ifstream file;

  vi nums;

  file.open("input.txt");

  while(std::getline(file, line, ',')){
    int number{0};
    std::istringstream(line) >> number;
    nums.push_back(number);
  }

  file.close();

  nums[1] = 12;
  nums[2] = 2;
  InstructionsReader ir{&nums};
  std::cout<<nums[0]<<'\n';

}
