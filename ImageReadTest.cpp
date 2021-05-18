#include <iostream>
#include <vector>
#include <tuple>
#include <unordered_map>
#include <string>
#include <chrono>
#include <algorithm>
#include <math.h>

using namespace std;

typedef tuple<int, int> tupleOf2;
typedef tuple<unsigned char, unsigned char, unsigned char, string, string> tupleRGB;


extern "C" {
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
}

void pointsImg2(vector<unsigned char> image1);


struct Pixel{
    // Anterior
    // Siguiente
    int colorRGB;
    vector<Pixel*> *diagonal;
    int posDiagonal;
    tuple<int, int> xy;
    int centro;

    Pixel(vector<Pixel*> diagonal, int color, int x, int y){
        this->colorRGB = color;
        this->posDiagonal = diagonal.size();
        this->diagonal = &diagonal;
        this->diagonal->push_back(this);
        this->xy = make_tuple(x,y);
    }

    Pixel(vector<Pixel*> diagonal, int pos, int color, int x, int y){
        this->colorRGB = color;
        this->posDiagonal = pos;
        this->diagonal = &diagonal;
        //this->diagonal->push_back(this);
        this->xy = make_tuple(x,y);
    }

};


vector<vector<Pixel*>> diagonalesImg1;
vector<vector<Pixel*>> diagonalesImg2;


//Original code of image and pixel management: https://www.cplusplus.com/forum/beginner/267364/


// Loads as RGBA... even if file is only RGB
// Feel free to adjust this if you so please, by changing the 4 to a 0.
bool load_image(vector<unsigned char>& image, const string& filename, int& x, int& y)
{
    int n;
    unsigned char* data = stbi_load(filename.c_str(), &x, &y, &n, 4);
    if (data != nullptr)
    {
        image = vector<unsigned char>(data, data + x * y * 4);
    }
    stbi_image_free(data);
    return (data != nullptr);
}


//  Simulation of how the pixels would be read according to the algorithm
void sections(int image1Height, int image1Width, int sectionHeight, int sectionWidth) {
    cout << "secciones" << endl;
    int fixedJ = 0;
    for (int i = 0; i <= image1Height; i++, fixedJ++) {
        if (fixedJ >= sectionWidth) {
            fixedJ = 0;
        }
        cout << i << ":  " ;
        vector<char> vector1(image1Width, 'o');

        for (int j = fixedJ; j <= image1Width; j += sectionWidth) {
            vector1[j] = '-';
        }
        for (char c : vector1) {
            cout << c ;
        }
        cout << endl;

    }
    /*for (int y = 0; y < image1Height; y++) {
        for (int x = 0; x < image1Width; x += sectionWidth) {
            sectionsVector.emplace_back(x, y);
            //cout << "x,y = " << x <<", "<< y << endl;
        }
    }*/
    cout << "fin de secciones";
}


// ESTO ES DEMASIADOOOO LARGO DE HACER O(n^2)
void comparisson(const vector<tuple<string,string>>& image1RgbValues, const vector<tuple<string,string>>& image2RgbValues){

    int subImagesFromImage1FoundInImage2 = 0;

    for (auto & image1RgbValue : image1RgbValues) {

        for (auto & image2RgbValue : image2RgbValues) {

            if (get<0>(image1RgbValue) == get<0>(image2RgbValue)) {
                subImagesFromImage1FoundInImage2++;
                cout << "Pair at :" << get<1>(image1RgbValue) << " and " << get<1>(image2RgbValue)<< endl;
            }
        }
    }
    cout << "Answer: " << subImagesFromImage1FoundInImage2;
}


unordered_map<int, tuple<vector<Pixel*>,vector<Pixel*>>> image1ColorMap;

const size_t RGBA = 4;

char a;
int sectionWidth, sectionHeight;


