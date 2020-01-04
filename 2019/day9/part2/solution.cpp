#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<array>

typedef std::vector<int64_t> vi;
typedef std::array<int64_t, 4> ai;


class IntCode{
  public:
    IntCode(vi*, int64_t);
    IntCode operator=(auto) = delete;
    ~IntCode() = default;

    void compute();

  private:
    int64_t index{0}, raw{0}, incremental{0};
    int64_t base{0}, option{0}, position{0};
    int64_t paramA{0}, paramB{0};
    bool jumped{false};
    int const input{0};
    ai instruction{0,0,0,0};
    vi* list{nullptr};

    void sum();
    void mult();
    void store();
    void store(bool);
    void output();
    void jump(bool, int64_t&);
    void setBase();

    void parse();
    void readParam(int);
    void readPosition(int, int);
    void checkCapacity(int64_t);
};



inline void IntCode::mult(){
  list->at(position) = paramA * paramB;
  incremental = 4;
}

inline void IntCode::sum(){
  list->at(position) = paramA + paramB;
  incremental = 4;
}

inline void IntCode::store(){
  list->at(position) = input;
  incremental = 2;
}

inline void IntCode::store(bool store){
  if(store) list->at(position) = 1;
  else list->at(position) = 0;
  incremental = 4;
}

inline void IntCode::output(){
  std::cout<<list->at(position)<<'\n';
  incremental = 2;
}

inline void IntCode::jump(bool jump, int64_t& position){
  if(jump) index = paramB;
  else index += 3;
  jumped = true;
}

inline void IntCode::setBase(){
  base += paramA;
  incremental = 2;
}

inline void IntCode::parse(){
  instruction[0] = raw % 100;
  raw = static_cast<int64_t>(raw/100);

  instruction[1] = raw % 10;
  raw = static_cast<int64_t>(raw/10);

  instruction[2] = raw % 10;
  raw = static_cast<int64_t>(raw/10);

  instruction[3] = raw % 10;
  raw = static_cast<int64_t>(raw/10);
}

inline void IntCode::readParam(int num){
  std::array<int64_t*, 2> parameter{&paramA, &paramB};

  for(int i{1}; i<=num; i++){
    option = instruction[i];
    switch(option){
      case 0:
        position = list->at(index + i);
        break;
      case 1:
        position = index + i;
        break;
      case 2:
        position = base + list->at(index + i);
        break;
    }
    checkCapacity(position);
    *parameter[i-1] = list->at(position);
  }
}


inline void IntCode::readPosition(int i, int offset){
  option = instruction[i];
  switch(option){
    case 0:
      position = list->at(index + offset);
      break;
    case 1:
      position = index + offset;
      break;
    case 2:
      position = std::abs(base + list->at(index + offset));
      break;
  }

  checkCapacity(position);
}

inline void IntCode::checkCapacity(int64_t offset){
  if(position >= list->size()) list->resize(position + 10);
}


inline void IntCode::compute(){
  int64_t opcode{0};

  do{
    raw = list->at(index);
    parse();
    opcode = instruction[0];

    switch(opcode){
      case 3:
      case 9:
        readParam(1);
        readPosition(1, 1);
        break;
      case 4:
        readPosition(1,1);
        break;
      default:
        readParam(2);
        readPosition(3, 3);
        break;
    }

    switch(opcode){
      case 1:
        sum();
        break;
      case 2:
        mult();
        break;
      case 3:
        store();
        break;
      case 4:
        output();
        break;
      case 5:
        jump(paramA != 0, index);
        break;
      case 6:
        jump(paramA == 0, index);
        break;
      case 7:
        store(paramA < paramB);
        break;
      case 8:
        store(paramA == paramB);
        break;
      case 9:
        setBase();
        break;
      case 99:
        std::cout<<"End"<<'\n';
        break;
      default:
        std::cout<<"Error"<<'\n';
    }

    if(!jumped) index += incremental;
    else jumped = false;

  } while(opcode != 99);

}


inline IntCode::IntCode(vi* l, int64_t i):list(l), input(i){
  compute();
};



int main(){
  std::string line;
  std::ifstream file;

  vi nums;

  file.open("input.txt");

  while(std::getline(file, line, ',')){
    int64_t number{0};
    std::istringstream(line) >> number;
    nums.emplace_back(number);
  }

  file.close();

  IntCode ic{&nums, 2};
}
