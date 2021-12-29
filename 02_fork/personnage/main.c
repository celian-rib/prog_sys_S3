#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

struct Personnage {
    const char * nom;
    const char * phrase;
    int repetitions;
    int secondes;
};

struct Personnage personnages[] = {
    {"Dale Cooper", "Are you Laura Palmer?", 5, 2},
    {"Log Lady", "The owls were flying.", 3, 3},
    {"Gordon Cole", "WHAT?!", 10, 1}
};

void simuler_personnage(const struct Personnage *p) {
    printf("%s arrive.\n", p->nom);
    sleep(p->secondes);
    for (int i = 0; i < p->repetitions; i++) {
        printf("%s: %s\n", p->nom, p->phrase);
        sleep(p->secondes);
    }
    printf("%s s'en va.\n", p->nom);
}

int main() {
    printf("* début simulation\n");

	pid_t pids[3];

	for(int i = 0; i < 3; i ++) {
		pid_t p1 = fork();
		pids[i] = p1;

		if (p1 == 0) {
			simuler_personnage(&personnages[i]);
			return 0;
		}
	}

	for(int i = 0; i < 3; i ++)
		waitpid(pids[i], NULL, 0);

    printf("* fin simulation\n");
    return 0;
}
