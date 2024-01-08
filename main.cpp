//Kai Sundararaj
//May 5, 2021
//ICS4U
//Mrs. Cullum
//This program reads in the elevation data from a file called "Colorado.dat" and then makes a map representing the elevations.
//Then on the map the lowest immediate elevation change path is shown starting from every pixel on the left side of the map in red. The lowest elevation path change of all those paths is in green.
//Then on the map, the lowest immediate elevation path is shown starting from every pixel on the left side of the map in red. The lowest elevation change path of all those paths is in yellow.
//Finally a map comparing the two pops up.
//The red paths are all shaded to be brighter in dark areas and darker in light areas. If it were the other way around it's hard to see that they're even shaded
//The green and yellow paths are not shaded because then they become too hard to see, they are different colours so that they can be differentiated on the comparison map.

//Including necessary things
#include <iostream>
#include <fstream>
#include <apmatrix.h>
#include <vector>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

//Using standard namespace
using namespace std;

//declaring functions
void readdata(ifstream &inputFile, apmatrix <int> &elevation);
void findmin(const apmatrix <int> &elevation, int &minimum);
void findmax(const apmatrix <int> &elevation, int &maximum);
void drawmap(const apmatrix <int> &elevation, const int maximum, const int minimum, apmatrix <float> &colournum);
void findpath(apmatrix <int> elevation, int y, vector <int> &echantotal, const apmatrix <float> &colournum);
void bestpath(const apmatrix <int> &elevation, int bestlinenum, const apmatrix <float> &colournum);
void findlowpath(apmatrix <int> elevation, int y, vector <int> &echantotal, const apmatrix <float> &colournum);
void bestlowpath(const apmatrix <int> &elevation, const int bestlinenum,  const apmatrix <float> &colournum);

int main()
{

    //opening file
    ifstream inputFile("Colorado.dat");

    //declaring variables for the elevation, colour for drawing the map and shading, the minimum value, and the maximum value
    apmatrix <int> elevation(844, 480);
    apmatrix <float> colournum(844, 480);
    int minimum = 0;
    int maximum = 0;



    //Telling user which map is being opened
    cout <<"1:least immediate elevation change" << endl;



    //Reading in the data, finding the minimum value and the maximum value
    readdata(inputFile, elevation);
    findmin(elevation, minimum);
    findmax(elevation, maximum);

    //Initializing allegro
    al_init();
    al_init_primitives_addon();



    //Creating first map display
    ALLEGRO_DISPLAY *map1 = al_create_display(844, 480);


    //Creating first map
    drawmap(elevation, maximum, minimum, colournum);

    //Initializing variables to say which line to draw, to save the best line, to compare the lines, and to save the best line elevation change
    int y = 0;
    int linesave = 0;
    vector <int> echantotal(480);
    int bestpathnum = 100000;

    //Loop to draw all the paths in red
    while(y < 479){
        //Drawing the paths
        findpath(elevation, y, echantotal, colournum);

        //Keeping track of the best path
        if(echantotal[y] < bestpathnum){
            bestpathnum = echantotal[y];
            linesave = y;
        }

        y++;
    }

    //Telling the user which path is best and its elevation change
    cout << "Best path preferring least immediate elevation change starts at line: " << linesave << ", its elevation change is: " << bestpathnum << " units." << endl;

    //Drawing the best path in green
    bestpath(elevation, linesave, colournum);

    //Displaying and getting rid of map
    al_flip_display();
    al_rest(5);
    al_destroy_display(map1);




    //Telling the user it's on the second map now
    cout << "2:lowest immediate elevation change" << endl;

    //Initializing variables to say which line to draw, to save the best line, to compare the lines, and to save the best line elevation change for the lowest immediate elevation map
    int ylow = 0;
    vector <int> echantotallow(480);
    int bestpathlownum = 100000;
    int linesavelow = 0;

    //Creating second display
    ALLEGRO_DISPLAY *map2 = al_create_display(844, 480);

    //Creating second map
    drawmap(elevation, maximum, minimum, colournum);

    //Loop to find best path for lowest immediate elevation
    while(ylow < 479){
        //Drawing all the paths in red
        findlowpath(elevation, ylow, echantotallow, colournum);

        //Keeping track of best path. Which line it starts at and the total elevation change in that path
        if(echantotallow[ylow] < bestpathlownum){
            bestpathlownum = echantotallow[ylow];
            linesavelow = ylow;
        }

        ylow++;

    }
    //Drawing the best path of the second method in yellow
    bestlowpath(elevation, linesavelow, colournum);

    //Telling the user the results of the best path of the second method
    cout << "Best path preferring going to lowest immediate elevation starts at line: " << linesavelow << ", its elevation change is: " << bestpathlownum << " units." << endl;

    //Displaying the map and destroying it
    al_flip_display();
    al_rest(5);
    al_destroy_display(map2);

    //Telling the user that it's on the third map
    cout << "3:comparing the best path from both." << endl;

    //Creating the third map
    ALLEGRO_DISPLAY *map3 = al_create_display(844, 480);

    //Drawing the third map and the 2 best paths for comparison
    drawmap(elevation, maximum, minimum, colournum);
    bestlowpath(elevation, linesavelow, colournum);
    bestpath(elevation, linesave, colournum);

    //Telling the user which method found a better path
    if(bestpathnum < bestpathlownum){
        cout << "The path preferring the immediate least elevation change found the best path with: " << bestpathnum << " units whereas the path preferring the lowest immediate elevation found its lowest elevation change path to be: " << bestpathlownum << " units." << endl;
    }
    if(bestpathlownum < bestpathnum){
        cout << "The path preferring the lowest immediate elevation found the best path with: " << bestpathlownum << " units whereas the path preferring the lowest immediate elevation change found its lowest elevation change path to be: " << bestpathnum << " units." << endl;
    }

    //Displaying and destroying the third map
    al_flip_display();
    al_rest(5);
    al_destroy_display(map3);

}



