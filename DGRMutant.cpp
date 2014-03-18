// Authors:
// James Walker   jwwalker at mtu dot edu
// Scott A. Kuhl  kuhl at mtu dot edu

#include <GL/glut.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <typeinfo>

#define RELAY_LISTEN_PORT 25885
#define SLAVE_LISTEN_PORT 25884
#define BUFLEN 512

using namespace std;
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
 * Description: A template class for a map node
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

};

#ifdef DGR_MASTER // if MASTER:
char *RELAY_IP = NULL;

struct sockaddr_in si_other;
int slen, s;
char buf [BUFLEN];
int so_broadcast = 1;
pthread_t senderThread;

#else // if SLAVE:

int s, milliseconds;
struct timespec req;
pthread_t receiverThread;

struct sockaddr_in si_me, si_other;
int slen;

bool receivedPacket = false;
int framesPassed = 0;

// command-line parameters
double frustum_left,frustum_right,frustum_bottom,frustum_top;
int screen_width,screen_height;
#endif

MapNode<float> * node1 = new MapNode<float>(std::string("node1"),0.0f);
MapNode<float> * node2 = new MapNode<float>(std::string("node2"),0.0f);
MapNode<float> * node3 = new MapNode<float>(std::string("node3"),0.0f);
MapNode<float> * node4 = new MapNode<float>(std::string("node4"),0.0f);

float data1 = *(node1->getData());
float data2 = *(node2->getData());
float data3 = *(node3->getData());
float data4 = *(node4->getData());

std::map<std::string,MapNodePtr *> InputMap = {

// parameters common to both MASTER and SLAVE
// ADD YOUR STATE PARAMETERS THAT NEED TO BE PASSED FROM MASTER TO SLAVE HERE.
    
    {node1->name, (MapNodePtr *) node1},
    {node2->name, (MapNodePtr *) node2},
    {node3->name, (MapNodePtr *) node3},
    {node4->name, (MapNodePtr *) node4}
  };

// Helper function for splitting strings along a delimiter (such as ~)
vector<string> &split(const string &s, char delim, vector<string> &elems) {
  stringstream ss(s);
  string item;
  while (getline(ss, item, delim)) {
    elems.push_back(item);
  }
  return elems;
}

// Helper function for splitting strings along a delimiter (such as ~)
vector<string> split(const string &s, char delim) {
  vector<string> elems;
  return split(s, delim, elems);
}

// Exit with error message
void error(const char *msg) {
  perror(msg);
  exit(1);
}

// Register a callback that is called when the program exits so we can be
// sure to close the port we're using.
void exitCallback() {
  close(s);
}

void init(void) {
  glClearColor (0.0, 0.0, 0.0, 0.0);
  glShadeModel (GL_FLAT);
}

// OpenGL display function.
// Your graphics rendering code will go here.
// If you do any state updating in your display function, be sure to do it for the MASTER ONLY.
void display(void) {

#ifdef DGR_MASTER   // All code that updates state variables should be exclusive to the MASTER.
                    // Forbidding the SLAVES from updating state variables and only getting them
                    // from the MASTER is what guarantees that the processes all stay synchronized.
  data1 += 1.0f;
  data2 += 2.0f;
  data3 += 5.0f;
  data4 += 0.01f;

#else  // The slave automatically shuts itself off if it hasn't received
       // any packets within a few seconds (it gives itself longer if it
       // hasn't received any packets at all yet)
       // Assumes a 60fps framerate
  framesPassed++;
  if (receivedPacket) {
    if (framesPassed > 180) exit(EXIT_SUCCESS);
  } else {
    if (framesPassed > 900) exit(EXIT_SUCCESS); // If your program takes a very long time to initialize,
                                                // you can increase this value so the slaves don't prematurely
                                                // shut themselves off.
  }

#endif

  // Display code common to both the MASTER and SLAVE (except the frustum call)
  // This simple example just displays a wireframe cube and slowly rotates it.
  glClear (GL_COLOR_BUFFER_BIT);
  glColor3f (1.0, 1.0, 1.0);
  glLoadIdentity ();
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  float x = 0;
  float y = 0;
  float z = 1.5;

#ifdef DGR_MASTER
  glFrustum (-1.03*3-x, 1.03*3-x, .28-z, 2.6-z, 3.9-y, 5000); // edit the 0.1,5000 if you want to change the near/far clipping distance
#else
  glFrustum (frustum_left-x, frustum_right-x, frustum_bottom-z, frustum_top-z, 3.9-y, 5000); // edit the 0.1,5000 if you want to change the near/far clipping distance
#endif
  glMatrixMode (GL_MODELVIEW);
  gluLookAt (0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
  glTranslatef(0,0,-30);
  glScalef (8.0, 8.0, 8.0);
  glPushMatrix();
  glColor3ub(data2,data3,0);
  glRotatef(data1, 0.0f, 1.0f, 0.0f);
  glutWireCube (1.0);
  glPopMatrix();
  glColor3ub(data2,data3,0);
  glRotatef(data1, 0.0f, 1.0f, 0.0f);
  glutWireCube (data4);

  
  glutSwapBuffers();
  glutPostRedisplay();
}

#if DGR_MASTER // if MASTER:
// The MASTER sends all state data to the RELAY (which is run on the IVS head node)
// via UDP packets in an infinite loop.
void sender() {
  
    while (true) 
    {
        //packet_buffer properties
        char * packet_buffer = new char[BUFLEN];
        int packet_length = 0;
        unsigned char packet_counter = 0;
        //current node properties
        char * node_buf;
        int node_length = 0;
        int node_counter = 0;

        bool first_node = true;
        bool last_node = false;


        for(auto it = InputMap.begin();it!= InputMap.end();it++)
        {

            node_length = 0;
            //if(std::next(it) == InputMap.end()){ last_node = true;}

            MapNodePtr * cur_node = it->second;
            std::string cur_name = cur_node->name;
            char* cur_data = cur_node->getDataString();
            int cur_data_length = cur_node->dataLength;

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
                if (sendto(s, packet_buffer, packet_length, 0, (struct sockaddr*)&si_other,slen) == -1) error ("ERROR sendto()");
                packet_counter ++;
                packet_length = 0;
                first_node = true;
                it--;

            }
            else
            { // add node to packet buffer
                memcpy(packet_buffer + packet_length, node_buf, node_length);
                packet_length += node_length;
                first_node = false;
                node_counter ++;
            }

        }
        if (sendto(s, packet_buffer, packet_length, 0, (struct sockaddr*)&si_other,slen) == -1) error ("ERROR sendto()");

    }
    usleep(32000);
}


