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
#include <sstream>
#include <functional>

#define BUFLEN 512

enum type_enum      //enumeration of data 
{ 
    //
    DGR_VOID,           //0 void
    DGR_INT,            //1 int
    DGR_DOUBLE,         //2 double
    DGR_FLOAT,          //3 float
    DGR_CHAR,           //4 char
    DGR_BOOLEAN,        //5 boolean
    //6->14 future use

    //C arrays
    DGR_PTR_VOID=16,    //16 void
    DGR_PTR_INT,        //17 int
    DGR_PTR_DOUBLE,     //18 double
    DGR_PTR_FLOAT,      //19 float
    DGR_PTR_CHAR,       //20 char
    DGR_PTR_BOOLEAN,    //21 boolean
    //16->30 future use
    
    //STL Containers
    DGR_STD_STRING=32,  //32 std::string
    //sequence containers
    DGR_STD_ARRAY,      //33 std::array
    DGR_STD_VECTOR,     //34 std::vector
    DGR_STD_DEQUE,      //35 std::deque
    DGR_STD_FORWARD_LIST,    //36 std::forward_list
    DGR_STD_LIST,       //37 std::list

    //container adaptors
    DGR_STD_STACK,      //38 std::stack
    DGR_STD_QUEUE,      //39 std::queue
    DGR_STD_PRIORITY_QUEUE,   //40 std::priority_queue

    //associative containers
    DGR_STD_SET,        //41 std::set
    DGR_STD_MULTISET,   //42 std::multiset
    DGR_STD_MAP,        //43 std::map
    DGR_STD_MULTIMAP,   //44 std::multimap

    //Unordered associative containers
    DGR_STD_UNORDERED_SET,        //45 std::unordered_set
    DGR_STD_UNORDERED_MULTISET,   //46 std::unordered_multiset
    DGR_STD_UNORDERED_MAP,        //47 std::unordered_map
    DGR_STD_UNORDERED_MULTIMAP,   //48 std::unordered_multimap
    //48->127 future use

    //command section 128->256
    CMD_NEW_OBJECT = 128
                
};

/**
 * Name: MapNode
 * Description: A template class for a universal map node
 */
class MapNodePtr{
public:
    std::string name;
    type_enum dataType;
    size_t dataLength;
    virtual std::string getData(void) =0;
    //setData callback function?
    MapNodePtr(std::string n, type_enum dT, size_t dL) : name(n.c_str()), dataType(dT), dataLength(dL){}
};

//only works with types that work with stringstream (native types)
template<typename T>
class MapNode : public MapNodePtr
{
protected:
    T data;

public:
    std::string getData(void){
        std::stringstream ss;
        ss << MapNode<T>::data; 
        return ss.str;
    }
    int setData(T d){ MapNode<T>::data = d; }
    MapNode(std::string n, type_enum dT, size_t dL, T d) : MapNodePtr(n, dT, dL), data(d){}
      
};

std::string * serialize(std::map<std::string, MapNodePtr *> InputMap)
{
    std::string * packets = new std::string;
    std::stringbuf message_buf;
    int packet_counter = 0;
    int node_length = 0;
    int packet_length = 0;
    bool first = true;
    bool last = false;


    for(auto it = InputMap.begin();it!= InputMap.end();it++)
    {
       
        if(std::next(it) == InputMap.end()) last = true;
        

        

        MapNodePtr * cur_node = it->second;
        std::string cur_name = cur_node->name;
        char cur_packet_num; 
        char cur_type = cur_node->dataType;
        std::string cur_data = cur_node->getData();
        
        if(first || last) cur_packet_num = (char)packet_counter;

        node_length = cur_name.length() + sizeof(cur_type) + cur_data.length();

        if(first || last) node_length += sizeof(cur_packet_num);

        if(packet_length + node_length > BUFLEN)
        {
          std::cout << message_buf.str();
          packet_length = 0;
          first = false;

        }else{
        
        if(first || last) { message_buf.sputc(cur_packet_num); node_length -= sizeof(cur_packet_num); }

        message_buf.sputn(cur_node->name, cur_name.length()); 
        message_buf.sputc(cur_node->dataType);
        message_buf.sputn(cur_node->getData(), cur_data.length());

        packet_length += node_length;

        
        first = false;
        node_length = 0;
        }
    }
    
    std::cout << message_buf.str();
    std::cout << std::endl;

    return message_buf.str();
    }
std::vector<std::string> * parser(std::string * packets){
  std::string * pack;
  for(int i = 0; i < packets->size(); i++)
  {
    // pack = &packets[i];
    // pack->replace(sizeof(char),pack->find('\0'),'#');
    // std::cout<<packets[i]<<std::endl;
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
parser(serialize(InputMap));

return 0;
}
