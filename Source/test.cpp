#include <iostream>
#include <string>
using namespace std;

int main() {
    string name;
    int age;

    cout << "이름을 입력하세요: ";
    cin >> name;

    cout << "나이를 입력하세요!!!: ";
    cin >> age;


    cout << "\n=== 입력 결과 ===" << endl;
    cout << "안녕하세요, " << name << "님!" << endl;
    cout << "당신은 " << age << "살이군요." << endl;
    cout << "10년 후에는 " << (age + 10) << "살이 되시겠네요!" << endl;

    cout << "\n종료하려면 아무 키나 누르세요...";
    cin.ignore();
    cin.get();

    return 0;
}
