#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "header_files.h"

#define MAX_TITLE_LENGTH 50
#define MAX_NAME_LENGTH 30
#define MAX_FILE_SIZE 256


book *book_arr;
author *author_arr;
writes *writes_arr;
int num_book_records, num_author_records, num_writes_records;


int compare_authors(const void *a, const void *b) {
    author *author_a = (author *)a;
    author *author_b = (author *)b;

    return strcmp(author_a->surname, author_b->surname);
}

int compare_authors_by_id(const void *a, const void *b) {
    const int id_a = *((const int *)a);
    const int id_b = *((const int *)b);

    if (id_a < id_b) {
        return -1;
    } else if (id_a > id_b) {
        return 1;
    } else {
        return 0;
    }
}

int compare_books(const void *a, const void *b) {
    return strcmp(((book *)a)->title, ((book *)b)->title);
}

int compare_writes(const void *a, const void *b) {
    writes *writes_a = (writes *)a;
    writes *writes_b = (writes *)b;

    //First, compare writer IDs
    if (writes_a->writer_id < writes_b->writer_id) {
        return -1;
    } else if (writes_a->writer_id > writes_b->writer_id) {
        return 1;
    }

    //If writer IDs are equal, compare book titles
    return strcmp(writes_a->title, writes_b->title);
}

int count_writes_for_book(const char *book_title) {
    int count = 0;
    for (int i = 0; i < num_writes_records; i++) {
        if (strcmp(writes_arr[i].title, book_title) == 0) {
            count++;
        }
    }
    return count;
}

void load_files(){
	
    FILE *fp_book, *fp_author, *fp_writes;

    //Open the files in read mode
    fp_book = fopen("book.txt", "r");
    fp_author = fopen("author.txt", "r");
    fp_writes = fopen("writes.txt", "r");

	//Checking if there is anything written in the files
    if (fp_book == NULL || fp_author == NULL || fp_writes == NULL){
        printf("Error opening the files.\n");
        return;
    }

	//Scanning the first line of each text file to find out the number of records
	fscanf(fp_book, "%d", &num_book_records);
    fscanf(fp_author, "%d", &num_author_records);
    fscanf(fp_writes, "%d", &num_writes_records);
	
    //Allocating memory for the authors based on the number of records
    book_arr = (book *)malloc(num_book_records * sizeof(book));
    author_arr = (author *)malloc(num_author_records * sizeof(author));
    writes_arr = (writes *)malloc(num_writes_records * sizeof(writes));
    
    //Checking the memory allocations
    if (book_arr == NULL || author_arr == NULL || writes_arr == NULL){
        printf("Memory allocation failed.\n");
        fclose(fp_book);
        fclose(fp_author);
        fclose(fp_writes);
        return;
    }
    	
	//Reading the contents of the files into the arrays
    for (int i = 0; i < num_book_records; i++){
        book_arr[i].title = (char *)malloc(MAX_TITLE_LENGTH * sizeof(char));
        fscanf(fp_book, " %[^\n] %d %f", book_arr[i].title, &book_arr[i].release_date, &book_arr[i].price);
    }
    for (int j = 0; j < num_author_records; j++){
        author_arr[j].surname = (char *)malloc(MAX_NAME_LENGTH * sizeof(char));
        author_arr[j].name = (char *)malloc(MAX_NAME_LENGTH * sizeof(char));
        fscanf(fp_author, " %d %[^\n] %[^\n] %d", &author_arr[j].writer_id, author_arr[j].surname, author_arr[j].name, &author_arr[j].num_of_books);
    }
    for (int k = 0; k < num_writes_records; k++){
        writes_arr[k].title = (char *)malloc(MAX_TITLE_LENGTH * sizeof(char));
        fscanf(fp_writes, " %[^\n] %d", writes_arr[k].title, &writes_arr[k].writer_id);
    }
    
    //Sorting books by title
    qsort(book_arr, num_book_records, sizeof(book), compare_books);
    //Sorting authors by id
    qsort(author_arr, num_author_records, sizeof(author), compare_authors_by_id);
    //Sorting writes by id 
    qsort(writes_arr, num_writes_records, sizeof(writes), compare_writes);
    
    //Closing the files
    fclose(fp_book);
    fclose(fp_author);
    fclose(fp_writes);
}

