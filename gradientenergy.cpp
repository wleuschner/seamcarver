#include "gradientenergy.h"
#include <cmath>
#include <QDebug>

GradientEnergy::GradientEnergy(QImage& I):image(I)
{
    greyTones();
    calculateGradients();
    changes = 0;
    width = image.width();
    height = image.height();
}

GradientEnergy::~GradientEnergy(){
}

int GradientEnergy::calculateEnergy(int x, int y){
    int g1 = qRed(Gx.pixel(x,y));
    int g2 = qRed(Gy.pixel(x,y));

    return abs(g1-128)+abs(g2-128);
}

void GradientEnergy::updateH(std::vector<int> seam){
    unsigned int* pixs = (unsigned int*)const_cast<unsigned char*>(grey.bits());
    for (int i = 0; i < seam.size(); i++){
        //move array to remove pixel
        //TODO
        for(int y=seam[i];y<height-1;y++)
        {
            pixs[i+y*grey.width()]=pixs[i+(y+1)*grey.width()];
        }
    }
    if(changes >= 2){
        //std::clock_t t = std::clock();
        calculateGradients();
        //t = std::clock() - t;
        //qDebug()<<"TIME: "<<((float)t)/CLOCKS_PER_SEC;
        changes = 0;
    }
    changes++;
    height--;
}

void GradientEnergy::updateV(std::vector<int> seam){
    unsigned int* pixs = (unsigned int*)const_cast<unsigned char*>(grey.bits());
    for (int i = 0; i < seam.size(); i++){
        //move array to remove pixel
        for(int x=seam[i];x<width-1;x++)
        {
            pixs[x+i*grey.width()]=pixs[x+1+i*grey.width()];
        }
    }
    if(changes >= 2){
        //std::clock_t t = std::clock();
        calculateGradients();
        //t = std::clock() - t;
        //qDebug()<<"TIME: "<<((float)t)/CLOCKS_PER_SEC;
        changes = 0;
    }
    changes++;
    width--;
}

QImage GradientEnergy::getGX(){
    return Gx;
}
QImage GradientEnergy::getGY(){
    return Gy;
}
QImage GradientEnergy::getEnergyPlot(){
    //QImage plot = QImage(image.width(),image.height(),QImage::Format_Mono);
    unsigned int* pixs = (unsigned int*) malloc(sizeof(unsigned int)*image.height()*image.width());
    float temp = 0;
    for (int y = 0; y < image.height(); y++){
        for (int x = 0; x < image.width(); x++){
            temp = calculateEnergy(x,y);
            pixs[y*image.width()+x] = qRgb(temp,temp,temp);
        }
    }
    return QImage((unsigned char*)pixs,width,height,QImage::Format_ARGB32);
}

void GradientEnergy::greyTones(){
    int width = image.width();
    int height = image.height();
    unsigned int* dst=(unsigned int*)malloc(sizeof(unsigned int)*width*height);
    const unsigned int* src=(const unsigned int*)image.bits();
    for(int y=0;y<height;y++)
    {
        for(int x=0;x<width;x++)
        {
            unsigned int c=src[x+y*image.width()];
            unsigned int r=qRed(c);
            unsigned int g=qGreen(c);
            unsigned int b=qBlue(c);
            unsigned int gs=(0.299f*r+0.587f*g+0.114f*b);
            dst[x+y*width]=qRgba(gs,gs,gs,255);
        }
    }
    grey = QImage((unsigned char*)dst,width,height,QImage::Format_ARGB32);
}

QImage GradientEnergy::conv(double* mat){
    int width = grey.width();
    int height = grey.height();
    const unsigned int* src = (const unsigned int*)grey.bits();
    unsigned int* dst = (unsigned int*)malloc(sizeof(unsigned int)*width*height);
    for(int y=0;y<height;y++)
    {
        for(int x=0;x<width;x++)
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
            dst[x+y*width]=qRgb(color,color,color);
        }
    }

    QImage conv_img((unsigned char*)dst,width,height,QImage::Format_ARGB32);
    return conv_img;
}
void GradientEnergy::calculateGradients(){
    double dx[] =  {0, 0,0,
                    -1, 0,1,
                    0, 0,0};

    double dy[] =  {0,-1,0,
                     0, 0, 0,
                     0, 1, 0};

    //Gx = QImage(image.width(),image.height(),QImage::Format_ARGB32);
    //Gy = QImage(image.width(),image.height(),QImage::Format_ARGB32);
    Gx = conv(dx);
    Gy = conv(dy);
}
