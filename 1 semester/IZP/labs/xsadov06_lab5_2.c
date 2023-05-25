/*
    lab 5, graded program #2
    file name: xloginxx_lab5_2.c
    complete the program and submit to WIS
*/

#include<stdio.h>

struct person_t {
    int age;
    double height;  // in cm
    char sex;   // 'M' for male, 'F' for female
};

// using typedef:
// typedef struct person_t Person;

// load person data from stdin (do not check data validity)
struct person_t load_person() {
    struct person_t person;
    scanf("%d %lf %c", &person.age, &person.height, &person.sex);
    return(person);
}

// print person record
void print_person(struct person_t person) {
    printf("age = %d height = %lf sex = %c \n", person.age, person.height, person.sex);
}

// return true if p1's height is strictly lower than that of p2
int is_lower(struct person_t p1, struct person_t p2) {
    if (p1.height < p2.height)
    {
        return(1);
    }
    else{
        return(0);
    }
}

int main() {
    // array of 3 people
    struct person_t people[3];

    // load 3 records
    for(int i=0; i<3; i++){
        people[i]=load_person();
    }
    // print all records
    for(int i=0; i<3; i++){
    printf("person %d \n",i);
    print_person(people[i]);
    }
    // check if people are sorted by height (lowest person first)
    if(is_lower(people[0],people[1]) && is_lower(people[1],people[2])){
        printf("people are sorted by height");
    }
    else{
        printf("people are sorted not by height");
    }
    return 0;
}
