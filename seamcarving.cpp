#include "seamcarving.h"
#include <cmath>
#include <QDebug>
#include <cmath>
#include <ctime>

SeamCarving::SeamCarving(QImage& img, EnergyFunctionI* en):image(img)
{
    width=img.width();
    height=img.height();
    energy = en;
}



int SeamCarving::getEnergy(int x, int y){
   return energy->calculateEnergy(x,y);
}
void SeamCarving::findSeamH(){
    long long* M =(long long*) malloc(sizeof(long long)*width*height);
    for(int x = 0; x < width; x++){
        for (int y = 0; y < height; y++){
            int i  = x+y*width;

            if(x == 0){
                //first column
                M[i] = getEnergy(x,y);
            } else {
                long long min = LONG_LONG_MAX;

                //get minimum of predeccesors
                int start = -1;
                int end = 2;

                if(y == 0){ //depends on k
                    start = 0;
                }else if(y == height-1){ //depends on k
                    end = 1;
                }

                for(int i = start; i < end; i++){
                    int j = (x-1)+(y+i)*width;
                    if(M[j] < min){
                        min = M[j];
                    }
                }
                //qDebug()<<"min: "<< min;
                M[i] = getEnergy(x,y)+min;
            }

        }
    }

    //M is calculated backtrack best seam
    seam.resize(width);
    //find minimum in last column
    long long min = LONG_LONG_MAX;
    int x = width-1;
    for(int y = 0; y < height; y++){
        //qDebug()<<M[x+y*image.width()];
        if(M[x+y*width] <= min){
            min = M[x+y*width];
            seam[x] = y;
        }
        //qDebug() << seam[y];
    }

    //start backtrack
    for(int x = width-2; x >= 0; x--){
        int y = seam[x+1];

        long long min = LONG_LONG_MAX;

        //get minimum of predeccesors
        int start = -1;
        int end = 2;
        if(y == 0){ //depends on k
            start = 0;
        }else if(y == width-1){ //depends on k
            end = 1;
        }
        for(int i = start; i < end; i++){
            int j = x+(y+i)*width;
            if(M[j] < min){
                min = M[j];
                seam[x] = y + i;
            }
        }
    }
    saveEnergyDist(M);
    energy->updateH(seam);
    free(M);
}
void SeamCarving::findSeamV(){
    long long* M =(long long*) malloc(sizeof(long long)*width*height);
    for (int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){

            int i  = x+y*width;

            if(y == 0){
                //first row
                M[i] = getEnergy(x,y);
            } else {
                long long min = LONG_LONG_MAX;

                //get minimum of predeccesors
                int start = -1;
                int end = 2;

                if(x == 0){ //depends on k
                    start = 0;
                }else if(x == width-1){ //depends on k
                    end = 1;
                }

                for(int i = start; i < end; i++){
                    int j = x+i+(y-1)*width;
                    if(M[j] < min){
                        min = M[j];
                    }
                }
                //qDebug()<<"min: "<< min;
                M[i] = getEnergy(x,y)+min;
            }

        }
    }

    //M is calculated backtrack best seam
    seam.resize(height);
    //find minimum in last row
    long long min = LONG_LONG_MAX;
    int y = height-1;
    for(int x = 0; x < width; x++){
        //qDebug()<<M[x+y*image.width()];
        if(M[x+y*width] <= min){
            min = M[x+y*width];
            seam[y] = x;
        }
        //qDebug() << seam[y];
    }

    //start backtrack
    for(int y = height-2; y >= 0; y--){
        int x = seam[y+1];

        long long min = LONG_LONG_MAX;

        //get minimum of predeccesors
        int start = -1;
        int end = 2;
        if(x == 0){ //depends on k
            start = 0;
        }else if(x == width-1){ //depends on k
            end = 1;
        }
        for(int i = start; i < end; i++){
            int j = x+i+y*width;
            if(M[j] < min){
                min = M[j];
                seam[y] = x + i;
            }
        }
    }
    saveEnergyDist(M);
    energy->updateV(seam);
    free(M);
}

void SeamCarving::saveEnergyDist(long long* M){
    //qDebug()<<"small of M" <<M[0];
    energyDist = QImage(width,height, QImage::Format_ARGB32);
    unsigned int* pixs = (unsigned int*)const_cast<unsigned char*>(energyDist.bits());
    long long max = LONG_LONG_MIN;
    for (int i = 0; i < width*height; i++){
        if (M[i] > max)
            max = M[i];
    }
    //qDebug()<<"max of M: " << max;
    double e = 0;
    for (int i = 0; i < width*height; i++){
        e = (double)M[i]/(double)max;
        e *= 255;
        pixs[i] = qRgb(e,e,e);
    }

}

void SeamCarving::removeSeamH(){
    findSeamH();
    unsigned int* pixs = (unsigned int*)const_cast<unsigned char*>(image.bits());
    for (int i = 0; i < seam.size(); i++){
        //move array to remove pixel
        //pixs[i+seam[i]*image.width()] = (unsigned int) qRgb(255,0,0);

        for(int y=seam[i];y<height-1;y++)
        {
            pixs[i+y*image.width()]=pixs[i+(y+1)*image.width()];
        }
    }
    height--;
}

void SeamCarving::removeSeamV(){
    findSeamV();
    unsigned int* pixs = (unsigned int*)const_cast<unsigned char*>(image.bits());
    for (int i = 0; i < seam.size(); i++){
        //qDebug()<<seam[i];
        //pixs[seam[i]+i*image.width()] = (unsigned int) qRgb(255,0,0);

        //move array to remove pixel
        for(int x=seam[i];x<width-1;x++)
        {
            pixs[x+i*image.width()]=pixs[x+1+i*image.width()];
        }
        //qDebug()<<seam[i];
    }
    //qDebug()<<"Width: "<<width;
    width--;
    //image = image.copy(0,0,image.width()-1,image.height());
}

QImage SeamCarving::getImage()
{
    //return image;
    return image.copy(0,0,width,height);
}