int menu(){
	int userChoice;
	
	//Display the menu options
       	printf("\n1. Insert new writer record\n");
		printf("2. Insert new book record\n");
		printf("3. Search a writer record\n");
		printf("4. Search a book record\n");
		printf("5. Delete a writer record\n");	
		printf("6. Delete a book record\n");	
		printf("7. Exit\n");	

        //Ask the user for their choice
        printf("Enter your choice: ");
        scanf("%d", &userChoice);
        
        return userChoice;
}

void insert_new_writer(){
	
	char surname[MAX_NAME_LENGTH], name[MAX_NAME_LENGTH];

    printf("Enter surname of the author: ");
    scanf("%s", surname);
    
    //Converting the input to uppercase string for more convenient comparisons
	for (int i = 0; i < strlen(surname); i++){
        surname[i] = toupper(surname[i]);
	}

    printf("Enter name of the author: ");
    scanf("%s", name);
    
    //Converting the input to uppercase string for more convenient comparisons
	for (int i = 0; i < strlen(name); i++){
        name[i] = toupper(name[i]);
	}
	
    //Allocating memory for a new author entry
    author new_author;
    new_author.surname = malloc(strlen(surname) + 1);
    new_author.name = malloc(strlen(name) + 1);

    if (new_author.surname == NULL || new_author.name == NULL){
        printf("Memory allocation failed.\n");
        return;
    }

    strcpy(new_author.surname, surname);
    strcpy(new_author.name, name);

    new_author.num_of_books = 0; // Initially, the author has no books

    //Assigning a unique writer_id
    new_author.writer_id = num_author_records + 1;
    
    //Incrementing the total number of authors
    num_author_records++;

    //Allocating memory for a larger author_arr
    author_arr = realloc(author_arr, num_author_records * sizeof(author));

    //Appending the new author entry to the author_arr
    author_arr[num_author_records - 1] = new_author;
    
    //Sorting authors by id
    qsort(author_arr, num_author_records, sizeof(author), compare_authors_by_id);
    
	printf("Author added successfully.\n");
}

void insert_new_book(){
    char title[MAX_TITLE_LENGTH];
    int release_date, number_of_authors;
    float price;

    // Input book details
    printf("Enter the title of the book: ");
    getchar(); 		//Consuming newline character from previous input
    fgets(title, sizeof(title), stdin);
    title[strcspn(title, "\n")] = '\0'; // Remove newline character

    printf("Enter the release date of the book: ");
    scanf("%d", &release_date);

    printf("Enter the price of the book: ");
    scanf("%f", &price);

    printf("Enter the number of authors for the book: ");
    scanf("%d", &number_of_authors);

    //Converting the input title to uppercase
    for (int i = 0; title[i]; i++){
        title[i] = toupper(title[i]);
    }

    //Allocating memory for a new book entry
    book new_book;
    new_book.title = malloc(strlen(title) + 1);
    
    
    if (new_book.title == NULL){
        printf("Memory allocation failed.\n");
        return;
    }
    
    strcpy(new_book.title, title);
    new_book.release_date = release_date;
    new_book.price = price;

    //Incrementing the total number of books
    num_book_records++;

    //Allocating memory for a larger book_arr
    book_arr = realloc(book_arr, num_book_records * sizeof(book));

    //Appending the new book entry to the book_arr
    book_arr[num_book_records - 1] = new_book;

    for (int i = 0; i < number_of_authors; i++) {
        char surname[MAX_NAME_LENGTH], name[MAX_NAME_LENGTH];

        printf("Enter surname for author %d: ", i + 1);
        scanf("%s", surname);

        // Converting the input surname to uppercase
        for (int j = 0; surname[j]; j++) {
            surname[j] = toupper(surname[j]);
        }
    
        printf("Enter the name of the author: ");
        scanf("%s", name);

        //Converting the input name to uppercase
        for (int j = 0; name[j]; j++) {
            name[j] = toupper(name[j]);
        }

        author temp_author;
        temp_author.surname = surname;
        temp_author.name = name;

		//Sorting authors by id
    	qsort(author_arr, num_author_records, sizeof(author), compare_authors);
        //Use binary search to find the writer
        author *found_author = bsearch(&temp_author, author_arr, num_author_records, sizeof(author), compare_authors);

        if (found_author != NULL) {
            printf("Author with surname %s and name %s already exists. Book is being associated with this author.\n", surname, name);
            found_author->num_of_books++; // Increase the author's num_of_books
        } else {
            printf("Author with surname %s and name %s does not exist. Will add them at once.\n", surname, name);

            //Allocating memory for a new author entry
            author new_author;
            new_author.surname = malloc(strlen(surname) + 1);;
            new_author.name = malloc(strlen(name) + 1);;

			if(new_author.surname == NULL || new_author.name == NULL) {
			    printf("Memory allocation failed for author.\n");
			    //Release any previously allocated memory
			    free(new_author.surname);
			    free(new_author.name);
			    return;
			}


			strcpy(new_author.surname, surname);
        	strcpy(new_author.name, name);
            new_author.num_of_books = 1; 		//Initially, the author has one book

            //Assigning a unique writer_id
            new_author.writer_id = num_author_records + 1;

            //Incrementing the total number of authors
            num_author_records++;

            //Allocating memory for a larger author_arr
            author_arr = realloc(author_arr, num_author_records * sizeof(author));

            //Appending the new author entry to the author_arr
            author_arr[num_author_records - 1] = new_author;
        }

        //Creating new writes entry
	    writes new_writes;
	    new_writes.title = malloc(strlen(new_book.title) + 1);
	    
		int writer_id_to_assign = num_author_records;
		if (found_author != NULL) {
		    writer_id_to_assign = found_author->writer_id;
		}
		
		new_writes.writer_id = writer_id_to_assign;
	
	    if (new_writes.title == NULL) {
	        printf("Memory allocation failed.\n");
	        return;
	    }
	
	    strcpy(new_writes.title, new_book.title);
	
	    //Incrementing the total number of writes entries
	    num_writes_records++;
	
	    //Allocating memory for a larger writes_arr
	    writes_arr = realloc(writes_arr, num_writes_records * sizeof(writes));
	
	    //Appending the new writes entry to the writes_arr
	    writes_arr[num_writes_records - 1] = new_writes;
    }

	//Sorting books by title
    qsort(book_arr, num_book_records, sizeof(book), compare_books);
    
    printf("Book added successfully.\n");
    save_data();
}