void getImagesPixels(const string& image1Filename) {

    vector<unsigned char> image1;

    int image1Width, image1Height;

    bool success = load_image(image1, image1Filename, image1Width, image1Height);
    cout << "Height: " << image1Height;
    cout << "  Width: " << image1Width;
    cin >> a;

    if (!success)
    {
        cout << "Error loading image 1 \n";
        return;
    }


    // For each row, get a pixel  every subImageWidth colums
    // Each time we go to the next row, start in different column, the next one to the right
    // so it does a stare running.


    // key: RGB, value: counter of apparitions
    unordered_map<string, int> image2ColorMap;

    // string 1: RGB String, String 2: x,y String
    vector<tuple<string,string>> image1RgbValues;



    cout << "lectura imagenes terminada, diagonales que deberia crear " << ceil(image1Width/sectionWidth) << endl;
    cin >> a;

    int cuantity = ceil(image1Width/sectionWidth);
    for (int diagonal = 0; diagonal <= cuantity; diagonal++){
        diagonalesImg1.push_back({});
    }
    cout << "creo diagonales";


    int fixedJ = 0, rgb;

    for (int i = 0; i < image1Width; i++, fixedJ++) {
        if (fixedJ >= sectionWidth) {
            fixedJ = 0;

            diagonalesImg1.insert(diagonalesImg1.begin(), {});

        }
        int diagonPosition = 0;
        for (int j = fixedJ; j < image1Height; j += sectionWidth) {
            //cout << "x, y = " << i << "," <<j<< endl;
            int temporalIndex = RGBA * (j * image1Width + i);

            int r = (static_cast<int>(image1[temporalIndex + 0]));
            int g = (static_cast<int>(image1[temporalIndex + 1]));
            int b = (static_cast<int>(image1[temporalIndex + 2]));
            rgb = ((r&0x0ff)<<16)|((g&0x0ff)<<8)|(b&0x0ff);

            Pixel *newPix = new Pixel(diagonalesImg1[diagonPosition],rgb,j,i);
            diagonalesImg1[diagonPosition].push_back(newPix);
/*            cout << "nuevo pixel generado en diagonal : " << diagonPosition << endl;
            cout << diagonalesImg1.size();*/
            if(i % (sectionWidth/2) == 0 ) {
                if (image1ColorMap.find(rgb) == image1ColorMap.end()){
                    tuple<vector<Pixel *>, vector<Pixel *>> newKey({}, {});
                    image1ColorMap[rgb] = newKey;
                }
                get<0>(image1ColorMap[rgb]).push_back(newPix);
                image1RgbValues.push_back(make_tuple((to_string(r)+ "-" +to_string(g)+ "-" +to_string(b)),(to_string(i)+","+to_string(j))));
                //cout << get<0>(image1RgbValues.back()) << "color" << rgb << " at " << get<1>(image1RgbValues.back())<< endl;
            }
            diagonPosition++;
        }
    }

/*
    cout << "los puntos claves son:" << endl;
    for(auto color : image1RgbValues){
        cout << get<0>(color) << " at " << get<1>(color)<< endl;
    }
*/

    cout << "vectores imagenes terminado, colores encontrados" << image1RgbValues.size() << endl;
    cin >>a;

    for(Pixel* pix: diagonalesImg1[0]){
        cout << "i.j" << get<0>(pix->xy) << "." << get<1>(pix->xy) << "color:  " << pix->colorRGB << endl;
    }
    for (Pixel *pixel : *((get<0>(image1ColorMap[16777215])).at(0))->diagonal) {
        cout << pixel->colorRGB << " i.j " << get<0>(pixel->xy)<< "." << get<1>(pixel->xy) << endl;
    }

}



void compareDiagons(Pixel *pixImg1, Pixel *pixImg2){
    int ubic = sectionWidth/2 * -3;
    int cont = 0;

    int pos2Find = pixImg1->posDiagonal;
    vector<Pixel *> *diagon = pixImg1->diagonal;
    int color1 = diagon->at(pos2Find+ubic)->colorRGB;

    //int temporalIndex = RGBA * (get<1>(pixImg2->xy)+ubic * image2Width + get<0>(pixImg2->xy)+ubic);
    /*int r = (static_cast<int>(image2[temporalIndex + 0]));
    int g = (static_cast<int>(image2[temporalIndex + 1]));
    int b = (static_cast<int>(image2[temporalIndex + 2]));

    int color2 = ((r&0x0ff)<<16)|((g&0x0ff)<<8)|(b&0x0ff);

    if(color1 == color2){}*/






}

bool cmp(pair<int, int>& a,
         pair<int, int>& b)
{
    return a.second > b.second;
}

unordered_map<int, int> colorAparitions;

vector<unsigned char> image2;

