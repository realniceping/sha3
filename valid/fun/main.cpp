#include <stdlib.h>
#include <stdio.h>
#include <winsock.h>

int main(){
 
    WSADATA wsaData;

    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if(iResult != 0){
        printf("WSA fail to start with: %d \n", iResult);
        return 1;
    }


    return 0;   

    FILE* fptr;     
    int fbody;

    fptr = fopen("test.txt", "w");

    if(fptr == NULL){
        printf("Err");
        return 1;
    }
    printf("File opened\n");
    printf("Enter file body(int): ");
    scanf("%d", &fbody);
    fprintf(fptr, "%d", fbody);
    fclose(fptr);


}