#include <iostream>
#include <vector>
#include <map>

std::vector<std::vector<std::pair<std::pair<int, int>, std::string> > > routes;
std::map<int, std::vector<std::pair<std::pair<int, int>, std::string> >* > timetable;

bool isLetter(char sign) {
    if ((int) (sign) >= (int) ('a') && (int) (sign) <= (int) ('z'))
        return true;
    else return ((int) (sign) >= (int) ('A') && (int) (sign) <= (int) ('Z'));
}

bool isNumber(char sign) {
    return ((int)(sign) >= (int)('0') && (int)(sign) <= (int)('9'));
}

std::pair<int, int> selectNumberOfRoute(std::string line, int start) {

    int number = (int)line[start] - (int)('0');
    int position = start + 1;

    while (isNumber(line[position])) {
        number *= 10;
        number += (int)(line[position]) - (int)('0');
        position++;
    }

    return std::make_pair(number, position);
}

bool areTramsWorking (int hour, int minute) {

    if (hour < 5 || hour > 21)
        return false;

    if (minute < 0 || minute > 59)
        return false;

    if (hour == 5 && minute < 55)
        return false;

    return !(hour == 21 && minute > 21);
}

std::pair<std::pair<int, int>, int> selectTime(std::string line, int start) {

    if (!isNumber(line[start]) || line[start] == '0')
        return std::make_pair(std::make_pair(-1,-1),-1);

    int hour, position;
    if (line[start + 1] == ':') {
        hour = (int) (line[start]) - (int) ('0');
        position = start + 1;
    }
    else if (isNumber(line[start + 1])) {
        hour = (int)(line[start]) - (int)('0');
        hour *= 10;
        hour += (int)(line[start + 1]) - (int)('0');
        position = start + 2;
    }
    else {
        return std::make_pair(std::make_pair(-1,-1),-1);
    }

    if (line[position] != ':')
        return std::make_pair(std::make_pair(-1,-1),-1);

    position++;
    if (!isNumber(line[position]) || !isNumber(line[position + 1]))
        return std::make_pair(std::make_pair(-1,-1),-1);

    int minute = (int)(line[position]) - (int)('0');
    minute *= 10;
    minute += (int)(line[position + 1]) - (int)('0');
    position += 2;

    if (!areTramsWorking(hour, minute))
        return std::make_pair(std::make_pair(-1,-1),-1);

    return std::make_pair(std::make_pair(hour, minute), position);
}

std::pair<std::string, int> selectBusStop(std::string line, int position) {

    int length = 0;
    while (isLetter(line[position + length]) || line[position + length] == '_'
           || line[position + length] == '^')
        length ++;

    std::string busStopName = line.substr((size_t)(position), (size_t)(length));
    position += length;

    return std::make_pair(busStopName, position);
}

bool biggerTime(int hour, int minute, int pHour, int pMinute) {

    if (pHour > hour)
        return false;

    return !(pHour == hour && pMinute >= minute);
}

void loadNewRoute(std::string line, int numberOfLine) {

    std::pair<int, int> result = selectNumberOfRoute(line, 0);
    int numberOfRoute = result.first;
    int position = result.second;

    std::cout << "route nr " << numberOfRoute << "\n";

    std::vector<std::pair<std::pair<int, int>, std::string> > route;
    int prevHour = 0, prevMinute = 0;

    while (position < line.size()) {

        if (line[position] != ' ') {
            std::cerr << numberOfLine << " " << line << "\n";
            return;
        }
        position ++;

        std::pair<std::pair<int, int>, int> time = selectTime(line, position);
        if (time.first.first == -1) {
            std::cerr << numberOfLine << " " << line << "\n";
            return;
        }

        int hour = time.first.first, minute = time.first.second;
        position = time.second;

        if (line[position] != ' ' ||
            !biggerTime(hour, minute, prevHour, prevMinute)) {
            std::cerr << numberOfLine << " " << line << "\n";
            return;
        }
        prevHour = hour;
        prevMinute = minute;
        position ++;

        std::pair<std::string, int> busStop = selectBusStop(line, position);
        std::string busStopName = busStop.first;
        position = busStop.second;

        route.push_back(std::make_pair(std::make_pair(hour, minute), busStopName) );
        std::cout << hour << ":" << minute << " " << busStopName << " " << route.size() <<"\n";
    }
    routes.push_back(route);
    timetable[numberOfRoute] = &route;
    std::cout << routes.size() << " " << timetable.size() << "\n";

}

void loadNewTicket(std::string line, int numberOfLine) {
    std::cout << numberOfLine << ": loadNewRoute: " << line << "\n";
}

void loadNewQuestion(std::string line, int numberOfLine) {
    std::cout << numberOfLine << ": loadNewQuestion: " << line << "\n";
}

void loadData() {

    std::string line;
    int numberOfLine = 0;

    while (std::getline(std::cin, line)) {

        if (line.empty())
            continue;
        else if (isLetter(line[0]) || line[0] == ' ')
            loadNewTicket(line, numberOfLine);
        else if (isNumber(line[0]))
            loadNewRoute(line, numberOfLine);
        else if (line[0] == '?')
            loadNewQuestion(line, numberOfLine);
        else
            std::cerr << numberOfLine << " " << line << "\n";

        numberOfLine ++;
    }

}

int main() {

    loadData();
    std::vector<std::pair<std::pair<int, int>, std::string> > route = *timetable[3];
    std::cout << route[2].first.second << " " << route[2].second << "\n";

    return 0;
}