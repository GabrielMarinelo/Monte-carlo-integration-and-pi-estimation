#include <iostream>
#include <cmath>
#include <random>
#include <getopt.h>
using namespace std;

random_device rd;
mt19937 gen(rd());

void pi_estimate(){
    
    uniform_real_distribution<> dis(0, 1.0);
    double ratio, circle_points = 0, square_points = 0;
    
    for(int j = 1; j <= 99999999; j++){
        square_points++;
        if(dis(gen) <= pow(1-pow(dis(gen),2),.5)) circle_points++;
    }
    ratio = circle_points/square_points;
    cout << "PI estimated: " << 4*ratio << endl;
   
}

double f(double x){
    return cos(x)*exp(-x/10);
}

void swap(double *a, double *b){
    double temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

double R_integrate(double a, double b){
    
    bool verify = false;
    if(a > b){ 
        swap(&a, &b);
        verify = true;
    }    
    
    double sum = 0, N = 99999999, delta_x = ((b-a)/N);

    for(double i = a; i <= b; i += delta_x){
        sum += f(i);
    }
    if (!verify) sum = -sum;
    return delta_x*sum;
}

double MC_integrate(double a, double b){
    
    double sum = 0, N = 99999999;
    uniform_real_distribution<> dis(a, b);

    for(double i = 1; i <= N; i++){
        sum += f(dis(gen));
    }  

    return -((b-a)/N)*sum;
}

double f_2var(double x, double y){
    return sin(x)*cos(y);
}

double MC_2var_integrate(double a, double b, double c, double d){
    
    double sum = 0, N = 99999999;
    uniform_real_distribution<> disx(a, b);
    uniform_real_distribution<> disy(c, d);
    
    for(double i = 1; i <= N; i++){
        sum += f_2var(disx(gen), disy(gen));
    }  

    return ((b-a)/N)*((d-c))*sum;
}

int main(int argc, char **argv){

    double sum = 0;
    int option_index;
    bool pi = false, i = false, two = false;
    double a, b, c, d;

    while(( option_index = getopt(argc, argv, "h2ipa:b:c:d:" )) != -1){
        switch (option_index){
            case 'h':
                cout << "-p to get a pi estimate through monte carlo method (circle area/square area)\n";
                cout << "-a to set the integral superior limit\n";
                cout << "-b to set the integral inferior limit\n";
                cout << "-c to set the y variable superior limit\n";
                cout << "-d to set the y variable inferior limit\n";
                cout << "-2 to make a double integral\n";
                cout << "-i to make a simple integral with MC method and Riemann sum\n";
                break;
            case '2':
                two = true;
                break;
            case 'p':
                pi = true;
                break;
            case 'a':
                a = stod(optarg);
                break;
            case 'b':
                b = stod(optarg);
                break;
            case 'c':
                c = stod(optarg);
                break;
            case 'd':
                d = stod(optarg);
                break;
            case 'i':
                i = true;
                break;    
            default :
                return 1;
        }
    }

    if(pi) pi_estimate();
    
    if(i){
        cout << "Riemann Integration: " << R_integrate(a,b) << endl;
        cout << "Monte Carlo Integration: " << MC_integrate(a,b) << endl;
    }

    if(two) cout << "Double Integration: " <<  MC_2var_integrate(a, b, c, d) << endl;
    
    return 0;
}
