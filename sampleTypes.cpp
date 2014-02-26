#include <cstdlib>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <vector>
#include <functional>
/**
 * Name: serializer
 * Description: a method to serialize data or a data structure for UDP
 *              transportation
 * Arguments: data = Data structure to be serialized
 * Returns: returns string to be pushed to receiver
 */
template<typename T> std::string serializer(T * data){};

/**
 * Name: parser
 * Description: a method to parse a chunk of the incoming data stream and
 *              returns the typed data
 * Arguments: data = raw data in a string to be parsed
 * Returns: returns pointer to the parsed data
 */

template<typename T>
T * parser(std::string data){};

template<> int * parser(std::string * data){};
template<> double * parser(std::string * data){};
template<> char * parser(std::string * data){};
template<> bool * parser(std::string * data){};
template<> std::vector * parser(std::string * data){};
template<> std::string * parser(std::string * data){};
template<> std::map * parser(std::string * data){};
/**
 * Name: MapNode
 * Description: A template class for a map node
 */
template<typename T>
class MapNode
{
public:
  MapNode(std::string n, T* d, std::function<std::string *> s, std::function<T*> p );
  MapNode(std::string n, T* d )

  static std::string name;
  type_info dataType;
  T * data;
  //function pointer for serializer method
  std::string * (* serialize)(T *);
  //function pointer for parser method
  T * (* parse)(std::string *);
};
template<typename T>
MapNode::MapNode(std::string n, T* d) {
  name = n;
  data = d;
  serialize = &serializer<T>;
  parse = &parser<T>;
};




int main(){
  MapNode<int> * node1 = new MapNode(string("first"),5);
  delete node1;
}


// #include <iostream>
// #include <cstdlib>
// using namespace std;

// template<class T> void quicksort(T a[], const int& leftarg, const int& rightarg)
// {
//   if (leftarg < rightarg) {

//     T pivotvalue = a[leftarg];
//     int left = leftarg - 1;
//     int right = rightarg + 1;

//   for(;;) {

//     while (a[--right] > pivotvalue);
//     while (a[++left] < pivotvalue);

//     if (left >= right) break;

//     T temp = a[right];
//     a[right] = a[left];
//     a[left] = temp;
//   }

//   int pivot = right;
//   quicksort(a, leftarg, pivot);
//   quicksort(a, pivot + 1, rightarg);
//   }
// }

// int main(void) {
//   int sortme[10];

//   for (int i = 0; i < 10; i++) {
//     sortme[i] = rand();
//     cout << sortme[i] << " ";
//   };
//   cout << endl;

//   quicksort<int>(sortme, 0, 10 - 1);

//   for (int i = 0; i < 10; i++) cout << sortme[i] << "
//   ";
//   cout << endl;
//   return 0;
// }