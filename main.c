#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header_files.h"


int main(int argc, char *argv[]){
	
	int userChoice;
	
	//Loading the text files
    load_files();
		
	//Showing menu and run choises
    printf("Welcome to our programm.\n\nMenu:");
    while (userChoice != 7) {    
    
    	userChoice = menu();

        //Handling the user's choice
        switch (userChoice) {
            case 1:
                printf("\nYou chose to insert a new writer.\n");
                insert_new_writer();
				save_data();
                break;
            case 2:
                printf("You chose to insert a new book.\n");
                insert_new_book();
				save_data();
				break;
            case 3:
                printf("You chose to search a writer.\n");
                search_writer();
                break;
            case 4:
                printf("You chose to search a book.\n");
                search_book();
                break;
            case 5:
                printf("You chose to delete a writer.\n");
                delete_writer();
                save_data();
				break;
            case 6:
                printf("You chose to delete a book.\n");
                delete_book();
                save_data();
				break;
            case 7:
                printf("Exiting the program. Goodbye.\n");
                save_data();
                free_mem();
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
    return 0;
}