void search_writer(){
	
	char search_surname[MAX_NAME_LENGTH], search_name[MAX_NAME_LENGTH];
	
	printf("Enter the surname of the writer you want to search: ");
	scanf("%s", search_surname);
	
	//Converting the input to uppercase string for more convenient comparisons
	for (int i = 0; i < strlen(search_surname); i++){
        search_surname[i] = toupper(search_surname[i]);
	}
	
	author temp_author;
    temp_author.surname = search_surname;

	//Sorting authors by id
    qsort(author_arr, num_author_records, sizeof(author), compare_authors);
    // Use binary search to find the writer
    author *found_author = bsearch(&temp_author, author_arr, num_author_records, sizeof(author), compare_authors);
	
    if (found_author != NULL){
        // Writer found, display their information
        printf("Writer found:\n");
        printf("ID: %d\n", found_author->writer_id);
        printf("Surname: %s\n", found_author->surname);
        printf("Name: %s\n", found_author->name);
        printf("Number of Books: %d\n", found_author->num_of_books);
        
        //Printing book(s) information
        printf("Book(s) information:\n");
        for(int i = 0; i < num_writes_records; i++){
            if(writes_arr[i].writer_id == found_author->writer_id){
                for(int j = 0; j < num_book_records; j++){
                    if(strcmp(book_arr[j].title, writes_arr[i].title) == 0){
                        printf("Title: %s\nRelease date: %d\nPrice: %.2f\n", book_arr[j].title, book_arr[j].release_date, book_arr[j].price);
                        break; //Found book, no need to continue searching
                    }
                }
            }
        }
    } else{
        printf("Writer with surname %s not found.\n", search_surname);
    }
}

void search_book(){

	char search_title[MAX_TITLE_LENGTH];
	
	printf("Enter the title of the book you want to search: ");
	getchar(); 		//Consuming newline character from previous input
    fgets(search_title, sizeof(search_title), stdin);		//Using fgets because the title may contain space characters
    
    //Removing newline character at the end of the input
    search_title[strcspn(search_title, "\n")] = '\0';
    
	//Converting the input to uppercase string for more convenient comparisons
	for (int i = 0; i < strlen(search_title); i++){
        search_title[i] = toupper(search_title[i]);
	}
	
	book temp_book;
    temp_book.title = search_title;

    // Use binary search to find the book
    book *found_book = bsearch(&temp_book, book_arr, num_book_records, sizeof(book), compare_books);

    if (found_book != NULL){
        //Book found, display their information
        printf("Book found:\n");
        printf("Title: %s\n", found_book->title);
        printf("Release date: %d\n", found_book->release_date);
        printf("Price: %.2f\n", found_book->price);
        
        //Printing author(s) information
        printf("Author(s) information:\n");
        for(int i = 0; i < num_writes_records; i++){
            if(strcmp(writes_arr[i].title, found_book->title) == 0){
                for(int j = 0; j < num_author_records; j++){
                    if(author_arr[j].writer_id == writes_arr[i].writer_id){
                        printf("ID: %d\nSurname: %s\nName: %s\nNumber of Books: %d\n", author_arr[j].writer_id, author_arr[j].surname, author_arr[j].name, author_arr[j].num_of_books);
                        break; //Found author, no need to continue searching
                    }
                }
            }
        }
    } else{
        printf("Book with title %s not found.\n", search_title);
    }
}

