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

int main(){

  

    std::string n;
    std::string food1, food2, food3, food4, food5, food6, food7;
    char fine1,fine2;
    char fine3[18];
    char fine4[12];
    
    n = "brent";

    char * N;
    float D;
    float d = 75.1;
    int l = 0;
    char * buf = new char[BUFLEN];
    memcpy(buf + l, n.c_str(), n.length());

    l += n.length();
    buf[l] = '\0';
    l += sizeof(char);

    memcpy(buf + l, &d, sizeof(D));
    l = 0;
    // D = (float*)(&buf[5]);
    // std::cout << buf << *D<< std::endl;
    int i;
    bool name_found = false;
    std::string name_0;
    for(i = 0; i<(30); i++){
    
            std::cout << buf[i] << std::endl;
            if(!name_found) name_0.push_back(buf[i]);
            if(buf[i] == '\0') name_found = true;
        
  
    }
    memcpy(&D, buf + n.length() + 1, sizeof(D));

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
delete buf;
   //parser(serialize(InputMap),InputMap);

return 0;
}