//Function to read in the data
void readdata(ifstream &inputFile, apmatrix <int> &elevation){

    int x = 0;
    int y = 0;

    if (!inputFile.is_open()){
        cout << "error" << endl;
    }


    while(y < 480){
        x = 0;
        while(x < 844){
            inputFile >> elevation[x][y];

            x++;
        }
        y++;
    }
inputFile.close();


}

//Function to find the minimum
void findmin(const apmatrix <int> &elevation, int &minimum){

    int x = 0;
    int y = 0;

    minimum = elevation [0][0];

    while(y < 480){
        x = 0;
        while(x < 844){
            if(elevation[x][y] < minimum){
                minimum = elevation[x][y];
            }
            x++;
        }
        y++;
    }

}

//Function to find the maximum
void findmax(const apmatrix <int> &elevation, int &maximum){

    int x = 0;
    int y = 0;

    maximum = elevation [0][0];

    while(y < 480){
        x = 0;
        while(x < 844){
            if(elevation[x][y] > maximum){
                maximum = elevation[x][y];
            }
            x++;
        }
        y++;
    }


}

//Function to draw the maps
void drawmap(const apmatrix <int> &elevation, const int maximum, const int minimum, apmatrix <float> &colournum){

    int x = 0;
    int y = 0;
    float range;
    float interval;


    range = maximum - minimum;
    interval = range / 255;


    while(y < 480){
        x = 0;
        while(x < 844){
            //Finding number that represents elevation in brightness
            colournum[x][y] = ((elevation[x][y] - minimum) / interval);

            //Drawing it as mountain or forest depending on elevation
            if(colournum[x][y] < 125){
                al_draw_pixel(x, y, al_map_rgb(0, colournum[x][y], (colournum[x][y]/1.2)));
            }
            else if(colournum[x][y] > 125){
                al_draw_pixel(x, y, al_map_rgb(colournum[x][y], (colournum[x][y] * 3/4), (colournum[x][y]/2)));
            }
            x++;
        }
        y++;
    }

}

