#include<iostream>
#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<vector>

typedef std::vector<int> vi;


class InstructionsReader{
  public:
    InstructionsReader(vi*, int&, int&, int&);
    ~InstructionsReader() = default;
    bool targetCompute();
  private:
    int index{0}, opcode{0};
    int valA{0}, valB{0}, indexVal{0};
    int noun{0}, verb{0}, target{0};
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

bool InstructionsReader::targetCompute(){

  list->at(1) = noun;
  list->at(2) = verb;

  while(index < size && opcode != 99){
    if(opcode == 1) sumOperation();
    else if(opcode == 2) multOperation();
    index += 4;
    opcode = list->at(index);

  }

  return list->at(0) == target;
}

InstructionsReader::InstructionsReader(vi* l, int& a, int& b, int& t)
  :list(l), size(l->size()), opcode(l->at(index)), noun(a), verb(b), target(t){};


inline void getCombinations(const vi& nums, int& t){
  int a{0}, b{0};
  for(int i{0}; i<=99; i++){
    for(int j{0}; j<=99; j++){
      vi copy{nums.begin(), nums.end()};
      InstructionsReader ir{&copy, i, j, t};
      if(ir.targetCompute()){
        a = i;
        b = j;
        goto endloop;
      }
    }
  }
  endloop:
    std::cout<<"noun: "<<a<<" verb: "<<b<<'\n';
}

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

  int target{19690720};
  getCombinations(nums, target);

}
