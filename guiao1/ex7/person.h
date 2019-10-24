
#define MAX_NAME_SIZE		50
#define NAME_FIELD_SIZE		MAX_NAME_SIZE + 1
#define FILE_NAME			"pessoas.txt"

typedef struct person {
	char name[MAX_NAME_SIZE];
	int age;
} Person;

Person* new_person(char* name, int age);
Person* clone_person(Person* p);
void destroy_person(Person* p);
int person_age(Person* p);
void person_change_age(Person* p, int age);
void save_person(Person* p);
void update_age_and_persist(char* name, int age);