//Function to find the paths the first way and draw them in shaded red
void findpath(apmatrix <int> elevation, int y, vector <int> &echantotal, const apmatrix <float> &colournum){

    int x = 0;
    //Saving y before it gets changed
    int y1 = y;
    //Variable to see if that pixel has already been used if y = 479 or y = 0
    bool used = false;


    //Elevation change - 1, elevation change 0, elevation change + 1
    int echanm1 = 0;
    int echan0 = 0;
    int echan1 = 0;


    while(x < 840){

    //Drawing first pixel on left
    if(x == 0){
        al_draw_pixel(x, y, al_map_rgb(255 - colournum[x][y], 0, 0));
    }

        used = false;
        //Special case if at top, has to stay within map
        if(y == 0){
            used = true;
            //Comparing absolute values to find least elevation change. If the absolute value is smallest then we use that one
            echan0 = abs(elevation[x][y] - elevation[x+1][y]);
            echan1 = abs(elevation[x][y] - elevation[x+1][y+1]);

            if(echan0 < echan1){
                y = y;
                echantotal[y1] = echantotal[y1] + echan0;
            }
            else if(echan1 < echan0){
                y = y + 1;
                echantotal[y1] = echantotal[y1] + echan1;
            }
            else if(echan0 == echan1){
                y = y;
                echantotal[y1] = echantotal[y1] + echan0;
            } else {
                cout << "error" << endl;
            }
        }

        //Special case if at bottom, has to stay within map
        if(y == 479){
            used = true;
            echanm1 = abs(elevation[x][y] - elevation[x+1][y-1]);
            echan0 = abs(elevation[x][y] - elevation[x+1][y]);

            if(echanm1 < echan0){
                y = y - 1;
                echantotal[y1] = echantotal[y1] + echanm1;
            }
            else if(echan0 < echanm1){
                y = y;
                echantotal[y1] = echantotal[y1] + echan0;
            }
            else if(echanm1 == echan0){
                y = y - 1;
                echantotal[y1] = echantotal[y1] + echanm1;
            } else {
                cout << "error" << endl;
            }

        }

        //All other cases, checked that it hasn't hit top or bot or else would repeat
        if(y > 0 && y < 480 && used == false){
            echanm1 = abs(elevation[x][y] - elevation[x+1][y-1]);
            echan0 = abs(elevation[x][y] - elevation[x+1][y]);
            echan1 = abs(elevation[x][y] - elevation[x+1][y+1]);

            if(echanm1 < echan0 && echanm1 < echan1){
                y = y - 1;
                echantotal[y1] = echantotal[y1] + echanm1;
            }
            else if(echan0 < echanm1&& echan0 < echan1){
                y = y;
                echantotal[y1] = echantotal[y1] + echan0;
            }
            else if(echan1 < echan0 && echan1 < echanm1){
                y = y + 1;
                echantotal[y1] = echantotal[y1] + echan1;
            }
            else if(echanm1 == echan0 || echanm1 == echan1){
                y = y - 1;
                echantotal[y1] = echantotal[y1] + echanm1;
            }
            else if(echan0 == echan1){
                y = y;
                echantotal[y1] = echantotal[y1] + echan0;
            }
            else{
                cout << "error" << endl;
            }
        }

        x++;

        //Drawing the pixel shaded in red
        al_draw_pixel(x, y, al_map_rgb(255 - colournum[x][y],0 , 0));

    }

}

//Function to draw the best path using the first method (lowest immediate elevation) in green
void bestpath(const apmatrix <int> &elevation, int bestlinenum, const apmatrix <float> &colournum){
    int echanm1 = 0;
    int echan0 = 0;
    int echan1 = 0;
    bool used = false;

    int y = bestlinenum;
    int x = 0;


    while(x < 840){

        if(x == 0){
            al_draw_pixel(x, y, al_map_rgb(0, colournum[x][y], 0));
        }

        used = false;

        if(y == 0){
            used = true;
            echan0 = abs(elevation[x][y] - elevation[x+1][y]);
            echan1 = abs(elevation[x][y] - elevation[x+1][y+1]);

            if(echan0 < echan1){
                y = y;
            }
            else if(echan1 < echan0){
                y = y + 1;
            }
            else if(echan0 == echan1){
                y = y;
            } else {
                cout << "error" << endl;
            }
        }

        if(y == 479){
            used = true;
            echanm1 = abs(elevation[x][y] - elevation[x+1][y-1]);
            echan0 = abs(elevation[x][y] - elevation[x+1][y]);

            if(echanm1 < echan0){
                y = y - 1;
            }
            else if(echan0 < echanm1){
                y = y;
            }
            else if(echanm1 == echan0){
                y = y - 1;
            } else {
                cout << "error" << endl;
            }

        }
        if(y > 0 && y < 480 && used == false){
            echanm1 = abs(elevation[x][y] - elevation[x+1][y-1]);
            echan0 = abs(elevation[x][y] - elevation[x+1][y]);
            echan1 = abs(elevation[x][y] - elevation[x+1][y+1]);

            if(echanm1 < echan0 && echanm1 < echan1){
                y = y - 1;
            }
            else if(echan0 < echanm1&& echan0 < echan1){
                y = y;
            }
            else if(echan1 < echan0 && echan1 < echanm1){
                y = y + 1;
            }
            else if(echanm1 == echan0 || echanm1 == echan1){
                y = y - 1;
            }
            else if(echan0 == echan1){
                y = y;
            }
            else{
                cout << "error" << endl;
            }
        }

        x++;




        //Drawing the pixel bright green (each pixel
        al_draw_pixel(x, y, al_map_rgb(0, 255, 0));

    }

}

