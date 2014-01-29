template std::vector<class T> class SubjectInputList
{
  bool isCamera;
  std::string name;
  std::vector<T> X_Y_Z;
  std::vector<T> Roll_Pitch_Yaw;

public:  
  str::string setName(std::string n)
  {name = n;	return name;};
  std::string getName(std::string)
  {return name;};

  std::vector<T> * setTranslation(std::vector<T>& x_y_z)
  { X_Y_Z = x_y_z;     return X_Y_Z;};
  std::vector<T> * setOrientationEuler(std::vector<T>& r_p_y);
  { Roll_Pitch_Yaw = r_p_y;	return Roll_Pitch_Yaw;};
  std::vector<T> * getTranslation()
  {return X_Y_Z;};
  std::vector<T> * getOrientationEuler()
  {return Roll_Pitch_Yaw;};
}

 
  
  