void delete_writer() {
    int writer_id;

	//Sorting authors by id
    qsort(author_arr, num_author_records, sizeof(author), compare_authors_by_id);
    // Printing the authors
    printf("\nAuthors:\n");
    for (int i = 0; i < num_author_records; i++) {
        printf("ID: %d, Surname: %s, Name: %s, Number of Books: %d\n", author_arr[i].writer_id, author_arr[i].surname, author_arr[i].name, author_arr[i].num_of_books);
    }

    printf("Enter the ID of the writer you want to delete: ");
    scanf("%d", &writer_id);

    // Use binary search to find the author
    author temp_author;
    temp_author.writer_id = writer_id;

    author *delete_author = bsearch(&temp_author, author_arr, num_author_records, sizeof(author), compare_authors_by_id);

    if (delete_author != NULL) {
        // Author found, delete their information
        printf("Deleting author:\n");
        printf("ID: %d\n", delete_author->writer_id);
        printf("Surname: %s\n", delete_author->surname);
        printf("Name: %s\n", delete_author->name);
        printf("Number of Books: %d\n", delete_author->num_of_books);

        // Delete associated writes entries and books
        for (int i = 0; i < num_writes_records; i++) {
            if (writes_arr[i].writer_id == writer_id) {
                writes temp_writes;
                temp_writes.writer_id = writer_id;
                temp_writes.title = writes_arr[i].title;

                // Find the associated writes entry using binary search
                writes *associated_writes = bsearch(&temp_writes, writes_arr, num_writes_records, sizeof(writes), compare_writes);

                if (associated_writes != NULL) {
                    // Delete the writes entry
                    for (int j = i; j < num_writes_records - 1; j++) {
                        writes_arr[j] = writes_arr[j + 1];
                    }
                    num_writes_records--;
                    writes_arr = realloc(writes_arr, num_writes_records * sizeof(writes));

                    // Find the associated book using binary search
                    book temp_book;
                    temp_book.title = associated_writes->title;
                    book *associated_book = bsearch(&temp_book, book_arr, num_book_records, sizeof(book), compare_books);

                    if (associated_book != NULL) {
                        // If the author is the sole author of the book, delete the book
                        int num_writes_for_book = count_writes_for_book(associated_book->title);
                        if (num_writes_for_book == 0) {
                            free(associated_book->title);
                            for (int j = associated_book - book_arr; j < num_book_records - 1; j++) {
                                book_arr[j] = book_arr[j + 1];
                            }
                            num_book_records--;
                            book_arr = realloc(book_arr, num_book_records * sizeof(book));
                        }
                    }
                }
            }
        }

        // Delete the author
        free(delete_author->surname);
        free(delete_author->name);
        for (int i = delete_author - author_arr; i < num_author_records - 1; i++) {
            author_arr[i] = author_arr[i + 1];
        }
        num_author_records--;
        author_arr = realloc(author_arr, num_author_records * sizeof(author));

        printf("Author deleted successfully.\n");
    } else {
        printf("Author with ID %d not found.\n", writer_id);
    }
}

