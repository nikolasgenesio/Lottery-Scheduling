#ifndef SCHED_H
#define SCHED_H

#include "process.h"

#define MAX_NAME_LEN 4

typedef struct sched_info
{
        char name[MAX_NAME_LEN + 1];                    // nome do algoritmo
        void (*initParamsFn)(Process *p, void *params); // inicializar os parametros de escalonamento
        void (*notifyProcStatusChangeFn)(Process *p);   // notificar sobre a mudança de estado de um processo
        Process *(*scheduleFn)(Process *plist);         // decidir qual o proximo processo a obter a CPU
        int (*releaseParamsFn)(Process *p);             // liberar os parametros de escalonemnto
} SchedInfo;

/**
 * @brief Funcao para inicializar as informacoes sobre escalonadores
 *
 */
void schedInitSchedInfo(void);

/**
 * @brief Funcao que retorna as informacoes sobre escalonador de um slot
 *
 * @param slot solt
 * @return SchedInfo* ponteiro para acessar as informacoes
 */
SchedInfo *schedGetSchedInfo(int slot);

/**
 * @brief Funcao que notifica o algoritmo de escalonamento sobre a mudança de estado de um processo
 *
 * @param p processo
 */
void schedNotifyProcStatusChange(Process *p);

/**
 * @brief Funcao que aciona o escalonador de processos, que decide qual algoritmo deve ser usado
 *
 * @param plist processo
 * @return Process* ponteiro para o processo escolhido
 */
Process *schedSchedule(Process *plist);

/**
 * @brief Funcao que associa um processo a um algoritmo de escalonamento especifico
 *
 * @param p processo
 * @param params parametros
 * @param slot slot
 * @return int 1 caso o algoritmo de escalonamento seja encontrado e -1, caso contrario
 */
int schedSetScheduler(Process *p, void *params, int slot);

/**
 * @brief Funcao que registra um algoritmo de escalonamento
 *
 * @param sched_info
 * @return int numero do slot ocupado e -1, caso contrario
 */
int schedRegisterScheduler(SchedInfo *si);

/**
 * @brief Funcao que remove do escalonador um algoritmo de escalonamento
 *
 * @param slot slot
 * @param name nome do algoritmo
 * @return int numero do slot removido e -1, caso contrario
 */
int schedUnregisterScheduler(int slot, char *name);

#endif
