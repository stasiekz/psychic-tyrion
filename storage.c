#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "storage.h"



hash_table_t *create_hash_table(int size)
{
	hash_table_t *new_table;
	int i;

	if (size<1) return NULL; /* invalid size for table */

	/* Attempt to allocate memory for the table structure */
	if ((new_table = malloc(sizeof(hash_table_t))) == NULL) {
		return NULL;
	}

	/* Attempt to allocate memory for the table itself */
	if ((new_table->table = malloc(sizeof(list_t *) * size)) == NULL) {
		return NULL;
	}

	/* Initialize the elements of the table */
	for(i=0; i<size; i++) new_table->table[i] = NULL;

	/* Set the table's size */
	new_table->size = size;

	return new_table;
}

unsigned int hash(hash_table_t *hashtable, char **buf)
{
	unsigned int hashval;
	int i = 0;
	char str[2048];
	strcpy(str, buf[0]);
	strcpy(str+strlen(buf[0]), buf[1]);

	/* we start our hash out at 0 */
	hashval = 0;

	/* for each character, we multiply the old hash by 31 and add the current
	 * character.  Remember that shifting a number left is equivalent to 
	 * multiplying it by 2 raised to the number of places shifted.  So we 
	 * are in effect multiplying hashval by 32 and then subtracting hashval.  
	 * Why do we do this?  Because shifting and subtraction are much more 
	 * efficient operations than multiplication.
	 for(; *str != '\0'; str++) hashval = *str + (hashval << 5) - hashval;*/
	while(str[i++]) hashval = *str + (hashval << 5) - hashval;

	/* we then return the hash value mod the hashtable size so that it will
	 * fit into the necessary range
	 */
	return hashval % hashtable->size;
}

list_t *lookup_string(hash_table_t *hashtable, char **buf)
{
	list_t *list;
	char str[2048];
	strcpy(str, buf[0]);
	strcpy(str+strlen(buf[0]), buf[1]);
	unsigned int hashval = hash(hashtable, buf);

	int i;

	/* Go to the correct list based on the hash value and see if str is
	 * in the list.  If it is, return return a pointer to the list element.
	 * If it isn't, the item isn't in the table, so return NULL.
	 */
	for(list = hashtable->table[hashval]; list != NULL; list = list->next) {
		if (!strcmp(buf[0], list->prefix[0]) && !strcmp(buf[1], list->prefix[1])){
			printf("\n\n\n%s %s\n%d\n", buf[0], buf[1], list->i);
			//if(list->liczba_suf < 1000)
			printf("[ ");
			for(i = 0; i < list->liczba_suf; i++)
				printf("%s ", list->suffix[i]);
			printf(" ]");
			return list;
		}
	}
	return NULL;
}

int add_string(hash_table_t *hashtable, char **buf)
{
	list_t *new_list;
	list_t *current_list;
	char str[2048];
	strcpy(str, buf[0]);
	strcpy(str+strlen(buf[0]), buf[1]);
	unsigned int hashval = hash(hashtable, buf);


	/* Does item already exist? */
	current_list = lookup_string(hashtable, buf);
	/* item already exists, don't insert it again. */
	if (current_list != NULL){
		current_list->i++;
		strcpy(current_list->suffix[current_list->liczba_suf], buf[2]);
		current_list->liczba_suf++;
		return 5;
	}
	/* Attempt to allocate memory for list */
	if ((new_list = malloc(sizeof(list_t))) == NULL) return 1;
	/* Insert into list */

	strcpy(new_list->prefix[0], buf[0]);
	strcpy(new_list->prefix[1], buf[1]);
	strcpy(new_list->suffix[0], buf[2]);
	new_list->i = 1;
	new_list->liczba_suf = 1;
	new_list->next = hashtable->table[hashval];
	hashtable->table[hashval] = new_list;
	printf("%s   ", hashtable->table[hashval]->prefix[0]);
	printf("%s\n", hashtable->table[hashval]->prefix[1]);

	return 0;
}


/*
   void free_table(hash_table_t *hashtable)
   {
   int i;
   list_t *list, *temp;

   if (hashtable==NULL) return;

   for(i=0; i<hashtable->size; i++) {
   list = hashtable->table[i];
   while(list!=NULL) {
   temp = list;
   list = list->next;
   free(temp->str);
   free(temp);
   }
   }

   free(hashtable->table);
   free(hashtable);
   }
 */

/*
   int main() {


   hash_table_t *my_hash_table;
   int size_of_table = 100000;
   if(!(my_hash_table = create_hash_table(size_of_table)))
   return 5;

   return 0;
   }
 */
