#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    cout << "=== Vector Test ===" << endl;

    vector<int> numbers;
    int count;

    cout << "숫자 개수 입력: ";
    cin >> count;

    cout << count << "개의 숫자를 입력하세요:" << endl;
    for (int i = 0; i < count; i++) {
        int num;
        cin >> num;
        numbers.push_back(num);
    }

    // 정렬
    sort(numbers.begin(), numbers.end());

    cout << "\n정렬된 결과: ";
    for (int num : numbers) {
        cout << num << " ";
    }
    cout << endl;

    // 합계와 평균
    int sum = 0;
    for (int num : numbers) {
        sum += num;
    }

    cout << "합계: " << sum << endl;
    cout << "평균: " << (double)sum / count << endl;

    cout << "\n계속하려면 아무 키나 누르세요...";
    cin.ignore();
    cin.get();

    return 0;
}
