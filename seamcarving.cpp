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

void SeamCarving::calculateGradients(){
    double dx[] =  {-1, 0,1,
                    -1, 0,1,
                    -1, 0,1};

    double dy[] =  {-1,-1,-1,
                     0, 0, 0,
                     1, 1, 1};

    //Gx = QImage(image.width(),image.height(),QImage::Format_ARGB32);
    //Gy = QImage(image.width(),image.height(),QImage::Format_ARGB32);
    Gx = conv(greyTones(image),dx);
    Gy = conv(greyTones(image),dy);
}

QImage SeamCarving::greyTones(const QImage& image)
{
    unsigned int* dst=(unsigned int*)malloc(sizeof(unsigned int)*image.width()*image.height());
    const unsigned int* src=(unsigned int*)image.bits();
    for(int y=0;y<image.height();y++)
    {
        for(int x=0;x<image.width();x++)
        {
            unsigned int c=src[x+y*image.width()];
            unsigned int r=qRed(c);
            unsigned int g=qGreen(c);
            unsigned int b=qBlue(c);
            unsigned int gs=(0.299f*r+0.587f*g+0.114f*b);
            dst[x+y*image.width()]=qRgba(gs,gs,gs,255);
        }
    }
    QImage grey_image((unsigned char*)dst,image.width(),image.height(),QImage::Format_ARGB32);
    return grey_image;
}

QImage SeamCarving::conv(const QImage& image,double* mat)
{
    unsigned int* src = (unsigned int*)image.bits();
    unsigned int* dst = (unsigned int*)malloc(sizeof(unsigned int)*image.width()*image.height());
    int width = image.width();
    int height = image.height();
    for(int y=0;y<image.height();y++)
    {
        for(int x=0;x<image.width();x++)
        {
            int color=0;
            //Convolute repair 180 degree
            for(int cy=-1;cy<=1;cy++)
            {
                for(int cx=-1;cx<=1;cx++)
                {
                    int c=0;
                    if((cx+x>=0 && cy+y>=0) && (cx+x<width && cy+y<height))
                    {
                        c = round(qRed(src[(x+cx)+(y+cy)*image.width()])*mat[(2-(cx+1))+((2-(cy+1))*3)]);
                    }
                    else if(cx+x<0)
                    {
                        if(cy+y<0)
                        {
                            c = round(qRed(src[(0)+(0)*image.width()])*mat[(2-(cx+1))+((2-(cy+1))*3)]);
                        }
                        else if(cy+y>=height)
                        {
                            c = round(qRed(src[(0)+(height-1)*image.width()])*mat[(2-(cx+1))+((2-(cy+1))*3)]);
                        }
                        else
                        {
                            c = round(qRed(src[(0)+(y+cy)*image.width()])*mat[(2-(cx+1))+((2-(cy+1))*3)]);
                        }
                    }
                    else if(cx+x>=width)
                    {
                        if(cy+y<0)
                        {
                            c = round(qRed(src[(width-1)+(0)*image.width()])*mat[(2-(cx+1))+((2-(cy+1))*3)]);
                        }
                        else if(cy+y>=height)
                        {
                            c = round(qRed(src[(width-1)+(height-1)*image.width()])*mat[(2-(cx+1))+((2-(cy+1))*3)]);
                        }
                        else
                        {
                            c = round(qRed(src[(width-1)+(y+cy)*image.width()])*mat[(2-(cx+1))+((2-(cy+1))*3)]);
                        }

                    }
                    else if(cy+y<0)
                    {
                        if(cx+x<0)
                        {
                            c = round(qRed(src[(0)+(0)*image.width()])*mat[(2-(cx+1))+((2-(cy+1))*3)]);
                        }
                        else if(cx+x>=width)
                        {
                            c = round(qRed(src[(width-1)+(0)*image.width()])*mat[(2-(cx+1))+((2-(cy+1))*3)]);
                        }
                        else
                        {
                            c = round(qRed(src[(cx+x)+(0)*image.width()])*mat[(2-(cx+1))+((2-(cy+1))*3)]);
                        }
                    }
                    else if(cy+y>height)
                    {
                        if(cx+x<0)
                        {
                            c = round(qRed(src[(0)+(height-1)*image.width()])*mat[(2-(cx+1))+((2-(cy+1))*3)]);
                        }
                        else if(cx+x>=width)
                        {
                            c = round(qRed(src[(width-1)+(height-1)*image.width()])*mat[(2-(cx+1))+((2-(cy+1))*3)]);
                        }
                        else
                        {
                            c = round(qRed(src[(width)+(height-1)*image.width()])*mat[(2-(cx+1))+((2-(cy+1))*3)]);
                        }
                    }
                    color+=c;
                }
            }
            color+=128;
            if(color>255)
            {
                color=255;
            }
            else if(color<0)
            {
                color = 0;
            }
            dst[x+y*image.width()]=qRgb(color,color,color);
        }
    }

    QImage conv_img((unsigned char*)dst,width,height,QImage::Format_ARGB32);
    return conv_img;
}
