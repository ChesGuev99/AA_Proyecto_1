#include <iostream>
#include <vector>
#include <tuple>
#include <unordered_map>
#include <string>
#include <chrono>
#include <algorithm>
#include <math.h>
#include <bits/stdc++.h>

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

    Pixel(vector<Pixel*> diagonal, int color, int x, int y){
        this->colorRGB = color;
        this->posDiagonal = diagonal.size();
        this->diagonal = &diagonal;
        this->diagonal->push_back(this);
        this->xy = make_tuple(x,y);
    }
};

/*struct Diagonal{
    Pixel *First;
    Pixel *Last;
    int Size;
};*/

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

void createDiagons(int cuantity, vector<vector<Pixel*>> *diagonaList){
    for (int diagonal = 0; diagonal <= cuantity; diagonal++){
        diagonalesImg1.push_back({});
    }
    cout << "creo diagonales";
}

vector<unsigned char> image2;
int image2Width, image2Height;

unordered_map<int, tuple<vector<Pixel*>,vector<Pixel*>>> image1ColorMap;

const size_t RGBA = 4;

char a;

void getImagesPixels(const string& image1Filename, const string& image2Filename, int sectionWidth) {

    vector<unsigned char> image1;
    int image1Width, image1Height;

    bool success = load_image(image1, "imageSamples/image1.jpg", image1Width, image1Height);
    cout << "Height: " << image1Height;
    cout << "  Width: " << image1Width;
    cin >> a;

    if (!success)
    {
        cout << "Error loading image 1 \n";
        return;
    }

    success = load_image(image2, image2Filename, image2Width, image2Height);

    if (!success)
    {
        cout << "Error loading image 2 \n";
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

    createDiagons(ceil(image1Width/sectionWidth),&diagonalesImg1);

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
/*            cout << "nuevo pixel generado en diagonal : " << diagonPosition << endl;
            cout << diagonalesImg1.size();*/
            if(i % (sectionWidth/2) == 0 ) {
                if (image1ColorMap.find(rgb) == image1ColorMap.end()){
                    tuple<vector<Pixel *>, vector<Pixel *>> newKey({}, {});
                    image1ColorMap[rgb] = newKey;
                }
                get<0>(image1ColorMap[rgb]).push_back(newPix);
                image1RgbValues.push_back(make_tuple((to_string(r)+ "-" +to_string(g)+ "-" +to_string(b)),(to_string(i)+","+to_string(j))));
                cout << get<0>(image1RgbValues.back()) << "color" << rgb << " at " << get<1>(image1RgbValues.back())<< endl;
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

    pointsImg2(image1);
}

bool cmp(pair<int, int>& a,
         pair<int, int>& b)
{
    return a.second < b.second;
}

unordered_map<int, int> colorAparitions;

void pointsImg2(vector<unsigned char> image1){
    vector<tuple<string,string>> image2RgbValues;
    //vector i,j Visitados
    for (int i = 0; i < image2Width; i++) {
        for (int j = 0; j < image2Height; j++) {

            int temporalIndex = RGBA * (j * image2Width + i);

            int r = (static_cast<int>(image1[temporalIndex + 0]));
            int g = (static_cast<int>(image1[temporalIndex + 1]));
            int b = (static_cast<int>(image1[temporalIndex + 2]));
            int rgb = ((r&0x0ff)<<16)|((g&0x0ff)<<8)|(b&0x0ff);

            if (image1ColorMap.find(rgb) != image1ColorMap.end()) {
                int diagonPos = diagonalesImg2.size();
                diagonalesImg2.push_back({});
                Pixel *newPix = new Pixel(diagonalesImg2[diagonPos],rgb,j, i);
                // TODO: getDiagonal(newPix)
                get<1>(image1ColorMap[rgb]).push_back(newPix);
                image2RgbValues.push_back(make_tuple((to_string(r)+ "-" +to_string(g)+ "-" +to_string(b)),(to_string(j)+","+to_string(i))));
                /*cout << "aparicion de color " << (to_string(r)+ "-" +to_string(g)+ "-" +to_string(b))
                                    << "at: " << (to_string(j)+","+to_string(i))<< endl;*/
                colorAparitions[rgb]++;
            }
        }
    }

    vector<pair<int, int>> elems(colorAparitions.begin(), colorAparitions.end());
    sort(elems.begin(), elems.end(), cmp);
    cout << "cantidad de elementos e vector2" << elems.size()<< endl;

    //sort(colorAparitions.begin(), colorAparitions.end(), cmp);
    cout << "encontrados en:" << endl;
    int color = elems[0].first;
    //get<1>(image1ColorMap[color]);
    for(auto pix : get<1>(image1ColorMap[color])){
        cout << "color: " <<color << " at x.y  " << get<0>(pix->xy) << "." << get<1>(pix->xy) << endl;
    }
    cout << "vectores imagenes terminado, largo de cada vector=" << image2RgbValues.size() << endl;
    cin >>a;

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
    getImagesPixels("imageSamples/image1.jpg","imageSamples/image2.jpg",192);

    auto done = chrono::high_resolution_clock::now();

    cout << "Miliseconds: " << chrono::duration_cast<chrono::milliseconds>(done - started).count();

    char a;
    cin >> a;

    return 0;
}