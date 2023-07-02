//
// Created by Runze on 01/07/2023.
//
#include "model.h"
#include "common.h"
#include <iostream>
using namespace std;

int main(){
    model m = model("..\\data\\Balls.obj");
    int n = m.getNumVertices();
    cout <<"Num: " <<n << endl;
    for(int i = 0; i < n; i++){
        vecf3 v = m.getVertices()[i];
        cout << v.x() << " " << v.y() << " " << v.z() << endl;
    }
    return 0;
}