void getDiagons(Pixel *pix, int &image2Width, int &image2Height){
    //REVISAR !!!
    vector<Pixel*> *diagon = (pix->diagonal);
   /* diagonalesImg2.push_back({});
    vector<Pixel*> diagon = diagonalesImg2.back();*/

    /*cout << "Diagonal antes" << endl;
    for (auto dig : *diagon) {
        cout << dig->colorRGB << " i.j " << get<0>(dig->xy)<< "." << get<1>(dig->xy) << endl;
    }*/
    Pixel *newPix;

    int temporalIndex, x, y;
    int halfHeight = (sectionHeight/2) ;


    /*cout <<"i.j Original:  " << get<0>(pix->xy) << "," << get<1>(pix->xy)<< endl;
    cout << "Hlaf Height" << halfHeight<< endl;*/
    for (int i = -1; i >= -4 ; --i) {
        if(i != -1) {
            x = get<0>(pix->xy)-halfHeight;
            y = get<1>(pix->xy)-halfHeight;
            halfHeight += halfHeight;
        }
        else{
            x = get<0>(pix->xy) - 1;
            y = get<1>(pix->xy) - 1;
        }

        if(x >= 0 && y >= 0) {
            temporalIndex = RGBA * (y * image2Width + x);
            int r = (static_cast<int>(image2[temporalIndex + 0]));
            int g = (static_cast<int>(image2[temporalIndex + 1]));
            int b = (static_cast<int>(image2[temporalIndex + 2]));
            int rgb = ((r & 0x0ff) << 16) | ((g & 0x0ff) << 8) | (b & 0x0ff);

            newPix = new Pixel(*diagon,0, rgb, x, y);
            /*auto it = diagon->begin();
            diagon->insert(it,newPix);*/

            diagon->push_back(newPix);
            rotate(diagon->rbegin(), diagon->rbegin() + 1, diagon->rend());

            /*cout << "diagonal luego de insert " << endl;
            for (auto dig : *diagon) {
                cout << dig->colorRGB << " i.j " << get<0>(dig->xy)<< "." << get<1>(dig->xy) << endl;
            }*/


        }

    }
    pix->centro = diagon->size()-1;


    halfHeight = (sectionHeight/2) ;


    //cout << "Hlaf Height" << halfHeight<< endl;
    for (int i = -1; i >= -4 ; --i) {
        if(i != -1) {
            x = get<0>(pix->xy)+halfHeight;
            y = get<1>(pix->xy)+halfHeight;
            halfHeight += halfHeight;
        }
        else{
            x = get<0>(pix->xy) + 1;
            y = get<1>(pix->xy) + 1;
        }

        if(x < image2Width and y < image2Height) {
            temporalIndex = RGBA * (y * image2Width + x);
            int r = (static_cast<int>(image2[temporalIndex + 0]));
            int g = (static_cast<int>(image2[temporalIndex + 1]));
            int b = (static_cast<int>(image2[temporalIndex + 2]));
            int rgb = ((r & 0x0ff) << 16) | ((g & 0x0ff) << 8) | (b & 0x0ff);

            newPix = new Pixel(*diagon,0, rgb, x, y);
            diagon->push_back(newPix);

            /*auto it = diagon->begin()+ diagon->size();
            diagon->insert(it,newPix);*/

            //diagon->push_back(newPix);


            //cout << newPix->colorRGB << " i.j " << get<0>(newPix->xy)<< "." << get<1>(newPix->xy) << endl;

        }
    }

    /*cout << "Diagonal todos for" << endl;
    for (auto dig : *diagon) {
        cout << dig->colorRGB << " i.j " << get<0>(dig->xy)<< "." << get<1>(dig->xy) << endl;
    }*/
    //*pix->diagonal = *diagon;
}

