class InputNode
{
protected:
  std::string name;
  std::vector< * InputNode> children;
public:
  InputNode(std::string n ): name(n) {}
  InputNode(std::string n, std::vector<InputNode> c): name(n), children(c) {}
  std::string getName() {return name};
  std::vector<InputNode> getChildren() {return children};

};

class InputFloat : public InputNode
{
protected:
  std::vector<float> values;
public:
  InputNodeFloat(std::string n, std::vector<float> v) : InputNode(n), values(v) {}
  InputNodeFloat(std::string n, std::vector<float> v, std::vector<InputNode> c) : InputNode(n, c), values(v) {}
  std::vector<float> getValues() {return values};
};

class InputDouble : public InputNode
{
protected:
  std::vector<double> values;
public:
  InputNodeFloat(std::string n, std::vector<double> v) : InputNode(n), values(v) {}
  InputNodeFloat(std::string n, std::vector<double> v, std::vector<InputNode> c) : InputNode(n, c), values(v) {}
  std::vector<double> getValues() {return values};
};


