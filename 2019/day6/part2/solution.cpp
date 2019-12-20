#include<iostream>
#include<iostream>
#include<string>
#include<tuple>
#include<fstream>
#include<sstream>
#include<vector>
#include<unordered_map>


typedef std::string st;
typedef std::vector<st> vst;
typedef std::pair<st,st> pst;
typedef std::tuple<int, st, vst> tistvst;
typedef std::pair<st, tistvst> pstistvst;
typedef std::vector<pst> vpst;
typedef std::unordered_map<st, tistvst> stistvst;


class Universe{
  public:
    Universe(vpst&);
    ~Universe() = default;

    int getTotalOrbit();
    int getTravelDistance();

  private:
    int orbitNum{0}, totalOrbit{0};
    int travelDistance{0};
    stistvst nodes;

    void build(vpst&);
    void propagateOrbit(st&, int);
    void calcTotalOrbit();
    void travelToSanta();
};



int Universe::getTravelDistance(){
  return travelDistance == -1 ? 0 : travelDistance;
}

// TODO IMPROVEMENTS:
// Travel to santa can be calculated using intersection point
// without the need of O(n2) time
void Universe::travelToSanta(){
  st parent{"SAN"}, child{"YOU"};
  int tParent{-1}; int tChild{-1};
  bool foundRoute{false};

  while(!foundRoute && parent != ""){
    parent = std::get<1>(nodes.at(parent));
    ++tParent;

    while(!foundRoute && child != ""){
      child = std::get<1>(nodes.at(child));
      ++tChild;
      if(parent == child) foundRoute = true;
    }
    if(!foundRoute){
      tChild = -1;
      child = "YOU";
    }
  }

  travelDistance = tParent + tChild;
}

void Universe::calcTotalOrbit(){
  for(const pstistvst& node : nodes){
    totalOrbit += std::get<0>(node.second);
  }
}

void Universe::propagateOrbit(st& parent, int orbit){
  vst planets{parent};

  for(int i{0}; i<planets.size(); i++){
    vst& childs{std::get<2>(nodes.at(planets[i]))};

    for(const st& child : childs)
      planets.emplace_back(child);
    
  }
  for(const st& planet : planets)
    std::get<0>(nodes.at(planet)) += orbit;
}


void Universe::build(vpst& list){
  bool parentExist{false}, childExist{false};
  st parent{""}, child{""};

  for(const pst val : list){
    parent = val.first;
    child = val.second;

    parentExist = nodes.find(parent) != nodes.end();
    childExist = nodes.find(child) != nodes.end();

    if(!parentExist && !childExist){
      nodes.insert({parent, {0, "", {child}}});
      nodes.insert({child, {1, parent, {}}});
    }
    else if(!parentExist && childExist){
      nodes.insert({parent, {0, "", {child}}});
      std::get<1>(nodes.at(child)) = parent;
      propagateOrbit(child, 1);
    }
    else if(parentExist && !childExist){
      std::get<2>(nodes.at(parent)).emplace_back(child);
      orbitNum = std::get<0>(nodes.at(parent)) + 1;
      nodes.insert({child, {orbitNum, parent, {}}});
    
    }
    else if(parentExist && childExist){
      std::get<2>(nodes.at(parent)).emplace_back(child);
      std::get<1>(nodes.at(child)) = parent;
      orbitNum = std::get<0>(nodes.at(parent)) + 1;
      propagateOrbit(child, orbitNum);
    }
  }
}

int Universe::getTotalOrbit(){
  return totalOrbit;
}

Universe::Universe(vpst& list){
  build(list);
  calcTotalOrbit();
  travelToSanta();
}


int main(){
  st line;
  st token;
  pst val{"",""};
  vpst list;

  std::ifstream file;
  file.open("input.txt");

  while(getline(file, line)){
    std::istringstream iss(line);

    while(getline(iss, token, ')')){
      if(val.first == "") val.first = token;
      else val.second = token;
    }
    list.push_back(val);
    val.first = "";
    val.second = "";
  }

  file.close();

  Universe un{list};
  std::cout<<un.getTotalOrbit()<<std::endl;
  std::cout<<un.getTravelDistance()<<std::endl;

}

