#pragma once
#define structs_h

#include <string>

using namespace std;

enum class Category{
    Concert,
    Sports,
    Workshop,
    Music,
    Marathon,
    Charity,
    Comedy,
    Theatre,
    Pet,
    Food,
    Technology,
    Competition,
    Count // Sentinel value marking the end of the enum
};

// Function to convert Category enum to string
// inline to avoid multiple definition errors
inline const char* catToString(Category category) {
    switch (category) {
        case Category::Concert: return "Concert";
        case Category::Sports: return "Sports";
        case Category::Workshop: return "Workshop";
        case Category::Music: return "Music";
        case Category::Marathon: return "Marathon";
        case Category::Charity: return "Charity";
        case Category::Comedy: return "Comedy";
        case Category::Theatre: return "Theatre";
        case Category::Pet: return "Pet";
        case Category::Food: return "Food";
        case Category::Technology: return "Technology";
        case Category::Competition: return "Competition";
        default: return "Unknown";
    }
}

enum class Gender{
    Male,
    Female
};

enum class State{
    Penang,
    Perak,
    Selangor,
    Johor,
    Pahang,
    Kedah,
    Terengganu,
    Kelantan,
    Sabah,
    Sarawak,
    Melaka,
    Negeri_Sembilan,
    Perlis,
    WP_Kuala_Lumpur,
    WP_Labuan,
    WP_Putrajaya
};

struct Name{
    string first_name;
    string last_name;
};

struct Venue{
    string address;
    string postcode;
    string city;
    State state;
};

struct Event{
    Category category;
    string name;
    string description;
    Venue venue;
    string date;
    string time;
    Organizer organizer;
};

struct Participant{
    Name name;
    string email;
    string gender;
    string phone;
};
