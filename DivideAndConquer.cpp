#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <unordered_map>

using std::vector; 
using namespace std;



//--------------------------------------------------------------STUCTS---------------------------------------------------------------
struct Pixel{
    // Anterior
    // Siguiente
    int colorRGB;
    vector<Pixel*> *diagonal;
    int posDiagonal;
    tuple<int, int> xy;

    Pixel(vector<Pixel*> diagonal, int color, int x, int y){
        this->colorRGB = color;
        this->posDiagonal = diagonal.size();
        this->diagonal = &diagonal;
        this->diagonal->push_back(this);
        this->xy = make_tuple(x,y);
    }
};

//-----------------------------------------------------------------------------------------------------------------------------------


//--------------------------------------------------------------FUNCIONES---------------------------------------------------------------

//Funcion que compara los puntos 
bool compare(Pixel *puntoClave, Pixel puntos2){

	vector<Pixel*> tempDiagonal = *puntoClave->diagonal;
	tempDiagonal = tempDiagonal[pos-50,pos+50];
	Pixel pixelAux = puntos2;

	vector<int> tempDiagonal2 = getDiagonal(puntos2);

	for(int i = -50 ; i < 50; i++){
		int x = pixel->x + i;
		int y = pixel->y + i;
		int rgb = getColor(x,y);
	}

    //FIRMA ALEX POR SI ACASO
	return false;	
}

//Funcion para el divide y venceras
void divideAndConquer(Pixel puntoClave, vector<Pixel*> puntos2, int cont){
	if (puntos2.size() > 1)
		cont += divideAndConquer(puntoClave, puntos2(puntos2.begin(), puntos2.begin() + puntos2.size()-1/2, cont));
	    cont += divideAndConquer(puntoClave, puntos2(puntos2.begin() + puntos2.size()-1/2, puntos2.end(), cont), cont);
    else
    	return compare(puntoClave, puntos2[0]);  //compare=in progress

}
unordered_map<int, tuple<vector<Pixel*>,vector<Pixel*>>> image1ColorMap;

//Funcion auxiliar para divide and conquer
void auxDivideAndConquer(){
	for (auto color: image1ColorMap){
		for (auto puntosClaves : image1ColorMap){    //hash tupla pos 0
			divideAndConquer(puntosClaves, colores, 0);

		}

	}

}



//-----------------------------------------------------------------------------------------------------------------------------------


int main(){

	cout<<"HOLA MUNDO"<<endl;

	return 0;
}
