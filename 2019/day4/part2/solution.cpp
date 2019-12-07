#include<iostream>
#include<string>
#include<stack>

typedef std::string st;

class DigitCombinations{
  public:
    DigitCombinations(const st&);
    ~DigitCombinations() = default;

    int const getNumCombinations() const;

  private:
    st const input;
    int rangeA{0}, rangeB{0};
    int number{0}, localNum{0}, nextNum{0};
    unsigned int numCombo{0};

    void cleanInput();
    void generateNumCombos();
    bool isValid();
};


bool DigitCombinations::isValid(){
  bool doubles{false};
  int invalid{0};

  std::stack<int> tracker;

  for(; number != 0; number /= 10){
    localNum = number%10;
    nextNum = (number%100)/10;

    if(number == localNum) break;
    if(localNum < nextNum) return false;
    else if(doubles && invalid == nextNum){
      invalid = nextNum;
      doubles = false;
      tracker.pop();
    }
    else if(invalid != nextNum &&
            localNum == nextNum){
      invalid = localNum;
      doubles = true;
      tracker.push(invalid);
    }
  }
  return tracker.size() != 0;
}

void DigitCombinations::generateNumCombos(){
  for(; rangeA<=rangeB; rangeA++){
    number = rangeA;
    if(isValid()) ++numCombo;
  }
}

void DigitCombinations::cleanInput(){
  for(int front{1}, back{0}; back<input.size(); ++front)
    if(input[front] == '-' || front == input.size()){
      if(rangeA == 0) rangeA = std::stoi(input.substr(back, front));
      else if(rangeB == 0) rangeB = std::stoi(input.substr(back, front-1));
      back = front + 1;
    }
}

int const DigitCombinations::getNumCombinations() const{
  return numCombo;
}

DigitCombinations::DigitCombinations(const st& i)
  :input(i){
  cleanInput();
  generateNumCombos();
}

int main(){
  st input{"357253-892942"};
  DigitCombinations dc{input};
  std::cout<<dc.getNumCombinations()<<std::endl;
}

