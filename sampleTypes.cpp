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

//512 bytes for buffer length?
#define BUFLEN 512

enum type_enum      //enumeration of data 
{ 
    //
    DGR_VOID,           //0 void
    DGR_INT,            //1 int
    DGR_DOUBLE,         //2 double
    DGR_FLOAT,          //3 float
    DGR_CHAR,           //4 char
    DGR_BOOL,           //5 bool
    //6->14 future use

    //C arrays
    DGR_PTR_VOID=16,    //16 void *
    DGR_PTR_INT,        //17 int *
    DGR_PTR_DOUBLE,     //18 double *
    DGR_PTR_FLOAT,      //19 float *
    DGR_PTR_CHAR,       //20 char *
    DGR_PTR_BOOL,       //21 bool *
    //16->30 future use ---------------------------
    
    //STL Containers
    DGR_STD_STRING=32,      //32 std::string

    //sequence containers
    DGR_STD_ARRAY,          //33 std::array
    DGR_STD_VECTOR,         //34 std::vector
    DGR_STD_DEQUE,          //35 std::deque
    DGR_STD_FORWARD_LIST,   //36 std::forward_list
    DGR_STD_LIST,           //37 std::list

    //container adaptors
    DGR_STD_STACK,          //38 std::stack
    DGR_STD_QUEUE,          //39 std::queue
    DGR_STD_PRIORITY_QUEUE, //40 std::priority_queue

    //associative containers
    DGR_STD_SET,            //41 std::set
    DGR_STD_MULTISET,       //42 std::multiset
    DGR_STD_MAP,            //43 std::map
    DGR_STD_MULTIMAP,       //44 std::multimap

    //Unordered associative containers
    DGR_STD_UNORDERED_SET,      //45 std::unordered_set
    DGR_STD_UNORDERED_MULTISET, //46 std::unordered_multiset
    DGR_STD_UNORDERED_MAP,      //47 std::unordered_map
    DGR_STD_UNORDERED_MULTIMAP, //48 std::unordered_multimap

    //STL Container pointers
    DGR_PTR_STD_STRING,         //50 std::string *

    //sequence container pointers
    DGR_PTR_STD_ARRAY,          //51 std::array *
    DGR_PTR_STD_VECTOR,         //52 std::vector *
    DGR_PTR_STD_DEQUE,          //53 std::deque *
    DGR_PTR_STD_FORWARD_LIST,   //54 std::forward_list *
    DGR_PTR_STD_LIST,           //55 std::list *

    //container adaptor pointers
    DGR_PTR_STD_STACK,          //56 std::stack *
    DGR_PTR_STD_QUEUE,          //57 std::queue *
    DGR_PTR_STD_PRIORITY_QUEUE, //58 std::priority_queue *

    //associative container pointers
    DGR_PTR_STD_SET,            //59 std::set *
    DGR_PTR_STD_MULTISET,       //60 std::multiset *
    DGR_PTR_STD_MAP,            //61 std::map *
    DGR_PTR_STD_MULTIMAP,       //62 std::multimap *
 
    //Unordered associative container pointers
    DGR_PTR_STD_UNORDERED_SET,      //63 std::unordered_set *
    DGR_PTR_STD_UNORDERED_MULTISET, //64 std::unordered_multiset *
    DGR_PTR_STD_UNORDERED_MAP,      //65 std::unordered_map *
    DGR_PTR_STD_UNORDERED_MULTIMAP, //66 std::unordered_multimap *
    //67->127 future use ------------------------------------------------

