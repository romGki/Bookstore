#include <stdio.h>
#include <stdlib.h>

typedef struct book{
	char *title;
	int release_date;
	float price;
} book;

typedef struct author{
	int writer_id;
	char *surname;
	char *name;
	int num_of_books;
} author;

typedef struct writes{
	char *title;
	int writer_id;
} writes;

//Functions
int compare_authors(const void *a, const void *b);
int compare_authors_by_id(const void *a, const void *b);
int compare_books(const void *a, const void *b);
int compare_writes(const void *a, const void *b);
int count_writes_for_book(const char *book_title);
void load_files();
int menu();
void insert_new_writer();
void insert_new_book();
void search_writer();
void search_book();
void delete_writer();
void delete_book();
void save_data();
void free_mem();
