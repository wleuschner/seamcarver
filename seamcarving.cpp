#include "seamcarving.h"
#include <cmath>

SeamCarving::SeamCarving(QImage img)
{
       image = img;
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

void SeamCarving::calculateGradients(){
    double dx[] =  {-1, 0,1,
                    -1, 0,1,
                    -1, 0,1};

    double dy[] =  {-1,-1,-1,
                     0, 0, 0,
                     1, 1, 1};

    //Gx = QImage(image.width(),image.height(),QImage::Format_ARGB32);
    //Gy = QImage(image.width(),image.height(),QImage::Format_ARGB32);
    unsigned int *pixs = (unsigned int*) image.bits();
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
