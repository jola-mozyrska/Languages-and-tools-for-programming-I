#include <string>
#include <algorithm>
#include <iostream>
#include <tuple>
#include <unordered_map>
#include <vector>
#include <climits>
#include <regex>

using namespace std;

const int mx_time = 950;
const long long INF = numeric_limits<double>::infinity();

const int lower_time_limit = 355;
const int upper_time_limit = 1281;

const int minutes_in_hour = 60;
const int pennys_in_price_integral = 100;

const regex assistant_regex("[0-9]+|[a-zA-Z_^]+");
const regex question_regex(
        "[?] [a-zA-Z_^]+(?: (?:(?:0*[1-9][0-9]{0,17})|(?:0+)) [a-zA-Z_^]+)+");
const regex tram_regex(
        "(?:(?:0*[1-9][0-9]{0,17})|(?:0+))(?: [1-9][0-9]?[:][0-9]{2} [a-zA-Z_^]+)+");
const regex ticket_regex(
        "([a-zA-Z ]+) (0|[1-9][0-9]{0,17})[.]([0-9]{2}) ([1-9][0-9]{0,17})");

//  checks if the tram number was already added
bool is_tram_present(const long long tram_number,
                     unordered_map<long long, unordered_map<string, int> > &schedule_for_trams) {
    if (schedule_for_trams.find(tram_number) == schedule_for_trams.end())
        return false;
    return true;
}

//  checks if the tram stops at given stop
bool check_if_tram_at_stop(const long long tram_number, string &stop,
                           unordered_map<long long, unordered_map<string, int> > &schedule_for_trams) {
    if (!is_tram_present(tram_number, schedule_for_trams))
        return false;

    auto schedule = schedule_for_trams[tram_number];
    if (schedule.count(stop) == 0)
        return false;

    return true;
}

//  adds tram trace to schedule
//  returns false if there is a loop in given tram trace
bool add_tram(const long long tram_number, vector<int> &schedule_time,
              vector <string> &tram_stops,
              unordered_map<long long, unordered_map<string, int> > &schedule_for_trams) {

    for (size_t i = 0; i < tram_stops.size(); ++i) {
        auto stop_name = tram_stops[i];
        auto stop_time = schedule_time[i];

        bool stop_repeated = check_if_tram_at_stop(tram_number, stop_name,
                                                   schedule_for_trams);
        if (stop_repeated)
            return false;

        schedule_for_trams[tram_number][stop_name] = stop_time;
    }
    return true;
}