void pointsImg2(const string& image2Filename){

    int image2Width, image2Height;

    bool success = load_image(image2, image2Filename, image2Width, image2Height);
    cout << "Height: " << image2Height;
    cout << "  Width: " << image2Width;
    cin >> a;

    if (!success)
    {
        cout << "Error loading image 2 \n";
        return;
    }


    vector<tuple<string,string>> image2RgbValues;
    //vector i,j Visitados
    int posDiagonal = 0;
    for (int i = 0; i < image2Width; i++) {
        for (int j = 0; j < image2Height; j++) {

            int temporalIndex = RGBA * (j * image2Width + i);

            int r = (static_cast<int>(image2[temporalIndex + 0]));
            int g = (static_cast<int>(image2[temporalIndex + 1]));
            int b = (static_cast<int>(image2[temporalIndex + 2]));
            int rgb = ((r&0x0ff)<<16)|((g&0x0ff)<<8)|(b&0x0ff);

            if (image1ColorMap.find(rgb) != image1ColorMap.end()) {
                vector<Pixel*> newDiag = {};
                Pixel *newPix = new Pixel({},diagonalesImg2.size(),rgb,i, j);

                cout << "Diagonal antes crearDiagon"<<newPix->diagonal->size()<<endl;

                getDiagons(newPix,image2Width,image2Height);
                //newDiag = *newPix->diagonal;
                diagonalesImg2.push_back(*newPix->diagonal);
                //diagonalesImg2.at(posDiagonal) = newDiag;
                cout << "Tamano de Diagonales2:  " << diagonalesImg2.size() << endl;
                cout << "Diagonal despues crearDiagon"<<newPix->diagonal->size()<<endl;

                /*for (Pixel *pixel : *newPix->diagonal) {
                    cout << pixel->colorRGB << " i.j " << get<0>(pixel->xy)<< "." << get<1>(pixel->xy) << endl;
                }*/



                /*for (Pixel *pixel : *newPix->diagonal) {
                    cout << pixel->colorRGB << " i.j " << get<0>(pixel->xy)<< "." << get<1>(pixel->xy) << endl;
                }*/
                //cout << (get<1>(image1ColorMap[rgb])).size();
                (get<1>(image1ColorMap[rgb])).push_back(newPix);
                image2RgbValues.push_back(make_tuple((to_string(r)+ "-" +to_string(g)+ "-" +to_string(b)),(to_string(i)+","+to_string(j))));
                /*cout << "aparicion de color " << (to_string(r)+ "-" +to_string(g)+ "-" +to_string(b))
                                    << "at: " << (to_string(i)+","+to_string(j))<< " color " << rgb << endl;*/
               /* if(i > 110 and j > 30 ){
                    if(i < 160 and j < 80 ){
                        cout << "aparicion de color " << (to_string(r)+ "-" +to_string(g)+ "-" +to_string(b)) << " / " << rgb
                             << " at: " << (to_string(j)+","+to_string(i))<< endl;
                    }

                }*/

                colorAparitions[rgb]++;
            }
        }
    }


    vector<pair<int, int>> elems(colorAparitions.begin(), colorAparitions.end());
    sort(elems.begin(), elems.end(), cmp);
    cout << "cantidad de elementos e vector2" << elems.size()<< endl;

    /*for(auto diagonal : diagonalesImg2){
        //cout << "\ncolor: " <<pix->colorRGB << " at i.j  " << get<0>(pix->xy) << "." << get<1>(pix->xy) << endl;
        for (Pixel *pixel : diagonal) {
            cout << pixel->colorRGB << " i.j " << get<0>(pixel->xy)<< "." << get<1>(pixel->xy) << endl;
        }
    }*/

    //sort(colorAparitions.begin(), colorAparitions.end(), cmp);
    cout << "encontrados en:" << endl;
    int color = elems[elems.size()/2].first;
    cout << color << "cantidad" << elems[elems.size()/2].second;
    cin >> a;
    //get<1>(image1ColorMap[color]);
    /*for(auto pix : get<1>(image1ColorMap[16777215])){
        cout << "\ncolor: " <<pix->colorRGB << " at i.j  " << get<0>(pix->xy) << "." << get<1>(pix->xy) << endl;
        for (Pixel *dig : diagonalesImg2[pix->posDiagonal]) {
            cout << dig->colorRGB << " i.j " << get<0>(dig->xy)<< "." << get<1>(dig->xy) << endl;
        }
    }*/
    cout << "vectores imagenes terminado, largo de cada vector=" << image2RgbValues.size() << endl;
    cin >>a;

}

