#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <ctime>

using namespace std;

// 날짜 문자열을 tm 구조체로 변환하는 함수
tm stringToDate(const string& dateStr) {
    tm date = {};
    istringstream ss(dateStr);
    ss >> get_time(&date, "%Y-%m-%d");
    return date;
}

// 두 tm 구조체의 날짜를 비교하는 함수
bool isEarlierThan(const tm& d1, const tm& d2) {
    return mktime(const_cast<tm*>(&d1)) < mktime(const_cast<tm*>(&d2));
}

int main() {
    int n;
    cin >> n; // 첫 번째 줄에서 정수 n을 읽어옵니다.
    cin.ignore(); // 개행 문자 제거

    string minDateStr;
    string minDayOfWeek;
    string minWeather;
    tm minDate = {};
    bool firstRainFound = false;

    for (int i = 0; i < n; ++i) {
        string date, dayOfWeek, weather;
        getline(cin, date, ' ');
        getline(cin, dayOfWeek, ' ');
        getline(cin, weather);

        if (weather == "Rain") {
            tm currentDate = stringToDate(date);
            
            if (!firstRainFound || isEarlierThan(currentDate, minDate)) {
                minDate = currentDate;
                minDateStr = date;
                minDayOfWeek = dayOfWeek;
                minWeather = weather;
                firstRainFound = true;
            }
        }
    }

    // 결과 출력
    if (firstRainFound) {
        cout << minDateStr << " " << minDayOfWeek << " " << minWeather << endl;
    } else {
        cout << "No rain data found" << endl;
    }

    return 0;
}