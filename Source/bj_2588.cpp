#include <iostream>

using namespace std;

int main (void)
{
    int layer1 = 0;
    int layer2 = 0;
    int layer3;
    int layer4;
    int layer5;
    int layer6;
    int h, t, o;

    do 
    {
        cout << "세자리 자연수" << endl;
        cin >> layer1 >> layer2;
    } while (layer1 < 100 || layer2 < 100);
    h = layer2 / 100;
    t = layer2 % 100 / 10;
    o = layer2 % 10;

    layer3 = layer1 * o;
    layer4 = layer1 * t;
    layer5 = layer1 * h;
    layer6 = layer3 + (layer4 * 10) + (layer5 * 100);

    cout << layer3 << endl;
    cout << layer4 << endl;
    cout << layer5 << endl;
    cout << layer5*100 + layer4*10 + layer3 << endl;

    return 0;

}