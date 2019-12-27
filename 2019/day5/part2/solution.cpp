#include<iostream>
#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<vector>
#include<array>
#include<functional>

typedef std::vector<int> vi;
typedef std::array<int, 4> ai;

typedef std::function<bool (const int&, const int&)> fb1;
typedef std::function<bool (const int&)> fb2;


class IntCode{
  public:
    IntCode(vi*);
    ~IntCode() = default;
    void compute();

  private:
    int index{0}, rawInstruction{0}, option{0};
    ai instruction{0,0,0,0};
    int valA{0}, valB{0}, indexVal{0};
    int const size{0};
    vi* list{nullptr};

    void sum();
    void mult();
    void store(int&);
    void store(fb1);
    void output();
    void jump(fb2, int&);

    void parse(int&);
};



inline void IntCode::mult(){
  option = instruction[2];
  switch(option){
    case 0:
      indexVal = list->at(index+1);
      valA = list->at(indexVal);
      break;
    case 1:
      valA = list->at(index+1);
      break;
  }

  option = instruction[1];
  switch(option){
    case 0:
      indexVal = list->at(index+2);
      valB = list->at(indexVal);
      break;
    case 1:
      valB = list->at(index+2);
      break;
  }

  indexVal = list->at(index+3);
  list->at(indexVal) = valA * valB;
}

inline void IntCode::sum(){
  option = instruction[2];
  switch(option){
    case 0:
      indexVal = list->at(index+1);
      valA = list->at(indexVal);
      break;
    case 1:
      valA = list->at(index+1);
      break;
  }

  option = instruction[1];
  switch(option){
    case 0:
      indexVal = list->at(index+2);
      valB = list->at(indexVal);
      break;
    case 1:
      valB = list->at(index+2);
      break;
  }

  indexVal = list->at(index+3);
  list->at(indexVal) = valA + valB;
}

inline void IntCode::store(int& input){
  indexVal = list->at(index+1);
  list->at(indexVal) = input;
}

inline void IntCode::store(fb1 check){
  option = instruction[2];
  switch(option){
    case 0:
      indexVal = list->at(index+1);
      valA = list->at(indexVal);
      break;
    case 1:
      valA = list->at(index+1);
      break;
  }

  option = instruction[1];
  switch(option){
    case 0:
      indexVal = list->at(index+2);
      valB = list->at(indexVal);
      break;
    case 1:
      valB = list->at(index+2);
      break;
  }

  indexVal = list->at(index+3);
  if(check(valA, valB)) list->at(indexVal) = 1;
  else list->at(indexVal) = 0;
}

inline void IntCode::output(){
  option = instruction[2];
  switch(option){
    case 0:
      indexVal = list->at(index+1);
      std::cout<<list->at(indexVal)<<'\n';
      break;
    case 1:
      std::cout<<list->at(index+1)<<'\n';
      break;
  }
}

inline void IntCode::jump(fb2 check, int& index){
  option = instruction[2];
  switch(option){
    case 0:
      indexVal = list->at(index+1);
      valA = list->at(indexVal);
      break;
    case 1:
      valA = list->at(index+1);
      break;
  }

  option = instruction[1];
  switch(option){
    case 0:
      indexVal = list->at(index+2);
      valB = list->at(indexVal);
      break;
    case 1:
      valB = list->at(index+2);
      break;
  }
  if(check(valA)) index = valB;
  else index += 3;
}

inline void IntCode::parse(int& op){
  instruction[3] = rawInstruction % 100;
  rawInstruction = static_cast<int>(rawInstruction/100);
  op = instruction[3];

  instruction[2] = rawInstruction % 10;
  rawInstruction = static_cast<int>(rawInstruction/10);

  instruction[1] = rawInstruction % 10;
  rawInstruction = static_cast<int>(rawInstruction/10);

  instruction[0] = rawInstruction % 10;
  rawInstruction = static_cast<int>(rawInstruction/10);
}

void IntCode::compute(){
  int incremental{0}, input{5}, opcode{0};
  bool jumped{false};

  do{
    rawInstruction = list->at(index);
    parse(opcode);

    switch(opcode){
      case 1:
        sum();
        incremental = 4;
        break;
      case 2:
        mult();
        incremental = 4;
        break;
      case 3:
        store(input);
        incremental = 2;
        break;
      case 4:
        output();
        incremental = 2;
        break;
      case 5:
        jump([](const int& a){return a != 0;}, index);
        jumped = true;
        break;
      case 6:
        jump([](const int& a){return a == 0;}, index);
        jumped = true;
        break;
      case 7:
        store([](const int& a, const int& b){return a < b;});
        incremental = 4;
        break;
      case 8:
        store([](const int& a, const int& b){return a == b;});
        incremental = 4;
        break;
    }

    if(!jumped) index += incremental;
    else jumped = false;

  } while(opcode != 99);
}


IntCode::IntCode(vi* l):list(l), size(l->size()){
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

  IntCode ic{&nums};
}
