#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>

struct SharedData {
    int *status;
    int *exit_flag;
    pthread_mutex_t mutex;
};

void sigint_handler(int signum) {
    // Récupérer la structure SharedData
    struct SharedData *shared_data = NULL;
    FILE *file = fopen("/tmp/shared_data", "r");
    if (file != NULL) {
        fread(&shared_data, sizeof(struct SharedData *), 1, file);
        fclose(file);
    }

    // Vérifier si la structure est valide et modifier les variables partagées
    if (shared_data != NULL) {
        pthread_mutex_lock(&shared_data->mutex);
        *(shared_data->status) = 1;
        *(shared_data->exit_flag) = 1;
        pthread_mutex_unlock(&shared_data->mutex);
    }
}

int main() {
    // Allocation dynamique pour la structure SharedData
    struct SharedData *shared_data = malloc(sizeof(struct SharedData));
    if (shared_data == NULL) {
        perror("Erreur lors de l'allocation de mémoire pour SharedData");
        exit(EXIT_FAILURE);
    }

    // Initialiser les variables dans la structure SharedData
    int status = 0;
    int exit_flag = 0;
    shared_data->status = &status;
    shared_data->exit_flag = &exit_flag;
    pthread_mutex_init(&shared_data->mutex, NULL);

    // Écrire la structure SharedData dans un fichier
    FILE *file = fopen("/tmp/shared_data", "w");
    if (file != NULL) {
        fwrite(&shared_data, sizeof(struct SharedData *), 1, file);
        fclose(file);
    }

    // Configuration du gestionnaire de signal pour SIGINT
    struct sigaction action;
    action.sa_handler = sigint_handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;

    if (sigaction(SIGINT, &action, NULL) == -1) {
        perror("Erreur lors de la configuration du gestionnaire de signal");
        exit(EXIT_FAILURE);
    }

    // Attendre la réception du signal
    while (!exit_flag) {
        printf("En attente d'une interruption...\n");
        sleep(1);
    }

    // Afficher le statut
    printf("Statut : %d\n", status);

    // Destruction du mutex
    pthread_mutex_destroy(&shared_data->mutex);

    // Libération de la mémoire allouée pour SharedData
    free(shared_data);

    // Suppression du fichier temporaire
    remove("/tmp/shared_data");

    return 0;
}