//Funcion que compara los puntos
bool compare(Pixel *puntoClave, Pixel *puntos2){

    vector<Pixel*> Diagonal1;
    vector<Pixel*> Diagonal2 = diagonalesImg2[puntos2->posDiagonal];
    cout << Diagonal1.size() << Diagonal2.size();


    int temporalIndex, x, y;
    int halfHeight = (sectionHeight/2) ;

    int color1, color2;

    int pos;
    vector<Pixel*> tempDiagonal1 = *puntoClave->diagonal;
    for (int i = 0; i <= 4; ++i) {
        if (i!=0){
            pos = puntoClave->posDiagonal - halfHeight;
            halfHeight+=halfHeight;
        }
        else{
            pos = puntoClave->posDiagonal - 1;
        }
        if( pos >= 0){
            Diagonal1.push_back(tempDiagonal1.at(pos));
            rotate(Diagonal1.rbegin(), Diagonal1.rbegin() + 1, Diagonal1.rend());
        }
    }
    puntoClave->centro = Diagonal1.size();
    Diagonal1.push_back(puntoClave);
    halfHeight = (sectionHeight/2) ;
    for (int i = 0; i <= 4; ++i) {
        if (i!=0){
            pos = puntoClave->posDiagonal + halfHeight;
            halfHeight+=halfHeight;
        }
        else{
            pos = puntoClave->posDiagonal + 1;
        }
        if( pos < tempDiagonal1.size()){
            Diagonal1.push_back(tempDiagonal1.at(pos));
        }
    }
    int cont;
    int start;
    start = (puntoClave->centro > puntos2->centro) ? puntos2->centro : puntoClave->centro;

    int largo;
    largo = (Diagonal1.size() < Diagonal2.size()) ? Diagonal1.size() : Diagonal2.size();

    for (int i = -start; i <  largo; ++i) {
        color1 = Diagonal1.at(puntoClave->centro-start)->colorRGB;
        color2 = Diagonal2.at(puntos2->centro-start)->colorRGB;
        cout << color1 << " = " << color2 << endl;
        if(color1 == color2){
            cont++;
        }
    }

    if(cont >= 3) return true;

    //FIRMA ALEX POR SI ACASO
    return false;
}



void getImagePixels(){
    vector<unsigned char> image1;
    int image1Width, image1Height;

    char a;


    bool success = load_image(image1, "", image1Width, image1Height);
    cout << "Height: " << image1Height;
    cout << "  Width: " << image1Width;
    cin >> a;

    success = load_image(image1, "imageSamples/image1.jpg", image1Width, image1Height);

    if (!success)
    {
        cout << "Error loading image 1 \n";
        return;
    }

    const size_t RGBA = 4;


    cout << "lectura imagenes terminada" << endl;
    cin >> a;
    vector<int> pixeles;


    for (int i = 0; i < image1Width; i++) {
        for (int j = 0; j < image1Height; j++) {
            //cout << "x, y = " << i << "," <<j<< endl;
            int temporalIndex = RGBA * (j * image1Width + i);

            int r = (static_cast<int>(image1[temporalIndex + 0]));
            int g = (static_cast<int>(image1[temporalIndex + 1]));
            int b = (static_cast<int>(image1[temporalIndex + 2]));
            int rgb = ((r&0x0ff)<<16)|((g&0x0ff)<<8)|(b&0x0ff);
            pixeles.push_back(rgb);

        }
    }
    cout << "leyo todos los pixeles! :D  son: " << pixeles.size() << endl;
    cout << "los puntos claves son:" << endl;
    cin >> a;
}


int main(){
    /*std::unordered_map<int,int> mymap;

    std::cout << "max_size = " << mymap.max_size() << std::endl;
    int rgb = ((255&0x0ff)<<16)|((255&0x0ff)<<8)|(255&0x0ff);
    cout << rgb<< endl;
    int redondo = ceil(rgb/0.99999999);
    cout << redondo;*/
    cout << "Calculating... " << endl;
    auto started = chrono::high_resolution_clock::now();
    //Zeus();
    //getImagePixels("image1.jpg", "image2.jpg", 192);
    //compare();

    //sections(25,100,2,5);
    /*sectionWidth = 192;
    getImagesPixels("imageSamples/image1_original.jpg");
    pointsImg2("imageSamples/image4.jpg");*/

    sectionWidth = 40;
    sectionHeight = 22;
    getImagesPixels("imageSamples/miniatura1.png");
    /*pointsImg2("imageSamples/miniatura2.png");

    for(Pixel *pix: get<1>(image1ColorMap[5606466])){
        cout << "\ncolor: " <<pix->colorRGB << " at i.j  " << get<0>(pix->xy) << "." << get<1>(pix->xy) << endl;
        cout << compare(get<0>(image1ColorMap[5606466])[0],pix) << endl;
    }*/

    auto done = chrono::high_resolution_clock::now();

    cout << "Miliseconds: " << chrono::duration_cast<chrono::milliseconds>(done - started).count();

    char a;
    cin >> a;

    return 0;
}