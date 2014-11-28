#include "seamcarving.h"
#include <cmath>
#include <QDebug>

SeamCarving::SeamCarving(QImage& img):image(img)
{
       //image = img;
       calculateGradients();
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
    int g1 = Gx.pixel(x,y);
    int g2 = Gy.pixel(x,y);

    return g1+g2;
}

void SeamCarving::findSeam(){
    int* M =(int*) malloc(sizeof(int)*image.width()*image.height());
    for (int y = 0; y < image.height(); y++){
        for(int x = 0; x < image.width(); x++){

            int i  = x+y*image.width();

            if(y == 0){
                //first row
                M[i] = getEnergy(x,y);
            } else {
                int min = INT_MAX;

                //get minimum of predeccesors
                int start = -1;
                int end = 2;
                if(x == 0){ //depends on k
                    start = 0;
                }else if(x == image.width()-1){ //depends on k
                    end = 1;
                }
                for(int i = start; i < end; i++){
                    int j = i+y*image.width();
                    if(M[j] < min){
                        min = M[j];
                    }
                }
                M[i] = getEnergy(x,y) + min;
            }

        }
    }

    //M is calculated backtrack best seam
    seam.resize(image.height());
    //find minimum in last row
    int min = INT_MAX;
    int y = image.height()-1;
    for(int x = 0; x < image.width(); x++){
        if(M[x+y*image.width()] < min){
            min = M[x+y*image.width()];
            seam[y] = x;
        }
    }

    //start backtrack
    for(int y = image.height()-2; y >= 0; y--){
        int x = seam[y+1];

        int min = INT_MAX;

        //get minimum of predeccesors
        int start = -1;
        int end = 2;
        if(x == 0){ //depends on k
            start = 0;
        }else if(x == image.width()-1){ //depends on k
            end = 1;
        }
        for(int i = start; i < end; i++){
            int j = i+y*image.width();
            if(M[j] < min){
                min = M[j];
                seam[y] = x + i;
            }
        }
    }
}

void SeamCarving::removeSeam(){
    findSeam();

    unsigned int* pixs = (unsigned int*) image.bits();
    for (int i = 0; i < seam.size(); i++){
        //image.setPixel(seam[i],i,qRgb(255,0,0));
        pixs[seam[i]+i*image.width()] = (unsigned int) qRgb(255,0,0);
        qDebug()<<seam[i];
    }
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
    Gx = conv(image,dx);
    Gy = conv(image,dy);
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
                color=0;
            }
            dst[x+y*image.width()]=qRgb(color,color,color);
        }
    }

    QImage conv_img((unsigned char*)dst,width,height,QImage::Format_ARGB32);
    return conv_img;
}
