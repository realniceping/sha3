#include <iostream>
#include <cstring>

int main(){
    char * a = new char[199];
    for (size_t i = 0; i < 199; i++)
    {
        a[i] = 0x01;
    }
    std::cout << strlen(a) << std::endl;

    


    return 0;
}