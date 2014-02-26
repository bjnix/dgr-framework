#include <cstdlib>
#include <unistd.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <typeinfo>
#include <iostream>
#include <functional>

#define BUFLEN 512


/**
 * Name: MapNode
 * Description: A template class for a map node
 */
class MapNodePtr{
public:
  std::string dataType;
  std::string name;
};

template<typename T>
class MapNode : public MapNodePtr
{
public:
  //MapNode(std::string n, T* d );
  MapNode(std::string n, T* d );
  MapNode(std::string n, T d);
  
  typedef struct{
    const char * name;
    T data;
  }packet;

  packet packetData;
  
};

template<typename T>
MapNode<T>::MapNode(std::string n, T* d) {
  name = n;
  packetData.name = &n[1];
  packetData.data = *d;
  dataType = typeid(*d).name();
};
template<typename T>
MapNode<T>::MapNode(std::string n, T d) {
  name = n;
  packetData.name = &n[1];
  packetData.data = d;
  dataType = typeid(d).name();
};
int serialize(std::map<std::string, MapNodePtr *> InputMap)
{
  char buf [BUFLEN];
  int length = 0;
  
  for(auto it = InputMap.begin();it!= InputMap.end();it++)
  {

    if(length >= (BUFLEN - 15))
    {
      return 0;
    }
    if(it->second->dataType == typeid(float).name()){ 
      MapNode<float> * cur_node = (MapNode<float> *) it->second; 
      std::cout << cur_node->name <<" "<< cur_node->packetData.data << std::endl;
    }
    else if(it->second->dataType == typeid(double).name()){ 
      MapNode<double> * cur_node = (MapNode<double> *) it->second; 
      std::cout << cur_node->name <<" "<< cur_node->packetData.data << std::endl;
    }
    else if(it->second->dataType == typeid(int).name()){ 
      MapNode<int> * cur_node = (MapNode<int> *) it->second; 
      std::cout << cur_node->name <<" "<< cur_node->packetData.data << std::endl;
    }
    else if(it->second->dataType == typeid(bool).name()){ 
      MapNode<bool> * cur_node = (MapNode<bool> *) it->second; 
      std::cout << cur_node->name <<" "<< cur_node->packetData.data << std::endl;
    }
    else if(it->second->dataType == typeid(std::string).name()){ 
      MapNode<std::string> * cur_node = (MapNode<std::string> *) it->second; 
      std::cout << cur_node->name <<" "<< cur_node->packetData.data << std::endl;
    }
    else{ return 1;}
    
    
  }
  return 0;
}
int main(){

  MapNode<int> * node1 = new MapNode<int>(std::string("first"),5);
  MapNode<double> * node2 = new MapNode<double>("second",8.1);
  MapNode<float> * node3 = new MapNode<float>("third",3.1);
  MapNode<std::string> * node4 = new MapNode<std::string>("fourth","foofighters");
  MapNode<bool> * node5 = new MapNode<bool>("fifth",true);

  std::map<std::string, MapNodePtr *> InputMap = {
    {node1->name,(MapNodePtr *)node1},
    {node2->name,(MapNodePtr *)node2},
    {node3->name,(MapNodePtr *)node3},
    {node4->name,(MapNodePtr *)node4},
    {node5->name,(MapNodePtr *)node5}
  };

  //std::cout << InputMap.at("first")->name <<" "<< ((MapNode<int>*) (InputMap.at("first")))->packetData.data << std::endl;
  serialize(InputMap);

  return 0;
}