void delete_book() {
    char title[MAX_TITLE_LENGTH];
    
    printf("Books:\n");
    for (int i = 0; i < num_book_records; i++) {
        printf("Title: %s, Release Date: %d, Price: %.2f\n", book_arr[i].title, book_arr[i].release_date, book_arr[i].price);
    }
    
    printf("Enter the title of the book you want to delete: ");
    getchar(); // Consuming newline character from previous input
    fgets(title, sizeof(title), stdin);
    title[strcspn(title, "\n")] = '\0'; // Remove newline character
    
    // Convert the input title to uppercase
    for (int i = 0; title[i]; i++) {
        title[i] = toupper(title[i]);
    }

    // Create a temporary book with the title for binary search
    book temp_book;
    temp_book.title = title;

    // Use binary search to find the book
    book *found_book = bsearch(&temp_book, book_arr, num_book_records, sizeof(book), compare_books);

    if (found_book != NULL) {
        // Book found, delete its information
        printf("Deleting book:\n");
        printf("Title: %s\n", found_book->title);
        printf("Release date: %d\n", found_book->release_date);
        printf("Price: %.2f\n", found_book->price);
        
        // Find and delete associated writes entries
        for (int i = 0; i < num_writes_records; i++) {
            if (strcmp(writes_arr[i].title, found_book->title) == 0) {
                int author_id = writes_arr[i].writer_id;
                
                // Find the author by ID
                author temp_author;
                temp_author.writer_id = author_id;
                author *found_author = bsearch(&temp_author, author_arr, num_author_records, sizeof(author), compare_authors_by_id);
                
                if (found_author != NULL) {
                    found_author->num_of_books--;
                }
                
                // Shift remaining writes entries to fill the gap
                for (int j = i; j < num_writes_records - 1; j++) {
                    writes_arr[j] = writes_arr[j + 1];
                }
                
                num_writes_records--;
                writes_arr = realloc(writes_arr, num_writes_records * sizeof(writes));
                i--; // Check the same index again since we shifted the array
            }
        }
        
        // Delete the book
        free(found_book->title);
        for (int i = found_book - book_arr; i < num_book_records - 1; i++) {
            book_arr[i] = book_arr[i + 1];
        }
        num_book_records--;
        book_arr = realloc(book_arr, num_book_records * sizeof(book));
        
        printf("Book deleted successfully.\n");
    } else {
        printf("Book with title %s not found.\n", title);
    }
}

void save_data(){
    // Save book data
    FILE *fp_book = fopen("book.txt", "w");
    if (fp_book == NULL){
        printf("Error opening book.txt for writing.\n");
        return;
    }
    fprintf(fp_book, "%d\n", num_book_records);
    for (int i = 0; i < num_book_records; i++){
        fprintf(fp_book, "%s\n%d\n%.2f\n", book_arr[i].title, book_arr[i].release_date, book_arr[i].price);
    }
    fclose(fp_book);

    // Save author data
    FILE *fp_author = fopen("author.txt", "w");
    if (fp_author == NULL){
        printf("Error opening author.txt for writing.\n");
        return;
    }
    fprintf(fp_author, "%d\n", num_author_records);
    for (int i = 0; i < num_author_records; i++){
        fprintf(fp_author, "%d\n%s\n%s\n%d\n", author_arr[i].writer_id, author_arr[i].surname, author_arr[i].name, author_arr[i].num_of_books);
    }
    fclose(fp_author);

    // Save writes data
    FILE *fp_writes = fopen("writes.txt", "w");
    if (fp_writes == NULL){
        printf("Error opening writes.txt for writing.\n");
        return;
    }
    fprintf(fp_writes, "%d\n", num_writes_records);
    for (int i = 0; i < num_writes_records; i++){
        fprintf(fp_writes, "%s\n%d\n", writes_arr[i].title, writes_arr[i].writer_id);
    }
    fclose(fp_writes);
}


void free_mem(){
	//Printing the contents of the files
	printf("Books:\n");
    for (int i = 0; i < num_book_records; i++){
        printf("Title: %s, Release Date: %d, Price: %.2f\n",
               book_arr[i].title, book_arr[i].release_date, book_arr[i].price);
    }
    printf("\nAuthors:\n");
    for (int i = 0; i < num_author_records; i++){
        printf("ID: %d, Surname: %s, Name: %s, Number of Books: %d\n",
               author_arr[i].writer_id, author_arr[i].surname, author_arr[i].name, author_arr[i].num_of_books);
    }
    printf("\nWrites:\n");
    for (int i = 0; i < num_writes_records; i++){
        printf("Title: %s, Writer ID: %d\n", writes_arr[i].title, writes_arr[i].writer_id);
    }
    
    
    
    // Free memory for book_arr
    for (int i = 0; i < num_book_records; i++){
        free(book_arr[i].title);
    }
    free(book_arr);

    // Free memory for author_arr
    for (int i = 0; i < num_author_records; i++){
        free(author_arr[i].surname);
        free(author_arr[i].name);
    }
    free(author_arr);

    // Free memory for writes_arr
    for (int i = 0; i < num_writes_records; i++){
        free(writes_arr[i].title);
    }
    free(writes_arr);
}