#else // if SLAVE:

// The SLAVES receive state data from teh RELAY via UDP packets and parse the
// data around a delimiter (this example uses ~) in an infinite loop.
void receiver() {
  char packet_buffer[BUFLEN];

  MapNodePtr * cur_node;
  std::string node_name;
  int node_data_length, packet_cursor,

  while (true) {
    if (recvfrom(s, packet_buffer, BUFLEN, 0, (struct sockaddr*)&si_other,
      &slen) == -1) error("ERROR recvfrom()");
    receivedPacket = true;
    framesPassed = 0;
    while(packet_buffer[packet_cursor] > 31 && packet_cursor < BUFLEN){
        
        //extract name
        node_name = "";
        for(int i = packet_cursor; i < BUFLEN; i++) 
        {
            if(packet_buffer[i]) { node_name.push_back(packet_buffer[i]); packet_cursor++; }
            else { packet_cursor++; break; }
        }

        //get current node and data length
        cur_node = InputMap.at(node_name);
        node_data_length = cur_node->dataLength;

        char * node_data = new char[node_data_length];
        node_data = &packet_buffer[packet_cursor];
        //set data
        cur_node->setData(node_data);
        packet_cursor += node_data_length;
        //print data        
    }
  }
}
#endif

// MAIN FUNCTION
int main(int argc, char** argv) {

#ifdef DGR_MASTER
  if (argc != 2) {
    printf("USAGE: %s relay-ip-address\n", argv[0]);
    return 1;
  }
  RELAY_IP=argv[1];
#else // if SLAVE:
  frustum_left = atof(argv[1]);
  frustum_right = atof(argv[2]);
  frustum_bottom = atof(argv[3]);
  frustum_top = atof(argv[4]);
  screen_width = atoi(argv[5]);
  screen_height = atoi(argv[6]);
#endif

  atexit(exitCallback);

  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);

#ifdef DGR_MASTER
  glutInitWindowSize((1920*6)/8, (1080*4)/8);
    glutInitWindowPosition(0, 0);
    glutCreateWindow(argv[0]);
    // This simple example doesn't use input callbacks, but the lines commented out
    // below demonstrate that any input callbacks you use should be exclusive to the
    // MASTER and not used by the SLAVES.
    //glutKeyboardFunc(keyboard);
    //glutMouseFunc(processMouse);
    //glutMotionFunc(mousePressMove);
    //glutPassiveMotionFunc(mouseMove);
#else
  glutInitWindowSize (screen_width, screen_height); 
  glutInitWindowPosition (0, 0);
  glutCreateWindow ("DGR Slave Node");
#endif

  init ();
  glutDisplayFunc(display);
  //glutReshapeFunc(reshape);
  //glutIdleFunc(animate);

#ifdef DGR_MASTER
    // socket to send data to relay
    slen=sizeof(si_other);
    so_broadcast = 1;

    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) error("ERROR socket");

    setsockopt(s, SOL_SOCKET, SO_BROADCAST, &so_broadcast, sizeof(so_broadcast));

    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(RELAY_LISTEN_PORT);
    if (inet_aton(RELAY_IP, &si_other.sin_addr) == 0) {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }

    if (pthread_create(&senderThread, NULL, sender, NULL) != 0) {
        perror("Can't start thread, terminating\n");
        return 1;
    }
#else
  // Socket to read data from relay
  slen=sizeof(si_other);
  if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) error("ERROR socket");
  memset((char *) &si_me, 0, sizeof(si_me));
  si_me.sin_family = AF_INET;
  si_me.sin_port = htons(SLAVE_LISTEN_PORT);
  si_me.sin_addr.s_addr = htonl(INADDR_ANY);
  if (bind(s, (struct sockaddr*)&si_me, sizeof(si_me)) == -1) error("ERROR bind");

  // listen for updates
  if (pthread_create(&receiverThread, NULL, receiver, NULL) != 0) {
    perror("Can't start thread, terminating");
    return 1;
  }
#endif

  // go
  glutMainLoop();
  exit(EXIT_SUCCESS);
}
