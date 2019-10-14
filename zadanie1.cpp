#include <string>
#include <algorithm>
#include <iostream>
#include <tuple>
#include <unordered_map>
#include <vector>
#include <climits>
#include <regex>

using namespace std;

// TODO:komentarze
// TODO:test

//unordered_map<long long, unordered_map<string, int>> &schedule_for_trams;
//
//vector<string> ticket_name;
//vector<double> ticket_price;
//vector<long long> ticket_time;
//
//vector<long long> cost(mx_time, INF);
//vector<vector<long long>> proposed_tickets(mx_time + 4);

const int mx_time = 950;
const long long INF = numeric_limits<double>::infinity();

bool is_tram_present(const long long tram_number,
                     unordered_map<long long, unordered_map<string, int>> &schedule_for_trams) {
    if (schedule_for_trams.find(tram_number) == schedule_for_trams.end())
        return false;
    return true;
}

bool check_if_tram_at_stop(const long long tram_number, string &stop,
                           unordered_map<long long, unordered_map<string, int>> &schedule_for_trams) {
    if (!is_tram_present(tram_number, schedule_for_trams))
        return false;

    auto schedule = schedule_for_trams[tram_number];
    if (schedule.count(stop) == 0)
        return false;

    return true;
}

bool add_tram(const long long tram_number, vector<int> &schedule_time,
              vector <string> &tram_stops,
              unordered_map<long long, unordered_map<string, int>> &schedule_for_trams) {

    // ull?
    for (unsigned long long i = 0; i < tram_stops.size(); ++i) {
        auto stop_name = tram_stops[i];
        auto stop_time = schedule_time[i];
        string destination = "*";
        if (i + 1 < tram_stops.size())
            destination = tram_stops[i + 1];

        bool stop_repeated = check_if_tram_at_stop(tram_number, stop_name,
                                                   schedule_for_trams);
        if (stop_repeated)
            return false;

        schedule_for_trams[tram_number][stop_name] = stop_time;
    }
    return true;
}

void add_ticket(string &name, const long long price, long long minutes,
                vector <string> &ticket_name,
                vector<long long> &ticket_price, vector<long long> &ticket_time,
                vector<long long> cost,
                vector <vector<string>> proposed_tickets) {
    if (minutes > mx_time)
        minutes = mx_time;
    ticket_name.push_back(name);
    ticket_price.push_back(price);
    ticket_time.push_back(minutes);

    cost[0] = 0;

    for (long long i = mx_time; i >= minutes; --i) {
        if (proposed_tickets[i - minutes].size() < 3) {
            if (cost[i] > cost[i - minutes] + price) {
                cost[i] = cost[i - minutes] + price;
                proposed_tickets[i] = proposed_tickets[i - minutes];
                proposed_tickets[i].push_back(name);
            } else if (cost[i] == cost[i - minutes] + price
                       && proposed_tickets[i - minutes].size() + 1 <
                          proposed_tickets[i].size()) {

                proposed_tickets[i] = proposed_tickets[i - minutes];
                proposed_tickets[i].push_back(name);
            }

        }
    }
}

bool ask_for_tickets(vector <string> &stops, vector<long long> &trams_numbers,
                     vector <vector<string>> proposed_tickets,
                     unordered_map<long long, unordered_map<string, int>> &schedule_for_trams) {
    long long n = trams_numbers.size();
    trams_numbers.push_back(trams_numbers[n - 1]);
    auto previous_tram = trams_numbers[0];
    auto first_stop_name = stops[0];
    auto previous_departure_time = schedule_for_trams[trams_numbers[0]][first_stop_name];
    string stop_to_wait = "";

    for (long long i = 0; i <= n; ++i) {
        auto stop_name = stops[i];
        auto arrival_time = schedule_for_trams[previous_tram][stop_name];
        auto departure_time = schedule_for_trams[trams_numbers[i]][stop_name];

        if (arrival_time < previous_departure_time)
            return false;

        if (arrival_time < departure_time && stop_to_wait != "")
            stop_to_wait = stop_name;

        if (arrival_time > departure_time)
            return false;

        previous_tram = trams_numbers[i];
        previous_departure_time = departure_time;
    }

    if (stop_to_wait != "") {
        cout << ":-( " << stop_to_wait << "\n";
        return true;
    }

    auto last_stop_name = stops[n];
    auto first_time = schedule_for_trams[trams_numbers[0]][first_stop_name];
    auto last_time = schedule_for_trams[trams_numbers[n]][last_stop_name];
    auto summary_time = last_time - first_time + 1;

    bool semicolon = false;
    cout << "!";
    for (auto ticket: proposed_tickets[summary_time]) {
        if (semicolon)
            cout << ";";
        else
            semicolon = true;
        cout << " " << ticket;
    }
    cout << "\n";

    return true;
}

//int main() {
//
//    long long x = 1;
//    string p = "b";
//
//    vector<int> schedule_time = {1, 2, 3};
//    vector<string> tram_stops = {"a", "b", "c"};
//    add_tram(x, schedule_time, tram_stops);
////cout << (int)check_if_tram_at_stop(x, p);
//
//    vector<string> stops = {"a", "c", "b"};
//    vector<long long> trams_numbers = {1, 1};
//    cout << ask_for_tickets(stops, trams_numbers);
//}

