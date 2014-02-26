// class Type
// {
// public:
//   virtual ~Type(){};
//   virtual void * allocate()const=0;
//   virtual void * cast(void * obj)const=0;

// };

// template<typename T> 
// class TypeImpl : public Type
// {
// public:
//   virtual void * allocate()const{ return new T; }
//   virtual void * cast(void * obj)const{ return static_cast<T*>(obj); }
// };
// /**
//  * Type * type = new TypeImpl<int>;
//  * void * myint = type->allocate();
//  * int i = type->cast(myint); 
//  */
template<typename T>
class MapNode
{
  static std::string name;
  type_info dataType;
  T * Data;
  //function pointer for serializer method
  std::str (* serialize)(T *);
  //function pointer for parser method
  void (* parse)(std::string *);
};
template<typename T>
T parse(std::string str){};
template<> int parse<int>()


