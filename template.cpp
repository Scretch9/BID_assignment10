#include<stdio.h>
#include <math.h>
#include <vector>

#define PI acos(-1.0)
#define LOOK_DST 50
#define KERNAL_BANDWITH 100


//YOUR CODE
double euclid_distance(int x, int y){
    return sqrt((x-y)*(x-y));
}

double gaussian_kernel(double distance, double bandwidth){
    return (1/(bandwidth*sqrt(2*PI)))*exp(-0.5*((distance/bandwidth)*(distance/bandwidth)));
}

std::vector<int> neighbourhood_points(int X[], int x_centeroid, int x_len, double distance){
    std::vector<int> neighbours;

    for (int i =0; i < x_len; i++){
        double dst = euclid_distance(X[i], x_centeroid);
        if(dst <= distance)neighbours.push_back(X[i]);
    }
    return neighbours;
}

int* initializeKMeans(int data[], int data_len, int k){

    const int size = 3;
    int min,max;
    for (size_t i = 0; i < data_len; i++)
    {
        if(!min || data[i] < min)min = data[i];
        if(!max || data[i] > max)max = data[i];
    }

    int means[size];
    for (size_t i = 0; i < k; i++)
    {
        means[i] = min + rand() % max;
    }

    return means;    
}

int updateMean(int n, int mean, int neighbour){
        return (mean*(n-1)+neighbour)/float(n);    
}


int classify(int means[], int k, int neighbour){
    int min;
    int index = -1;

    for (size_t i = 0; i < k; i++)
    {
        int distance = euclid_distance(neighbour, means[i]);

        if(distance < min || !min){
            min = distance;dygbh
            index = i;
        }
    }

    return index;
    
}

//input:
// int[] data : array of data including previous data so far, with length of num_prev_data.
// num_prev_data : number of data above
int predict(int* data, int num_prev_data){
    //your new data should be data[num_prev_data-1];
    int value = data[num_prev_data-1];

    //NEW///////////////////////////////////////////////////////////
    int* means = initializeKMeans(data, num_prev_data,3);

    std::vector<int> clusterSizes[3];
    int belongsTo[num_prev_data];

    //NEW///////////////////////////////////////////////////////////

    //output: 0 for black, 1 for gray and 2 for white.
    if(value > 300)return 2;
    if(value > 200)return 1;
    else return 0;
}



//////////////////////////////////////////////////////////
//////////////////DO NOT MODIFY BELOW/////////////////////
//////////////////////////////////////////////////////////
int main(void){
    float average_score = 0;
    for(int f=1; f <= 4; f++){
        char filename[10];
        sprintf(filename, "data%d.txt", f);
        FILE* fp = fopen(filename, "r");
        
        int N;
        fscanf(fp, "%d", &N);
        int data[N];
        int score = 0;
        for(int i=0; i < N; i++){
            int d, label;
            fscanf(fp, "%d %d", &d, &label);
            data[i] = d;
            int pred = predict(data, i+1);
            if(pred == label)score++;
        }
        printf("round %d: your score: %f\n",f, score / float(N));
        average_score += score/float(N);
        //score the prediction
        fclose(fp);
    }
    average_score /= 4;
    printf("average score: %f\n", average_score);
}