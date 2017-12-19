#include <stdio.h>
#include <math.h>

int main() {
    const double pi = 3.14159;
    const double d_theta = 2.0*pi/60.0;
    const double r = 250./2.;

    for( int i = 0; i <= 60; i += 5  ) {

        double x2 = cos( i*d_theta ) * r;
        double y2 = sin( i*d_theta ) * r;

        printf( "<line id=\"%d\" x1=\"0\" y1=\"0\""
                " x2=\"%.2f\" y2=\"%.2f\" stroke-width=\".2\" stroke=\"black\"/>\n",
                    i, x2, y2 );
    }

}
