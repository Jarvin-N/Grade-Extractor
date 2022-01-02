/*
Usage: Reads a passed CSV grade file and prints the average 
grade for each assignment recorded in the file.
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/**
 * This function counts the number of commas in a passed string and
 * returns it.
*/
int number_of_commas(char *string){
    int i = 0;
    int comma_counter = 0;
    bool valid_flag = false; //Keeps track if characters are in quotes.
    for(i=0;i<strlen(string);i++){
        if(string[i] == '\"'){
            if(valid_flag == false){
                valid_flag = true;
            }
            else if(valid_flag == true){
                valid_flag = false;
            }
        }
        else if(valid_flag == false){
            if(string[i] == ','){
                comma_counter = comma_counter + 1;
            }
        }
    }
    return comma_counter;
}

/**
 * This function reads a passed string for the location of commas
 * and then places those locations into an int array.
 */
void comma_indexs(char *string, int *result){
    int i = 0;
    //Keeps track if characters are in quotes.
    bool valid_flag = false;
    //Only add the place of the second comma.
    int comma_count = 0;
    int result_index = -1;
    for(i=0;i<strlen(string);i++){
        if(string[i] == '\"'){
            if(valid_flag == false){
                valid_flag = true;
            }
            else if(valid_flag == true){
                valid_flag = false;
            }
        }
        else if(valid_flag == false){
            if(string[i] == ','){
                if(comma_count > 0){
                    result[result_index = result_index + 1] = i;
                }
                else{
                    comma_count = comma_count+1;
                }
            }
        }
    }
}

/**
 * This function extracts the gradees from a passed string, and stores them inside
 * of a 2d int array.
 */
int **extract_grades(char *string, int **grades, int number_of_assignments, int current_student){
    //First find the location of the commas for the passed string.
    int comma_locations[number_of_assignments];
    comma_indexs(string, comma_locations);
    int i=0;
    int *gradeBook = (int*)malloc(number_of_assignments*sizeof(int));
    //Now extract the data betweeen the commas.
    for(i=0;i<number_of_assignments;i++){
        //Use holder as variable that holds the found grade.
        char holder[3] = "zzz";
        //If statement makes sure to stop grabbing the data from the next comma.
        if(i != (number_of_assignments-1)){
            int next_index = comma_locations[i+1];
            int j=0;
            for(j=comma_locations[i];j<next_index;j++){
                if(string[j] >= '0' && string[j] <= '9'){
                   int l = 0;
                   for(l=0;l<strlen(holder);l++){
                       if(holder[l] == 'z'){
                           holder[l] = string[j];
                           //Break the loop to prevent all of holder from being over written.
                           break;
                       }
                   } 
                }
            }
        }
        else{
            int j=0;
            for(j=comma_locations[i];j<strlen(string);j++){
                if(string[j] >= '0' && string[j] <= '9'){
                   int l = 0;
                   for(l=0;l<strlen(holder);l++){
                       if(holder[l] == 'z'){
                           holder[l] = string[j];
                           break;
                       }
                   }
                }
            }
        }
        //If all values of holder are unchanged skip the next step.
        bool changed = false;
        int l = 0;
        for(l=0;l<strlen(holder);l++){
            if(holder[l] != 'z'){
                changed = true;
            }
        }
        if(changed == true){
            //Remove the holding value z.
            for(l=0;l<strlen(holder);l++){
                if(holder[l] == 'z'){
                    holder[l] = '\0';
                }
            }
        }
        if(changed==false){
            for(l=0;l<strlen(holder);l++){
                if(holder[l] == 'z'){
                    /*
                    Set it equal to 1 because an unknown grade will have the 
                    value 111 which is unattainable.
                    */
                    holder[l] = '1';
                }
            }
        }
        int final_grade = atoi(holder);
        gradeBook[i] = final_grade;
    }
    int k = 0;
    //Update the 2d array of grades.
    for(i=0;i<number_of_assignments;i++){
            grades[current_student][i] = gradeBook[i];
    }
    free(gradeBook);
    return grades;
}

/**
 * This function calculates the average grade given the numbeer of students
 * and their marks. Returns the calculated grade.
 */
float average_calculator(int **grades, int number_of_students, int assignment_number){
    int i=0;
    float total_marks = 0;
    float total_grades = 0;
    for(i=0;i<number_of_students;i++){
        float current_grade = grades[i][assignment_number];
        //111 is a placeholder grade for people who didnt complete a test or no grade.
        if(current_grade != 111){
            total_grades = total_grades + current_grade;
            total_marks = total_marks + 1;
        }
    }
    float average = (total_grades/total_marks);
    return average;
}

int main(int argc, char* argv[]){
    //Open the passed file.
    FILE *grades_file = fopen(argv[1], "r");
    //Check if the file opens correctly.
    if(grades_file == NULL){
        fprintf(stderr, "File failed to open.");
        exit(1);
    }
    //Convert the file into a 2D array.
    int i=0;
    int k=0;
    char *students[100];
    char line[150];
    while(fgets(line, 150, grades_file)){
        students[i] = strdup(line);
        i++;
    }
    fclose(grades_file);
    //Calculate the number of students
    int number_of_students = i;
    int total_commas = 0;
    //Calculate the number of commas in a string.
    for(i=0;i<number_of_students;i++){
        int comma_count = number_of_commas(students[i]);
        total_commas = total_commas + comma_count;
    }
    //Calculate the total number of assignments.
    int number_of_assignments = (total_commas/number_of_students)-1;
    //Grab the index's of where the grades are located after
    int **grades_holder = malloc(sizeof(int*) * number_of_students);
    for(i=0; i<number_of_students; i++){
        grades_holder[i] = malloc(sizeof(int*) * number_of_assignments);
    }
    for(i=0;i<number_of_students;i++){
        grades_holder = extract_grades(students[i], grades_holder, number_of_assignments, i);
        for(k=0;k<number_of_assignments;k++){
        }
    } 
    //Calculating the average grades.
    for(i=0;i<number_of_assignments;i++){
        float average_grade = average_calculator(grades_holder, number_of_students, i);
        printf("A%d %.1f \n", i+1, average_grade);
    }
    
    free(grades_holder);
}