    //command section 128->256
    DGR_PTR_FUNCTION = 128,          //128 function pointer for callbacks   
    CMD_NEW_OBJECT
                
};
template<typename T>
type_enum typeid_int(T data_typeid)
{
    if(typeid(void)     == typeid(data_typeid)) return DGR_VOID;
    else if(typeid(int)      == typeid(data_typeid)) return DGR_INT;
    else if(typeid(bool)     == typeid(data_typeid)) return DGR_DOUBLE;
    else if(typeid(float)    == typeid(data_typeid)) return DGR_FLOAT;
    else if(typeid(char)     == typeid(data_typeid)) return DGR_CHAR;
    else if(typeid(bool)     == typeid(data_typeid)) return DGR_BOOL;

    else if(typeid(void *)   == typeid(data_typeid)) return DGR_PTR_VOID;
    else if(typeid(int *)    == typeid(data_typeid)) return DGR_PTR_INT;
    else if(typeid(bool *)   == typeid(data_typeid)) return DGR_PTR_DOUBLE;
    else if(typeid(float *)  == typeid(data_typeid)) return DGR_PTR_FLOAT;
    else if(typeid(char *)   == typeid(data_typeid)) return DGR_PTR_CHAR;
    else if(typeid(bool *)   == typeid(data_typeid)) return DGR_PTR_BOOL;

    else if(typeid(std::string)             == typeid(data_typeid)) return DGR_STD_STRING;
    // if(typeid(std::array<U>)               == typeid(data_typeid)) return DGR_STD_ARRAY;
    // if(typeid(std::vector)              == typeid(data_typeid)) return DGR_STD_VECTOR;
    // if(typeid(std::deque))              == typeid(data_typeid)) return DGR_STD_DEQUE;
    // if(typeid(std::forward_list)        == typeid(data_typeid)) return DGR_STD_FORWARD_LIST;
    // if(typeid(std::list)                == typeid(data_typeid)) return DGR_STD_LIST;
    // if(typeid(std::stack)               == typeid(data_typeid)) return DGR_STD_STACK;
    // if(typeid(std::queue)               == typeid(data_typeid)) return DGR_STD_QUEUE;
    // if(typeid(std::priority_queue)      == typeid(data_typeid)) return DGR_STD_PRIORITY_QUEUE;
    // if(typeid(std::set)                 == typeid(data_typeid)) return DGR_STD_SET;
    // if(typeid(set::multiset)            == typeid(data_typeid)) return DGR_STD_MULTISET;
    // if(typeid(set::map)                 == typeid(data_typeid)) return DGR_STD_MAP;
    // if(typeid(set::multimap)            == typeid(data_typeid)) return DGR_STD_MULTIMAP;
    // if(typeid(set::unordered_set)       == typeid(data_typeid)) return DGR_STD_UNORDERED_SET;
    // if(typeid(set::unordered_multiset)  == typeid(data_typeid)) return DGR_STD_UNORDERED_MULTISET;
    // if(typeid(set::unordered_map)       == typeid(data_typeid)) return DGR_STD_UNORDERED_MAP;
    // if(typeid(set::unordered_multimap)  == typeid(data_typeid)) return DGR_STD_UNORDERED_MULTIMAP;

    // if(typeid(std::string *)            == typeid(data_typeid)) return DGR_PTR_STD_STRING;
    // if(typeid(std::array *)             == typeid(data_typeid)) return DGR_PTR_STD_ARRAY;
    // if(typeid(std::vector *)            == typeid(data_typeid)) return DGR_PTR_STD_VECTOR;
    // if(typeid(std::deque *)             == typeid(data_typeid)) return DGR_PTR_STD_DEQUE;
    // if(typeid(std::forward_list *)      == typeid(data_typeid)) return DGR_PTR_STD_FORWARD_LIST;
    // if(typeid(std::list *)              == typeid(data_typeid)) return DGR_PTR_STD_LIST;
    // if(typeid(std::stack *)             == typeid(data_typeid)) return DGR_PTR_STD_STACK;
    // if(typeid(std::queue *)             == typeid(data_typeid)) return DGR_PTR_STD_QUEUE;
    // if(typeid(std::priority_queue *)    == typeid(data_typeid)) return DGR_PTR_STD_PRIORITY_QUEUE;
    // if(typeid(std::set *)               == typeid(data_typeid)) return DGR_PTR_STD_SET;
    // if(typeid(set::multiset *)          == typeid(data_typeid)) return DGR_PTR_STD_MULTISET;
    // if(typeid(set::map *)               == typeid(data_typeid)) return DGR_PTR_STD_MAP;
    // if(typeid(set::multimap *)          == typeid(data_typeid)) return DGR_PTR_STD_MULTIMAP;
    // if(typeid(set::unordered_set *)     == typeid(data_typeid)) return DGR_PTR_STD_UNORDERED_SET;
    // if(typeid(set::unordered_multiset *)== typeid(data_typeid)) return DGR_PTR_STD_UNORDERED_MULTISET;
    // if(typeid(set::unordered_map *)     == typeid(data_typeid)) return DGR_PTR_STD_UNORDERED_MAP;
    // if(typeid(set::unordered_multimap *)== typeid(data_typeid)) return DGR_PTR_STD_UNORDERED_MULTIMAP;
     else{
        return DGR_VOID;
     }

}

