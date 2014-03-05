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

#define RELAY_LISTEN_PORT 25885
#define SLAVE_LISTEN_PORT 25884
#define BUFLEN 512

using namespace std;

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
  int packetLength;
};

template<typename T>
MapNode<T>::MapNode(std::string n, T* d) {
  name = n;
  packetData.name = &n[1];
  packetData.data = *d;
  dataType = typeid(*d).name();
  packetLength = sizeof(packetData.name) + sizeof(packetData.data);
};
template<typename T>
MapNode<T>::MapNode(std::string n, T d) {
  name = n;
  packetData.name = &n[1];
  packetData.data = d;
  dataType = typeid(d).name();
  packetLength = sizeof(packetData.name) + sizeof(packetData.data);
};
int serialize(std::map<std::string, MapNodePtr *> InputMap)
{
  char buf [BUFLEN];
  int length = 0;
  void * sendStruct;

  for(auto it = InputMap.begin();it!= InputMap.end();it++)
  {

    if(length >= (BUFLEN - 15))
    {
      return 0;
    }
    if(it->second->dataType == typeid(float).name()){ 
      MapNode<float> * cur_node = (MapNode<float> *) it->second; 
      sendStruct = &(cur_node->packetData);
      length = cur_node->packetLength;
      std::cout << cur_node->name << " " << cur_node->packetData.data << " " << length << std::endl;
    }
    else if(it->second->dataType == typeid(double).name()){ 
      MapNode<double> * cur_node = (MapNode<double> *) it->second; 
      sendStruct = &(cur_node->packetData);
      length = cur_node->packetLength;
      std::cout << cur_node->name << " " << cur_node->packetData.data << " " << length << std::endl;
    }
    else if(it->second->dataType == typeid(int).name()){ 
      MapNode<int> * cur_node = (MapNode<int> *) it->second; 
      sendStruct = &(cur_node->packetData);
      length = cur_node->packetLength;length = cur_node->packetLength;
      std::cout << cur_node->name << " " << cur_node->packetData.data << " " << length << std::endl;
    }
    else if(it->second->dataType == typeid(bool).name()){ 
      MapNode<bool> * cur_node = (MapNode<bool> *) it->second; 
      sendStruct = &(cur_node->packetData);
      length = cur_node->packetLength;
      std::cout << cur_node->name << " " << cur_node->packetData.data << " " << length << std::endl;
    }
    else if(it->second->dataType == typeid(std::string).name()){ 
      MapNode<std::string> * cur_node = (MapNode<std::string> *) it->second; 
      sendStruct = &(cur_node->packetData);
      length = cur_node->packetLength;
      std::cout << cur_node->name << " " << cur_node->packetData.data << " " << length << std::endl;
    }
    else{ return 1;}

    if (sendto(s, sendStruct, length, 0, (struct sockaddr*)&si_other,slen) == -1) error ("ERROR sendto()");

    
  }
  return 0;
}




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

std::map<std::string,float> InputMap = {
// parameters common to both MASTER and SLAVE
// ADD YOUR STATE PARAMETERS THAT NEED TO BE PASSED FROM MASTER TO SLAVE HERE.
    {"data1",0.0f},
    {"data2",0.0f},
    {"data3",0.0f},
    {"data4",0.0f}
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
  InputMap.at("data1") += 1.0f;
  InputMap.at("data2") += 2.0f;
  InputMap.at("data3") += 5.0f;
  InputMap.at("data4") += 0.01f;

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
  glColor3ub(InputMap.at("data2"),InputMap.at("data3"),0);
  glRotatef(InputMap.at("data1"), 0.0f, 1.0f, 0.0f);
  glutWireCube (1.0);
  glPopMatrix();
  glColor3ub(InputMap.at("data2"),InputMap.at("data3"),0);
  glRotatef(InputMap.at("data1"), 0.0f, 1.0f, 0.0f);
  glutWireCube (InputMap.at("data4"));

  
  glutSwapBuffers();
  glutPostRedisplay();
}

#if DGR_MASTER // if MASTER:
// The MASTER sends all state data to the RELAY (which is run on the IVS head node)
// via UDP packets in an infinite loop.
void sender() {
  
  while (true) 
  {
    
    char buf [BUFLEN];
    int length = 0;
    void * sendStruct;

    for(auto it = InputMap.begin();it!= InputMap.end();it++)
    {

      if(length >= (BUFLEN - 15))
      {
        return 0;
      }
      if(it->second->dataType == typeid(float).name())
      { 
        MapNode<float> * cur_node = (MapNode<float> *) it->second; 
        sendStruct = &(cur_node->packetData);
        length = cur_node->packetLength;
        std::cout << cur_node->name << " " << cur_node->packetData.data << " " << length << std::endl;
      }
      else if(it->second->dataType == typeid(double).name())
      { 
        MapNode<double> * cur_node = (MapNode<double> *) it->second; 
        sendStruct = &(cur_node->packetData);
        length = cur_node->packetLength;
        std::cout << cur_node->name << " " << cur_node->packetData.data << " " << length << std::endl;
      }
      else if(it->second->dataType == typeid(int).name())
      { 
        MapNode<int> * cur_node = (MapNode<int> *) it->second; 
        sendStruct = &(cur_node->packetData);
        length = cur_node->packetLength;length = cur_node->packetLength;
        std::cout << cur_node->name << " " << cur_node->packetData.data << " " << length << std::endl;
      }
      else if(it->second->dataType == typeid(bool).name())
      { 
        MapNode<bool> * cur_node = (MapNode<bool> *) it->second; 
        sendStruct = &(cur_node->packetData);
        length = cur_node->packetLength;
        std::cout << cur_node->name << " " << cur_node->packetData.data << " " << length << std::endl;
      }
      else if(it->second->dataType == typeid(std::string).name())
      { 
        MapNode<std::string> * cur_node = (MapNode<std::string> *) it->second; 
        sendStruct = &(cur_node->packetData);
        length = cur_node->packetLength;
        std::cout << cur_node->name << " " << cur_node->packetData.data << " " << length << std::endl;
      }
      else
      { return 1;}

      if (sendto(s, sendStruct, length, 0, (struct sockaddr*)&si_other,slen) == -1) error ("ERROR sendto()");

      
    }
    usleep(32000);
  }
}


#else // if SLAVE:

// The SLAVES receive state data from teh RELAY via UDP packets and parse the
// data around a delimiter (this example uses ~) in an infinite loop.
void receiver() {
  char buf[BUFLEN];
  vector<string> splits;
  vector<string> packet;
  //printf("in receiver");
  while (true) {
    if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr*)&si_other,
      &slen) == -1) error("ERROR recvfrom()");
    receivedPacket = true;
    framesPassed = 0;
    string itrmdt(buf);
    //printf("recieved: %s\n", buf);
    splits = split(itrmdt, '~');
    // NOTE: This simple example only sends/receives a single value (rotation),
    // but it sends rotation twice, separated by a ~ in order to demonstrate the
    // technique of how you can send multiple values separated by ~ and then
    // get the values back out, as shown here.
    for(auto splitsIter = splits.begin(); splitsIter != splits.end(); splitsIter++)
    {
      packet = split(*splitsIter, '`');
      InputMap.at(packet[0]) = (float)atof(packet[1].c_str());
      
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
