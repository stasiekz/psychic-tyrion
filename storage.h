#ifndef CHUJ
#define CHUJ

typedef struct _list_t_ {

	char prefix[2][1024];
	char suffix[1000][1024];
	int liczba_suf;
	int i;
	struct _list_t_ *next;

} list_t;

typedef struct _hash_table_t_ {
	int size;       /* the size of the table */
	list_t **table; /* the table elements */
} hash_table_t;


hash_table_t *create_hash_table(int size);

unsigned int hash(hash_table_t *hashtable, char **buf);

list_t *lookup_string(hash_table_t *hashtable, char **buf);

int add_string(hash_table_t *hashtable, char **buf);


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
#endif