/**
 * Name: MapNode
 * Description: A template class for a universal map node
 */
class MapNodePtr{
public:
    std::string name;
    type_enum dataType;
    size_t dataLength;
    virtual char * getDataString(void) =0;
    virtual void setData(char *) =0;
    //setData callback function?
    MapNodePtr(std::string n) : name(n){}
};

//only works with POD types
template<typename T>
class MapNode : public MapNodePtr
{
protected:
    T data;

public:
    T * getData(){ return &data;}

    char * getDataString(void)
    {
        char * data_array = new char[dataLength];
        memcpy(data_array, &data, dataLength);        
        return data_array;
    }
    void setData(char * data_array)
    {
        memcpy(&data, data_array, dataLength);
        //std::cout<< data << std::endl;
    }

    MapNode(std::string n, T d) : MapNodePtr(n), data(d){
        dataType = typeid_int<T>(d);
        dataLength = sizeof(T);
    }

    MapNode(std::string n, T d, size_t s) : MapNodePtr(n), data(d){
        dataType = typeid_int<T>(d);
        dataLength = s;
    }


    MapNode(std::string n, type_enum dT, size_t dL, T d) : MapNodePtr(n, dT, dL), data(d){}
      
};


int serialize(std::map<std::string, MapNodePtr *> InputMap, char * message_buf)
{
    // char * message_buf = new char[BUFLEN];
    char * node_buf;
    unsigned char packet_counter = 0;
    int node_counter = 0;
    int node_length = 0;
    int packet_length = 0;
    bool first_node = true;
    bool last_node = false;


    for(auto it = InputMap.begin();it!= InputMap.end();it++)
    {
       
        node_length = 0;
        if(std::next(it) == InputMap.end()){ last_node = true;
        std::cout << "last node!" << std::endl;}
        if(first_node) std::cout<< "first node!" << std::endl;
        
        MapNodePtr * cur_node = it->second;
        std::string cur_name = cur_node->name;
        char* cur_data = cur_node->getDataString();
        int cur_data_length = cur_node->dataLength;

        unsigned char cur_type = cur_node->dataType;
        
        node_length = cur_name.length() + sizeof(char) + cur_data_length;

        // if(first_node || last_node) node_length += sizeof(packet_counter);
        node_buf = new char[node_length];
        
        //prepare node buffer
        memcpy(node_buf, cur_name.c_str(), cur_name.length());
        node_buf[cur_name.length()] = '\0';
        memcpy(node_buf + cur_name.length() + 1, cur_data, cur_data_length);
        //message buffer full. send and start a new one.
        if( (packet_length + node_length) > BUFLEN)
        {
          for(int i = 0; i < packet_length; i++ ){ std::cout << message_buf[i]; message_buf[i] = '\0';}
          std::cout << std::endl << "split! " << packet_counter << std::endl;
          packet_counter ++;
          packet_length = 0;
          first_node = true;
          it--;

        }else{
        
            

            memcpy(message_buf + packet_length, node_buf, node_length);
            // std::cout << "buffer: "<< std::endl;
            // for(int i = 0; i < BUFLEN; i++){
            //     if(message_buf[i] != '\0'){
            //         if(message_buf[i] > 31) printf("%c",message_buf[i]); 
            //         else printf(":%i",message_buf[i]);
            //     }
            //     else printf("| ");
            // }
            // std::cout << "::" << std::endl;   
            packet_length += node_length;


            
            first_node = false;
            node_counter ++;
        }

    }
    std::cout << " buffer: ";
    for(int i = 0; i < BUFLEN; i++) printf("%c",message_buf[i]);
    puts("\n");
    return node_counter;
}

