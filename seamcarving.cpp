#include "seamcarving.h"

SeamCarving::SeamCarving(QImage img)
{
       image = img;
       calculateGradients();
}

void SeamCarving::calculateGradients(){
    Gx = QImage(image.width(),image.height(),QImage::Format_Mono);
    Gy = QImage(image.width(),image.height(),QImage::Format_Mono);
    unsigned int *pixs = (unsigned int*) image.bits();
    /*
    for (int y = 0; y < image.height(); y++){
        for (int x = 0; x < image.width(); x++){

            int i = y*image.width()+x;

            int r = qRed(pixs[i]);
            int g = qGreen(pixs[i]);
            int b = qBlue(pixs[i]);

            int l =

        }
    }
    */
}