//  adds new avilable ticket and updates optimal cost for given time
void add_ticket(string &name, const long long price, long long minutes,
                vector <string> &ticket_name,
                vector<long long> &ticket_price, vector<long long> &ticket_time,
                vector<long long> &cost,
                vector <vector<string> > &proposed_tickets) {
    //  time exceeds minutes in a day
    if (minutes > mx_time)
        minutes = mx_time;
    ticket_name.push_back(name);
    ticket_price.push_back(price);
    ticket_time.push_back(minutes);

    cost[0] = 0;
    for (int multiple_ticket = 1; multiple_ticket <= 3; ++multiple_ticket) {
        for (long long i = mx_time; i >= minutes; --i) {
            //  greater time has better tickets
            if (i < mx_time) {
                if (cost[i] > cost[i + 1]) {
                    cost[i] = cost[i + 1];
                    proposed_tickets[i] = proposed_tickets[i + 1];
                } else if (cost[i] == cost[i + 1] &&
                           proposed_tickets[i].size() >
                           proposed_tickets[i + 1].size()) {
                    cost[i] = cost[i + 1];
                    proposed_tickets[i] = proposed_tickets[i + 1];
                }
            }

            //  knapsack approach, use previously counted cost
            if (cost[i - minutes] != INF &&
                proposed_tickets[i - minutes].size() < 3) {
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
}

//  Prints names of best maximum 3 tickets
//  returns true if nothing have to be printed on stderr
bool ask_for_tickets(vector <string> &stops, vector<long long> &trams_numbers,
                     vector <vector<string> > &proposed_tickets,
                     unordered_map<long long, unordered_map<string, int> > &schedule_for_trams,
                     long long &number_of_tickets) {
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

        //  we are too late for the tram
        if (arrival_time > departure_time)
            return false;

        //  we have to wait for the tram
        if (arrival_time < departure_time && stop_to_wait != "")
            stop_to_wait = stop_name;

        previous_tram = trams_numbers[i];
        previous_departure_time = departure_time;
    }

    if (stop_to_wait != "") {
        cout << ":-( " << stop_to_wait << endl;
        return true;
    }

    auto last_stop_name = stops[n];
    auto first_time = schedule_for_trams[trams_numbers[0]][first_stop_name];
    auto last_time = schedule_for_trams[trams_numbers[n]][last_stop_name];
    auto summary_time = last_time - first_time + 1;

    //  we can't buy tickets
    if (proposed_tickets[summary_time].size() == 0) {
        cout << ":-|" << endl;
        return true;
    }

    number_of_tickets += proposed_tickets[summary_time].size();
    bool semicolon = false;
    cout << "!";
    for (auto ticket: proposed_tickets[summary_time]) {
        if (semicolon)
            cout << ";";
        else
            semicolon = true;
        cout << " " << ticket;
    }
    cout << endl;

    return true;
}

void print_error(string line, int line_number) {
    cerr << "Error in line " << line_number << ": " << line << endl;
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

void question_about_ticket_command(string line, int line_number,
                                   vector <string> &stops,
                                   vector<long long> &trams_numbers,
                                   vector <vector<string> > &proposed_tickets,
                                   long long &number_of_tickets,
                                   unordered_map<long long, unordered_map<string, int> > &schedule_for_trams) {
    bool error = false;
    int number_of_stops = 0;
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
        if (is_tram_present(trams_numbers.at(j), schedule_for_trams)) {
            i++;
            stops.at(j + 1) = (*i).str();
            if (!check_if_tram_at_stop(trams_numbers.at(j), stops.at(j),
                                       schedule_for_trams)
                || !check_if_tram_at_stop(trams_numbers.at(j), stops.at(j + 1),
                                          schedule_for_trams)) {
                error = true;
            }
            i++;
            j++;
        } else {
            error = true;
        }
    }
    if (!error && !ask_for_tickets(stops, trams_numbers, proposed_tickets,
                                   schedule_for_trams, number_of_tickets)) {
        error = true;
    }


    if (error) {
        print_error(line, line_number);
    }
}

void add_tram_command(string line, int line_number, long long tram_number,
                      vector <string> &stops, vector<int> &times,
                      unordered_map<long long, unordered_map<string, int> > &schedule_for_trams) {
    bool error = false;
    int number_of_stops = 0;

    sregex_iterator i = sregex_iterator(line.begin(),
                                        line.end(), assistant_regex);
    tram_number = stoll((*i).str());
    i++;
    if (!is_tram_present(tram_number, schedule_for_trams)) {
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

    if (error) {
        print_error(line, line_number);
    } else {
        if (!add_tram(tram_number, times, stops,
                      schedule_for_trams)) {
            print_error(line, line_number);
        }
    }
}

void add_ticket_command(smatch result, string &ticket_name, long long &price,
                        long long &validity) {
    smatch::iterator i = result.begin();
    long long price_integral;
    i++;
    ticket_name = *i;
    i++;
    price_integral = stoll((*i).str());
    i++;
    price = price_to_pennys(price_integral, stoi((*i).str()));
    i++;
    validity = stoll(*i);
}

int main() {
    smatch result;
    string line, name = "";
    int line_number = 0;
    long long number_of_tickets = 0, price = -1, validity = -1, tram_number = -1;
    unordered_map<long long, unordered_map<string, int> > schedule_for_trams;
    vector <string> stops, ticket_name;
    vector<long long> trams_numbers, ticket_price, ticket_time;
    vector<long long> cost(mx_time + 2, INF);
    vector <vector<string> > proposed_tickets(mx_time + 2);
    vector<int> times;

    while (!getline(cin, line).eof()) {
        line_number++;
        if (!line.empty()) {
            if (regex_match(line, question_regex)) {
                question_about_ticket_command(line, line_number, stops,
                                              trams_numbers, proposed_tickets,
                                              number_of_tickets,
                                              schedule_for_trams);
            } else if (regex_match(line, tram_regex)) {
                add_tram_command(line, line_number, tram_number, stops, times,
                                 schedule_for_trams);
            } else if (regex_match(line, result, ticket_regex)) {
                add_ticket_command(result, name, price, validity);
                add_ticket(name, price, validity, ticket_name, ticket_price,
                           ticket_time, cost, proposed_tickets);
            } else {
                print_error(line, line_number);
            }
        }
    }

    cout << number_of_tickets << endl;
}