int parser(char * packet, std::map<std::string, MapNodePtr *> InputMap)
{
    

    char packet_number, node_data_type;

    MapNodePtr * cur_node;
    std::string node_name;
    int node_data_length, packet_cursor_0, packet_cursor_1, packet_counter;


    std::cout << "parsing..." << std::endl; 
    packet_cursor_0 = 0;
    packet_counter = 0;

    //each iteration of the while loop is one packet
    while(packet[packet_cursor_0] > 31 && packet_cursor_0 < BUFLEN){
        
        //extract name
        node_name = "";
        for(int i = packet_cursor_0; i < BUFLEN; i++) 
        {
            if(packet[i])
            {

                node_name.push_back(packet[i]);
                packet_cursor_0++;
            }
            else
            {
                packet_cursor_0++;
                break;
            }
        }

        std::cout <<"name: " << node_name;
        
        //get current node and data length
        cur_node = InputMap.at(node_name);
        node_data_length = cur_node->dataLength;
        
        std::cout << " node_data_length: " << node_data_length << std::endl;
        

        
        //std::cout << "node_data_length: " << node_data_length << std::endl;
        char * node_data = new char[node_data_length];
        node_data = &packet[packet_cursor_0];
        printf("node_data:%i:\n",packet[packet_cursor_0]); 
        packet_cursor_1 = packet_cursor_0 + node_data_length;
        while(packet_cursor_0 < packet_cursor_1)  std::cout << packet[packet_cursor_0++];
        std::cout << std::endl;
        //get pointer to data in string
       
        //set data
        cur_node->setData(node_data);
        //print data        
        
        //prove that write was successful
        std::cout << node_name << " : " << cur_node->getDataString() << " : " << std::endl;

        // delete node_data;
        packet_counter ++;
    }
    return packet_counter;
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

    int * first,first_0,first_1,first_2,first_3;
    double * second,second_0,second_1,second_2,second_3;
    float * third,third_0,third_1,third_2,third_3;
    bool * fifth,fifth_0,fifth_1,fifth_2,fifth_3;

    MapNode<int> * node1 = new MapNode<int>(std::string("first"),5);
    MapNode<double> * node2 = new MapNode<double>(std::string("second"),8.1);
    MapNode<float> * node3 = new MapNode<float>(std::string("third"),3.1);
    //MapNode<std::string> * node4 = new MapNode<std::string>(std::string("fourth"),"foofighters 1");
    MapNode<bool> * node5 = new MapNode<bool>(std::string("fifth"),true);
    MapNode<int> * node11 = new MapNode<int>(std::string("first_0"),5);
    MapNode<double> * node12 = new MapNode<double>(std::string("second_0"),8.1);
    MapNode<float> * node13 = new MapNode<float>(std::string("third_0"),3.1);
    //MapNode<std::string> * node14 = new MapNode<std::string>(std::string("fourth_0"),"foofighters 2");
    MapNode<bool> * node15 = new MapNode<bool>(std::string("fifth_0"),true);
    MapNode<int> * node21 = new MapNode<int>(std::string("first_1"),5);
    MapNode<double> * node22 = new MapNode<double>(std::string("second_1"),8.1);
    MapNode<float> * node23 = new MapNode<float>(std::string("third_1"),3.1);
    //MapNode<std::string> * node24 = new MapNode<std::string>(std::string("fourth_1"),"foofighters 3");
    MapNode<bool> * node25 = new MapNode<bool>(std::string("fifth_1"),true);
    MapNode<int> * node31 = new MapNode<int>(std::string("first_2"),5);
    MapNode<double> * node32 = new MapNode<double>(std::string("second_2"),8.1);
    MapNode<float> * node33 = new MapNode<float>(std::string("third_2"),3.1);
    //MapNode<std::string> * node34 = new MapNode<std::string>(std::string("fourth_2"),"foofighters 4");
    MapNode<bool> * node35 = new MapNode<bool>(std::string("fifth_2"),true);
    MapNode<int> * node41 = new MapNode<int>(std::string("first_3"),5);
    MapNode<double> * node42 = new MapNode<double>(std::string("second_3"),8.1);
    MapNode<float> * node43 = new MapNode<float>(std::string("third_3"),3.1);
    //MapNode<std::string> * node44 = new MapNode<std::string>(std::string("fourth_3"),"foofighters 5");
    MapNode<bool> * node45 = new MapNode<bool>(std::string("fifth_3"),true);
    
    first = node1->getData();
    second = node2->getData();
    third = node3->getData();
    fifth = node5->getData();
    
    std::map<std::string, MapNodePtr *> InputMap_0 = 
    {
        {node1->name,(MapNodePtr *)node1},
        {node2->name,(MapNodePtr *)node2},
        {node3->name,(MapNodePtr *)node3},
        // {node4->name,(MapNodePtr *)node4},
        {node5->name,(MapNodePtr *)node5},
        {node11->name,(MapNodePtr *)node11},
        {node12->name,(MapNodePtr *)node12},
        {node13->name,(MapNodePtr *)node13},
        // {node14->name,(MapNodePtr *)node14},
        {node15->name,(MapNodePtr *)node15},
        {node21->name,(MapNodePtr *)node21},
        {node22->name,(MapNodePtr *)node22},
        {node23->name,(MapNodePtr *)node23},
        // {node24->name,(MapNodePtr *)node24},
        {node25->name,(MapNodePtr *)node25},
        {node31->name,(MapNodePtr *)node31},
        {node32->name,(MapNodePtr *)node32},
        {node33->name,(MapNodePtr *)node33},
        // {node34->name,(MapNodePtr *)node34},
        {node35->name,(MapNodePtr *)node35},
        {node41->name,(MapNodePtr *)node41},
        {node42->name,(MapNodePtr *)node42},
        {node43->name,(MapNodePtr *)node43},
        // {node44->name,(MapNodePtr *)node44},
        {node45->name,(MapNodePtr *)node45},

    };

    std::map<std::string, MapNodePtr *> InputMap_1;
    InputMap_1 = InputMap_0;

    *first = *first * 2;
    *second = *second * 3;
    *third = *third * 4;
    *fifth = false;
    std::cout << std::endl <<std::endl << InputMap_1.at("first")->name << " " << *first << std::endl;
    std::cout << std::endl <<std::endl << InputMap_1.at("second")->name << " " << *second << std::endl;
    std::cout << std::endl <<std::endl << InputMap_1.at("third")->name << " " << *third<< std::endl;
    std::cout << std::endl <<std::endl << InputMap_1.at("fifth")->name << " " << *fifth << std::endl;
    
    char * packet_buffer = new char[BUFLEN]; 
    std::cout<< "number of packets sent: " << serialize(InputMap_0,packet_buffer) << std::endl;
    std::cout<< "number of packets received: " << parser(packet_buffer,InputMap_1) << std::endl;

    std::cout << std::endl <<std::endl << InputMap_1.at("first")->name << " " << *first << std::endl;
    std::cout << std::endl <<std::endl << InputMap_1.at("second")->name << " " << *second << std::endl;
    std::cout << std::endl <<std::endl << InputMap_1.at("third")->name << " " << *third<< std::endl;
    std::cout << std::endl <<std::endl << InputMap_1.at("fifth")->name << " " << *fifth << std::endl;

return 0;
}
