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
typedef struct{
  const char * name;
}packetPtr;

template<typename T>
class MapNode : public MapNodePtr
{
public:
  //MapNode(std::string n, T* d );
  MapNode(std::string n, T* d );
  MapNode(std::string n, T d);
  
  typedef struct packet: public packetPtr{
    T data;
  }packet;

  packet packetData;
  int packetLength;
};

template<typename T>
MapNode<T>::MapNode(std::string n, T* d) {
  name = n;
  packetData.name = n.c_str();
  packetData.data = *d;
  dataType = typeid(*d).name();
  packetLength = sizeof(packetData.name) + sizeof(packetData.data);
};
template<typename T>
MapNode<T>::MapNode(std::string n, T d) {
  name = n;
  packetData.name = n.c_str();
  packetData.data = d;
  dataType = typeid(d).name();
  std::cout<<sizeof(packetData)<<std::endl;
  packetLength = sizeof(packetData);
};
std::vector<std::string> * serialize(std::map<std::string, MapNodePtr *> InputMap)
{
  char buf [BUFLEN];
  int length = 0;
  packetPtr * sendStruct;
  std::vector<std::string> * packets = new std::vector<std::string>;
  for(auto it = InputMap.begin();it!= InputMap.end();it++)
  {
    std::string message_buf;

    if(length >= (BUFLEN - 15))
    {
      return packets;
    }
    if(it->second->dataType == typeid(float).name()){ 
      MapNode<float> * cur_node = (MapNode<float> *) it->second; 
      sendStruct = &(cur_node->packetData);
      length = cur_node->packetLength;
      
      message_buf.reserve( sizeof(cur_node->packetData) );
      message_buf.append( (const char*) &(cur_node->packetData.name), sizeof(cur_node->packetData.name));
      message_buf.append( (const char*) &(cur_node->packetData.data) , sizeof(cur_node->packetData.data));
      std::cout << sendStruct->name <<" "<< cur_node->packetData.data << " " << message_buf << " " << message_buf.size() << " " << length << std::endl;
    }
    else if(it->second->dataType == typeid(double).name()){ 
      MapNode<double> * cur_node = (MapNode<double> *) it->second; 
      sendStruct = &(cur_node->packetData);
      length = cur_node->packetLength;
      std::string message_buf;
      message_buf.reserve( sizeof(cur_node->packetData) );
      message_buf.append( (const char*) &(cur_node->packetData.name), sizeof(cur_node->packetData.name));
      message_buf.append( (const char*) &(cur_node->packetData.data) , sizeof(cur_node->packetData.data));

      std::cout << sendStruct->name <<" "<< cur_node->packetData.data << " " << message_buf << " " << message_buf.size() << " " << length << std::endl;
    }
    else if(it->second->dataType == typeid(int).name()){ 
      MapNode<int> * cur_node = (MapNode<int> *) it->second; 
      sendStruct = &(cur_node->packetData);
      length = cur_node->packetLength;length = cur_node->packetLength;
      std::string message_buf;
      message_buf.reserve( sizeof(cur_node->packetData) );
      message_buf.append( (const char*) &(cur_node->packetData.name), sizeof(cur_node->packetData.name));
      message_buf.append( (const char*) &(cur_node->packetData.data) , sizeof(cur_node->packetData.data));

      std::cout << sendStruct->name <<" "<< cur_node->packetData.data << " " << message_buf << " " << message_buf.size() << " " << length << std::endl;
    }
    else if(it->second->dataType == typeid(bool).name()){ 
      MapNode<bool> * cur_node = (MapNode<bool> *) it->second; 
      sendStruct = &(cur_node->packetData);
      length = cur_node->packetLength;
      std::string message_buf;
      message_buf.reserve( sizeof(cur_node->packetData) );
      message_buf.append( (const char*) &(cur_node->packetData.name), sizeof(cur_node->packetData.name));
      message_buf.append( (const char*) &(cur_node->packetData.data) , sizeof(cur_node->packetData.data));

      std::cout << sendStruct->name <<" "<< cur_node->packetData.data << " " << message_buf << " " << message_buf.size() << " " << length << std::endl;
    }
    else if(it->second->dataType == typeid(std::string).name()){ 
      MapNode<std::string> * cur_node = (MapNode<std::string> *) it->second; 
      sendStruct = &(cur_node->packetData);
      length = cur_node->packetLength;
      std::string message_buf;
      message_buf.reserve( sizeof(cur_node->packetData) );
      message_buf.append( (const char*) &(cur_node->packetData.name), sizeof(cur_node->packetData.name));
      message_buf.append( (const char*) &(cur_node->packetData.data) , sizeof(cur_node->packetData.data));

      std::cout << sendStruct->name <<" "<< cur_node->packetData.data << " " << message_buf << " " << message_buf.size() << " " << length << std::endl;
    }
    else{ std::cout<<"error"<<std::endl; return packets;}
    
    packets->push_back(message_buf);
    
  }
  return packets;
}
void parser(std::vector<std::string>> * packets){
  for(auto it = packets->front(); it != packets->back();it++)
  {
    it.find()
  }
}
// void receiver() {
//   char buf[BUFLEN];
//   vector<string> splits;
//   vector<string> packet;
//   //printf("in receiver");
//   while (true) {
//     if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr*)&si_other,
//       &slen) == -1) error("ERROR recvfrom()");
//     receivedPacket = true;
//     framesPassed = 0;
//     string itrmdt(buf);
//     //printf("recieved: %s\n", buf);
//     splits = split(itrmdt, '~');
//     // NOTE: This simple example only sends/receives a single value (rotation),
//     // but it sends rotation twice, separated by a ~ in order to demonstrate the
//     // technique of how you can send multiple values separated by ~ and then
//     // get the values back out, as shown here.
//     for(auto splitsIter = splits.begin(); splitsIter != splits.end(); splitsIter++)
//     {
//       packet = split(*splitsIter, '`');
//       InputMap.at(packet[0]) = (float)atof(packet[1].c_str());
      
//     }
//   }
// }
int main(){

  MapNode<int> * node1 = new MapNode<int>(std::string("first"),5);
  MapNode<double> * node2 = new MapNode<double>("second",8.1);
  MapNode<float> * node3 = new MapNode<float>("third",3.1);
  MapNode<std::string> * node4 = new MapNode<std::string>("fourth","foofighters");
  MapNode<bool> * node5 = new MapNode<bool>("fifth",true);
  std::cout<<node1->packetData.name<<" "<< sizeof(node1->packetData)<<std::endl;
  std::cout<<node2->packetData.name<<" "<< sizeof(node2->packetData)<<std::endl;
  std::cout<<node3->packetData.name<<" "<< sizeof(node3->packetData)<<std::endl;
  std::cout<<node4->packetData.name<<" "<< sizeof(node4->packetData)<<std::endl;
  std::cout<<node5->packetData.name<<" "<< sizeof(node5->packetData)<<std::endl<<std::endl;

  std::map<std::string, MapNodePtr *> InputMap = {
    {node1->name,(MapNodePtr *)node1},
    {node2->name,(MapNodePtr *)node2},
    {node3->name,(MapNodePtr *)node3},
    {node4->name,(MapNodePtr *)node4},
    {node5->name,(MapNodePtr *)node5}
  };

  //std::cout << InputMap.at("first")->name <<" "<< ((MapNode<int>*) (InputMap.at("first")))->packetData.data << std::endl;
  parse(serialize(InputMap));

  return 0;
}
