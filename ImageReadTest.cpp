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


struct Pixel{
    // Anterior
    // Siguiente
    // Color
    // x,y
};

struct Diagonal{
    Pixel *First;
    Pixel *Last;
    int Size;
};

vector<Diagonal*> diagonalesImg1;
vector<Diagonal*> diagonalesImg2;

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

void createDiagons(int cuantity, vector<Diagonal*> diagonaList){
    for (int diagonal = 0; diagonal <= cuantity; diagonal++){
        Diagonal *newDiagonal = new Diagonal();
        newDiagonal->First = nullptr;
        newDiagonal->Last = nullptr;
        newDiagonal->Size = 0;
        diagonaList.push_back(newDiagonal);
    }
}


void getImagesPixels(const string& image1Filename, const string& image2Filename, int sectionWidth) {


    int image1Width, image1Height, image2Width, image2Height;
    vector<unsigned char> image1;
    vector<unsigned char> image2;

    char a;


    bool success = load_image(image1, image1Filename, image1Width, image1Height);
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

    const size_t RGBA = 4;


    // For each row, get a pixel  every subImageWidth colums
    // Each time we go to the next row, start in different column, the next one to the right
    // so it does a stare running.


    // key: RGB, value: counter of apparitions
    unordered_map<int, int> image1ColorMap;
    unordered_map<string, int> image2ColorMap;

    // string 1: RGB String, String 2: x,y String
    vector<tuple<string,string>> image1RgbValues;
    vector<tuple<string,string>> image2RgbValues;


    cout << "lectura imagenes terminada" << endl;
    cin >> a;

    createDiagons(ceil(image1Width/sectionWidth),diagonalesImg1);

    int fixedJ = 0;
    for (int i = 0; i < image1Width; i++, fixedJ++) {
        if (fixedJ >= sectionWidth) {
            fixedJ = 0;

            diagonalesImg1.insert(diagonalesImg1.begin(), new Diagonal());

        }
        for (int j = fixedJ; j < image1Height; j += sectionWidth) {
            //cout << "x, y = " << i << "," <<j<< endl;
            int temporalIndex = RGBA * (j * image1Width + i);

            int r = (static_cast<int>(image1[temporalIndex + 0]));
            int g = (static_cast<int>(image1[temporalIndex + 1]));
            int b = (static_cast<int>(image1[temporalIndex + 2]));
            int rgb = ((r&0x0ff)<<16)|((g&0x0ff)<<8)|(b&0x0ff);

            Pixel *newPix = new Pixel();
            //newPix;


            // Rgb value in the form of "255-255-255", can be changed to a different representation, for now it will be a string.
            string rgbValueImage1 = to_string(static_cast<int>(image1[temporalIndex + 0]))
                                      + "-" + to_string(static_cast<int>(image1[temporalIndex + 1]))
                                    + "-" + to_string(static_cast<int>(image1[temporalIndex + 2]));

            string rgbValueImage2 = to_string(static_cast<int>(image2[temporalIndex + 0]))
                                    + "-" + to_string(static_cast<int>(image2[temporalIndex + 1]))
                                    + "-" + to_string(static_cast<int>(image2[temporalIndex + 2]));

            string xy = to_string(i)+","+to_string(j);
            image1RgbValues.emplace_back(rgbValueImage1,xy);
            image2RgbValues.emplace_back(rgbValueImage1,xy);
        }

    }
    cout << "vectores imagenes terminado, largo de cada vector=" << image1RgbValues.size() << endl;
    cin >>a;

}


int main(){
    std::unordered_map<int,int> mymap;

    std::cout << "max_size = " << mymap.max_size() << std::endl;
    int rgb = ((255&0x0ff)<<16)|((255&0x0ff)<<8)|(255&0x0ff);
    cout << rgb<< endl;
    int redondo = ceil(rgb/0.99999999);
    cout << redondo;
    /*
    cout << "Calculating... " << endl;
    auto started = chrono::high_resolution_clock::now();
    //Zeus();
    *//*getImagePixels("image1.jpg", "image2.jpg", 192);
    compare();*//*

    //sections(25,100,2,5);
    getImagesPixels("imageSamples/image1.jpg", "imageSamples/image2.jpg", 192);

    auto done = chrono::high_resolution_clock::now();

    cout << "Miliseconds: " << chrono::duration_cast<chrono::milliseconds>(done - started).count();

    char a;
    cin >> a;

    return 0;*/
}