const int lower_time_limit = 355;
const int upper_time_limit = 1281;

const int minutes_in_hour = 60;
const int pennys_in_price_integral = 100;

const regex assistant_regex("[0-9]+|[a-zA-Z_^]+");
const regex question_regex("[?]\\s[a-zA-Z_^]+(?:\\s[0-9]+\\s[a-zA-Z_^]+)+");
const regex course_regex("[0-9]+(?:\\s[1-9]?[0-9]:[0-9]{2}\\s[a-zA-Z_^]+)+");
const regex ticket_regex(
        "([a-zA-Z]+(?:\\s[a-zA-Z]+)*)\\s([0-9]+)[.]([0-9]{2})\\s([0-9]+)");

void print_error(string line, int line_number) {
    cerr << "Error in line " << line_number << ": " << line << "\n";
}

int number_of_whitespaces(string line) {
    int counter = 0;

    for (size_t i = 0; i < line.size(); i++) {
        if (line[i] == ' ') {
            counter++;
        }
    }

    return counter;
}

int time_to_minutes(int hours, int minutes) {
    return hours * minutes_in_hour + minutes;
}

long long price_to_pennys(long price_integral, int price_fractional) {
    return price_integral * pennys_in_price_integral + price_fractional;
}

void question_about_ticket(string line, int line_number,
                           vector<string> &stops,
                           vector<long long> &trams_numbers) {
    bool error = false;
    int number_of_stops = 0;

    if (regex_match(line, question_regex)) {
        sregex_iterator i = sregex_iterator(line.begin(),
                                            line.end(), assistant_regex);
        number_of_stops = (number_of_whitespaces(line) + 1) / 2;
        stops = vector<string>(number_of_stops);
        trams_numbers = vector<long long>(number_of_stops - 1);
        int j = 0;
        stops.at(j) = (*i).str();
        i++;
        while ((i != sregex_iterator()) && !error) {
            trams_numbers.at(j) = stoll((*i).str());
            if (is_tram_number_correct(trams_numbers.at(j))) {
                i++;
                stops.at(j + 1) = (*i).str();
                if (!check_if_tram_at_stop(trams_numbers.at(j), stops.at(j))
                    || !check_if_tram_at_stop(trams_numbers.at(j),
                                              stops.at(j + 1))) {
                    error = true;
                }
                i++;
                j++;
            } else {
                error = true;
            }
        }
        if (!ask_for_tickets(stops, trams_numbers)) {
            error = true;
        }
    } else {
        error = true;
    }

    if (error) {
        print_error(line, line_number);
    }
}

void add_tram(string line, int line_number, long long &tram_number,
              vector<string> &stops, vector<int> &times) {
    bool error = false;
    int number_of_stops = 0;

    if (regex_match(line, course_regex)) {
        sregex_iterator i = sregex_iterator(line.begin(),
                                            line.end(), assistant_regex);
        tram_number = stoll((*i).str());
        i++;
        if (is_tram_number_correct(tram_number)) {
            number_of_stops = number_of_whitespaces(line) / 2;
            stops = vector<string>(number_of_stops);
            times = vector<int>(number_of_stops);
            int j = 0, current_time = 0, previous_time = 0, hour = 0;
            while ((i != sregex_iterator()) && !error) {
                hour = stoi((*i).str());
                i++;
                current_time = time_to_minutes(hour, stoi((*i).str()));
                if ((current_time >= lower_time_limit)
                    && (current_time <= upper_time_limit)) {
                    i++;
                    times.at(j) = current_time;
                    stops.at(j) = (*i).str();
                    i++;
                    j++;
                    if (current_time <= previous_time) {
                        error = true;
                    }
                    previous_time = current_time;
                } else {
                    error = true;
                }
            }
        } else {
            error = true;
        }
    } else {
        error = true;
    }

    if (error) {
        print_error(line, line_number);
    }
}

void add_ticket(string line, int line_number,
                string &ticket_name, long long &price, long long &validity) {
    smatch result;

    if (regex_match(line, result, ticket_regex)) {
        smatch::iterator i = result.begin();
        long price_integral;
        i++;
        ticket_name = *i;
        i++;
        price_integral = stol((*i).str());
        i++;
        price = price_to_pennys(price_integral, stoi((*i).str()));
        i++;
        validity = stoll(*i);
    } else {
        print_error(line, line_number);
    }
}

void choose_function(string line, int line_number) {
    char first_sign = line[0];
    string ticket_name = "";
    long long price = 0;
    long long validity = 0;
    long long tram_number = 0;
    vector<string> stops;
    vector<long long> trams_numbers;
    vector<int> times;

    if (first_sign == '?') {
        question_about_ticket(line, line_number, stops, trams_numbers);
    } else if ((first_sign >= '0') && (first_sign <= '9')) {
        add_tram(line, line_number, tram_number, stops, times);
    } else if (((first_sign >= 'A') && (first_sign <= 'Z'))
               || ((first_sign >= 'a') && (first_sign <= 'z'))) {
        add_ticket(line, line_number, ticket_name, price, validity);
    } else {
        print_error(line, line_number);
    }
}

int main() {
//    string line;
//    int line_number = 0;
//
//    while (!getline(cin, line).eof()) {
//        line_number++;
//        if (!line.empty()) {
//            choose_function(line, line_number);
//        }
//    }


}