//Function to draw the paths using the second method (always go to immediate lowest elevation) in shaded red
void findlowpath(apmatrix <int> elevation, int y, vector <int> &echantotal, const apmatrix <float> &colournum){

    bool used = false;
    int x = 0;
    int y1 = y;


    int echanm1 = 0;
    int echan0 = 0;
    int echan1 = 0;


    while(x < 840){

        if(x == 0){
            al_draw_pixel(x, y, al_map_rgb(255, 0, 0));
        }

    used = false;
        if(y == 0){
            //Comparing values to see which one is the biggest (therefore lowest elevation) and then we use that one
            echan0 = elevation[x][y] - elevation[x+1][y];
            echan1 = elevation[x][y] - elevation[x+1][y+1];
            used = true;

            if(echan0 > echan1){
                y = y;
                echantotal[y1] = echantotal[y1] + abs(echan0);
            }
            else if(echan1 > echan0){
                y = y + 1;
                echantotal[y1] = echantotal[y1] + abs(echan1);
            }
            else if(echan0 == echan1){
                y = y;
                echantotal[y1] = echantotal[y1] + abs(echan0);
            } else {
                cout << "error" << endl;
            }
        }

        if(y == 479){
            echanm1 = elevation[x][y] - elevation[x+1][y-1];
            echan0 = elevation[x][y] - elevation[x+1][y];
            used = true;

            if(echanm1 > echan0){
                y = y - 1;
                echantotal[y1] = echantotal[y1] + abs(echanm1);
            }
            else if(echan0 > echanm1){
                y = y;
                echantotal[y1] = echantotal[y1] + abs(echan0);
            }
            else if(echanm1 == echan0){
                y = y - 1;
                echantotal[y1] = echantotal[y1] + abs(echanm1);
            } else {
                cout << "error" << endl;
            }

        }
        if(y > 0 && y < 480 && used == false){
            echanm1 = elevation[x][y] - elevation[x+1][y-1];
            echan0 = elevation[x][y] - elevation[x+1][y];
            echan1 = elevation[x][y] - elevation[x+1][y+1];

            if(echanm1 > echan0 && echanm1 > echan1){
                y = y - 1;
                echantotal[y1] = echantotal[y1] + abs(echanm1);
            }
            else if(echan0 > echanm1&& echan0 > echan1){
                y = y;
                echantotal[y1] = echantotal[y1] + abs(echan0);
            }
            else if(echan1 > echan0 && echan1 > echanm1){
                y = y + 1;
                echantotal[y1] = echantotal[y1] + abs(echan1);
            }
            else if(echanm1 == echan0 || echanm1 == echan1){
                y = y - 1;
                echantotal[y1] = echantotal[y1] + abs(echanm1);
            }
            else if(echan0 == echan1){
                y = y;
                echantotal[y1] = echantotal[y1] + abs(echan0);
            }
            else{
                cout << "error" << endl;
            }
        }

        x++;

        //Shading drawing in red (each pixel)
        al_draw_pixel(x, y, al_map_rgb(255 - colournum[x][y],0 , 0));

    }

}

//Function to draw best path using second method (lowest immediate elevation) in yellow
void bestlowpath(const apmatrix <int> &elevation, const int bestlinenum, const apmatrix <float> &colournum){
    int echanm1 = 0;
    int echan0 = 0;
    int echan1 = 0;
    bool used = false;

    int y = bestlinenum;
    int x = 0;


    while(x < 840){

        if(x == 0){
            al_draw_pixel(x, y, al_map_rgb(0, 255, 0));
        }
        used = false;
        if(y == 0){
            used = true;
            echan0 = elevation[x][y] - elevation[x+1][y];
            echan1 = elevation[x][y] - elevation[x+1][y+1];

            if(echan0 > echan1){
                y = y;
            }
            else if(echan1 > echan0){
                y = y + 1;
            }
            else if(echan0 == echan1){
                y = y;
            } else {
                cout << "error" << endl;
            }
        }

        if(y == 479){
            used = true;
            echanm1 = elevation[x][y] - elevation[x+1][y-1];
            echan0 = elevation[x][y] - elevation[x+1][y];

            if(echanm1 > echan0){
                y = y - 1;
            }
            else if(echan0 > echanm1){
                y = y;
            }
            else if(echanm1 == echan0){
                y = y - 1;
            } else {
                cout << "error" << endl;
            }

        }
        if(y > 0 && y < 480 && used == false){
            echanm1 = elevation[x][y] - elevation[x+1][y-1];
            echan0 = elevation[x][y] - elevation[x+1][y];
            echan1 = elevation[x][y] - elevation[x+1][y+1];

            if(echanm1 > echan0 && echanm1 > echan1){
                y = y - 1;
            }
            else if(echan0 > echanm1&& echan0 > echan1){
                y = y;
            }
            else if(echan1 > echan0 && echan1 > echanm1){
                y = y + 1;
            }
            else if(echanm1 == echan0 || echanm1 == echan1){
                y = y - 1;
            }
            else if(echan0 == echan1){
                y = y;
            }
            else{
                cout << "error" << endl;
            }
        }

        x++;


        //Drawing the path (each pixel) in bright yellow
        al_draw_pixel(x, y, al_map_rgb(255, 255 , 0));


    }
}
