#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main() {
    cout << "=== C++ Test Program ===" << endl;

    // 변수 테스트
    int num = 42;
    double pi = 3.14159;
    string message = "Hello, VSCode!";

    cout << "Number: " << num << endl;
    cout << "Pi: " << pi << endl;
    cout << "Message: " << message << endl;

    // 벡터 테스트
    vector<int> numbers = {1, 2, 3, 4, 5};
    cout << "\nVector elements: ";
    for (int n : numbers) {
        cout << n << " ";
    }
    cout << endl;

    // 간단한 계산
    int sum = 0;
    for (int n : numbers) {
        sum += n;
    }
    cout << "Sum: " << sum << endl;

    cout << "\nProgram finished successfully!" << endl;

    return 0;
}
