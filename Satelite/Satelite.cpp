#include "Satelite.h"
//#include <iostream>
//#include <string>
using namespace std;

Satelite::Satelite()
{

}

/*void Satelite::imprimirMensagem() {
    //cout << "Mensagem: " << endl;
}*/

//Calc. the rectangle area and return
float Satelite::areaRetangulo(int x, int y) {
    return x * y;
}

//Calc. the triangle area and return
float Satelite::areaTriangulo(int x, int y) {
    return (x * y)/2;
}

/*int Satelite::escolheArea() {
    //recebe a entrada do usuario
    int escolha = true;

    return escolha;
}*/

//Returns 1 if login and password are correct, else return 0
int Satelite::authenticate(char login[], char password[]) {
    int i,temp=0;
    char loginTrue[50] = {'s','a','t', 'e', 'l', 'i', 't', 'e', '\0'}; //Right login
    char passwordTrue[20] = {'1','2','3', '4', '5', '6', '\0'};//Right password
    
    //Compare login
    for (i=0; loginTrue[i]!='\0'||login[i]!='\0';i++)
    {
        if (loginTrue[i]!=login[i])
        {
            temp=1;
            break;
        }
    }

    //Compare password
    for (i=0; passwordTrue[i]!='\0'||password[i]!='\0';i++)
    {
        if (passwordTrue[i]!=password[i])
        {
            temp=1;
            break;
        }
    }

    if (temp==0)
        return 1;
    else
        return 0;
}