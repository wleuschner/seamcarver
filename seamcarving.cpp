#include "seamcarving.h"
#include <cmath>
#include <QDebug>
#include <cmath>

SeamCarving::SeamCarving(QImage& img):image(img)
{

}

QImage SeamCarving::getGX()
{
    return Gx;
}

QImage SeamCarving::getGY()
{
    return Gy;
}

int SeamCarving::getEnergy(int x, int y){
    int g1 = qRed(Gx.pixel(x,y));
    int g2 = qRed(Gy.pixel(x,y));

    return abs(g1-128)+abs(g2-128);
}

void SeamCarving::findSeamV(){
    calculateGradients();
    long long* M =(long long*) malloc(sizeof(long long)*image.width()*image.height());
    for (int y = 0; y < image.height(); y++){
        for(int x = 0; x < image.width(); x++){

            int i  = x+y*image.width();

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
                }else if(x == image.width()-1){ //depends on k
                    end = 1;
                }

                for(int i = start; i < end; i++){
                    int j = x+i+(y-1)*image.width();
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
    seam.resize(image.height());
    //find minimum in last row
    long long min = LONG_LONG_MAX;
    int y = image.height()-1;
    for(int x = 0; x < image.width(); x++){
        //qDebug()<<M[x+y*image.width()];
        if(M[x+y*image.width()] <= min){
            min = M[x+y*image.width()];
            seam[y] = x;
        }
        //qDebug() << seam[y];
    }

    //start backtrack
    for(int y = image.height()-2; y >= 0; y--){
        int x = seam[y+1];

        long long min = LONG_LONG_MAX;

        //get minimum of predeccesors
        int start = -1;
        int end = 2;
        if(x == 0){ //depends on k
            start = 0;
        }else if(x == image.width()-1){ //depends on k
            end = 1;
        }
        for(int i = start; i < end; i++){
            int j = x+i+y*image.width();
            if(M[j] < min){
                min = M[j];
                seam[y] = x + i;
            }
        }
    }
    saveEnergyDist(M);
    free(M);
}

void SeamCarving::saveEnergyDist(long long* M){
    //qDebug()<<"small of M" <<M[0];
    energyDist = QImage(image.width(),image.height(), QImage::Format_ARGB32);
    unsigned int* pixs = (unsigned int*) energyDist.bits();
    long long max = LONG_LONG_MIN;
    for (int i = 0; i < image.width()*image.height(); i++){
        if (M[i] > max)
            max = M[i];
    }
    //qDebug()<<"max of M: " << max;
    double e = 0;
    for (int i = 0; i < image.width()*image.height(); i++){
        e = (double)M[i]/(double)max;
        e *= 255;
        pixs[i] = qRgb(e,e,e);
    }

}

void SeamCarving::removeSeamV(){
    findSeamV();
    unsigned int* pixs = (unsigned int*) image.bits();
    for (int i = 0; i < seam.size(); i++){
        //qDebug()<<seam[i];
        //pixs[seam[i]+i*image.width()] = (unsigned int) qRgb(255,0,0);

        //move array to remove pixel
        for(int x=seam[i];x<image.width()-1;x++)
        {
            pixs[x+i*image.width()]=pixs[x+1+i*image.width()];
        }
        //qDebug()<<seam[i];
    }
    image = image.copy(0,0,image.width()-1,image.height());
}
