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
    virtual char * getData(void) =0;
    virtual void setData(char *) =0;
    //setData callback function?
    MapNodePtr(std::string n) : name(n){}
    MapNodePtr(std::string n, type_enum dT, size_t dL) : name(n), dataType(dT), dataLength(dL){}
};

//only works with types that work with stringstream (native types)
template<typename T>
class MapNode : public MapNodePtr
{
protected:
    T data;

public:
    char * getData(void){
        
        char * data_array = new char[dataLength];
        memcpy(data_array, &data, dataLength);
        // memcpy(&data, data_array, dataLength);
        // std::cout<< data << std::endl;

        
        return data_array;
    }
    void setData(char * data_array){

        memcpy(&data, data_array, dataLength);
        //std::cout<< data << std::endl;
    }

    MapNode(std::string n, T d) : MapNodePtr(n), data(d){
        std::cout<< data << ": "<< std::endl;
        dataType = typeid_int<T>(d);
        dataLength = sizeof(T);
        //setData(getData());
        std::cout << name << " : " << data << " : " << getData() << " : " <<  typeid(T).name() << " : " << dataLength << std::endl;
        
    }
    MapNode(std::string n, type_enum dT, size_t dL, T d) : MapNodePtr(n, dT, dL), data(d){}
      
};


std::string * serialize(std::map<std::string, MapNodePtr *> InputMap)
{
    std::stringstream message_buf;
    unsigned char packet_counter = 0;
    int node_length = 0;
    int packet_length = 0;
    bool first_node = true;
    bool last_node = false;
    message_buf << std::flush;

    for(auto it = InputMap.begin();it!= InputMap.end();it++)
    {
       
        node_length = 0;
        if(std::next(it) == InputMap.end()){ last_node = true;
        std::cout << "last node!" << std::endl;}
        if(first_node) std::cout<< "first node!" << std::endl;
        
        MapNodePtr * cur_node = it->second;
        std::string cur_name = cur_node->name;
        char* cur_data = cur_node->getData();
        int cur_data_length = cur_node->dataLength;

        unsigned char cur_type = cur_node->dataType;
        

        //node_length = cur_name.length() + sizeof(char) + sizeof(cur_type) + cur_data_length;
        node_length = cur_name.length() + sizeof(char) + cur_data_length;

        if(first_node || last_node) node_length += sizeof(packet_counter);

        if( (packet_length + node_length) > BUFLEN)
        {
          std::cout << message_buf.str() << std::endl << "split!" << std::endl;
          message_buf.str(std::string());
          packet_counter ++;
          packet_length = 0;
          first_node = true;
          it--;

        }else{
        
            if(first_node) { message_buf << packet_counter; }
            //switch comment to include type character 
            //message_buf << cur_node->name << '\0' << cur_type << cur_node->getData();
            message_buf << cur_node->name << '\0' << cur_data;

            packet_length += node_length;
            std::cout << cur_node->name << " : " <<  cur_data << " : " << cur_data_length << std::endl;


            
            first_node = false;
        }

    }
    
    std::string * packet = new std::string(message_buf.str());
    
    //std::cout<< *packet << " packet_length: " << packet_length << std::endl;

    return packet;
}

void parser(std::string * packets, std::map<std::string, MapNodePtr *> InputMap)
{
    std::stringstream packet_sstream;
    packet_sstream.str(*packets);

    char packet_number, node_data_type;
    bool packet_split = false;

    MapNodePtr * cur_node;
    std::string node_name;
    int node_data_length;

    //while(!packet_sstream.good()){}
    packet_sstream.get(packet_number);

    std::cout << "parsing..." << std::endl << static_cast<int>(packet_number) << " : " <<std::endl;

    if(static_cast<int>(packet_number) != 0) { packet_split = true; }

    while(packet_sstream.rdbuf()->in_avail())
    {



        std::getline(packet_sstream, node_name, '\0'); 
        std::cout <<"name: " << node_name << std::endl;

        cur_node = InputMap.at(node_name);

        //packet_sstream.get(node_data_type);
        node_data_length = cur_node->dataLength;

        std::cout << "node_data_length " << node_data_length << std::endl;
        if(node_data_length == 1) node_data_length += 1;
        char * node_data = new char[node_data_length];

        //get data from stream
        packet_sstream.get(node_data, node_data_length );
        std::cout << "node_data " << node_data << std::endl;
        //set data
        cur_node->setData(node_data);

        //std::cout << node_name << " : " << node_data_type << " : " << cur_node->getData() << " : " << std::endl;
        std::cout << node_name << " : " << cur_node->getData() << " : " << std::endl;

        delete node_data;
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
    std::map<std::string, MapNodePtr *> InputMap = 
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

    std::stringstream foo;
    std::stringstream bar;
    std::string food1, food2, food3, food4, food5, food6, food7;
    char fine1,fine2;
    char fine3[18];
    char fine4[12];
    // food4 = std::string("home");
    // food5 = std::string("goon!");    
    // food6 = std::string("qwert asdf");
    // foo << 'f' << 'n' << food4 << '\0' << food5 << '\0' << food6 ;
    // foo.get( fine1 );
    // foo.get( fine2 );

    // std::getline(foo , food1, '\0');
    // std::getline(foo , food2, '\0');
    // foo.get(fine3, food6.length() + 1);
    // std::cout << fine1 << " : " << fine2 << " : " << food1 << " : " << food2 << " : " << fine3 << std::endl;
    char * n = "name";
    char nulll = 'n';
    char * N;
    float D;
    float d = 75.1;
    int l = 0;
    char * buf = new char[BUFLEN];
    memcpy(buf, n, 5);
    std::cout << buf << std::endl;
    l += sizeof(n);
    // memcpy(buf + 5, &nulll, 1);
    // std::cout << std::string(buf) << std::endl;
    memcpy(buf + 5, &d, sizeof(float));
    l = 0;
    // D = (float*)(&buf[5]);
    // std::cout << buf << *D<< std::endl;
    int i;
    std::string name_0;
    for(i = 0; i<(BUFLEN-1); i++){
        if(buf[i] == '\0'){
            std::cout << "null!" << std::endl;
            break;
        }
        else
        name_0.push_back(buf[i]);
    }
    memcpy(&D, buf + i + 1, sizeof(float));

    // memcpy(&N , buf, sizeof(n)+1);
    // memcpy(&D, buf + sizeof(n)+1, sizeof(d));

    // std::cout << " : " << N << " : " << D << " : " <<std::endl;

    // foo << static_cast<char>(61) << "name" << '\0' << static_cast<char>(65) << "dataofsize12" ;
    // food7 = foo.str();
    // std::cout << ":" << foo.str() << ":" << std::endl;

    // foo.str("");

    // std::cout << ":" << foo.str() << ":" << std::endl;
    
    // foo << static_cast<char>(61) << "name" << '\0' << static_cast<char>(65) << "dataofsize12" << std::endl;
    


    std::cout << " : " << name_0 << " : " << D << " : " << std::endl;

   //parser(serialize(InputMap),InputMap);

return 0;
}
