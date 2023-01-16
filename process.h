#ifndef PROCESS_H
#define PROCESS_H

#include <stdlib.h>

#define PROC_INITIALIZING 0 // inicializando
#define PROC_WAITING 2      // aguardando
#define PROC_READY 4        // pronto
#define PROC_RUNNING 8      // executando
#define PROC_TERMINATING 16 // terminado

typedef struct proc Process;

/**
 * @brief Funcao que retorna o PID (identificador do Processo) de um processo
 *
 * @param p processo
 * @return int PID
 */
int processGetPid(Process *p);

/**
 * @brief Funcao que retorna o PPID (identificador do Processo Pai) de um processo
 *
 * @param p processo
 * @return int PPID
 */
int processGetParentPid(Process *p);

/**
 * @brief Funcao que retorna o status atual de um processo
 *
 * @param p processo
 * @return int status
 */
int processGetStatus(Process *p);

/**
 * @brief Funcao que retorna o tempo de CPU usado por um processo
 *
 * @param p processo
 * @return int tempo de CPU usado
 */
int processGetCpuUsage(Process *p);

/**
 * @brief Funcao que retorna o ponteiro para os parametros de escalonamento
 *
 * @param p processo
 * @return void* ponteiro para os parametros
 */
void *processGetSchedParams(Process *p);

/**
 * @brief Funcao que retorna o slot ocupado pelo algoritmo de escalonamento
 *
 * @param p processo
 * @return int slot ocupado
 */
int processGetSchedSlot(Process *p);

/**
 * @brief Funcao que aponta o ponteiro para o proximo processo da lista
 *
 * @param p processo
 * @return Process* ponteiro para o proximo processo
 */
Process *processGetNext(Process *p);

/**
 * @brief Funcao que aponta o ponteiro para o processo anterior da lista
 *
 * @param p processo
 * @return Process* ponteiro para o processo anterior
 */
Process *processGetPrev(Process *p);

/**
 * @brief Funcao que altera o valor do identificador do processo pai de um processo
 *
 * @param p processo
 * @param ppid identificador do processo pai
 * @return int identificador do processo que foi alterado
 */
int processSetParentPid(Process *p, int ppid);

/**
 * @brief Funcao que altera o status de um processo
 *
 * @param p processo
 * @param status status do processo
 * @return int identificador do processo
 */
int processSetStatus(Process *p, int status);

/**
 * @brief Funcao que adiciona o tempo da CPU
 *
 * @param p processo
 * @param add tempo para ser adicionado
 * @return int tempo atualizado
 */
int processAddCpuUsage(Process *p, int add);

/**
 * @brief Funcao que redireciona ponteiro de parametros de escalonamento para uma estrutura
 *
 * @param p processo
 * @param sp estrutura
 */
void processSetSchedParams(Process *p, void *sp);

/**
 * @brief Funcao que altera o escalonamento por meio do numero do slot associado
 *
 * @param p processo
 * @param slot numero do slot
 */
void processSetSchedSlot(Process *p, int slot);

/**
 * @brief Funcao que retorna um processo atraves do seu identificador
 *
 * @param plist processo
 * @param pid identificador do processo
 * @return Process* processo
 */
Process *processGetByPid(Process *plist, int pid);

/**
 * @brief Funcao que retorna um processo a partir do seu status
 *
 * @param plist processo
 * @param status status
 * @return Process* processo
 */
Process *processGetByStatus(Process *plist, int status);

/**
 * @brief Funcao que retorna um processo a partir de um algoritmo de escalonamento que eh identificado pelo seu slot
 *
 * @param plist processo
 * @param slot slot
 * @return Process* processo
 */
Process *processGetBySchedSlot(Process *plist, int slot);

/**
 * @brief Funcao que cria um processo no inicio da lista
 *
 * @param plist processo
 * @return Process* processo do inicio
 */
Process *processCreate(Process *plist);

/**
 * @brief Funcao que remover um processo atraves do PID
 *
 * @param plist processo
 * @param pid identificador do processo
 * @return Process* processo do inicio
 */
Process *processDestroy(Process *plist, int pid);

/**
 * @brief Funcao que imprime a lista de processo
 *
 * @param plist processo
 * @param dumpSchedParamsFn parametros do processo
 */
void printProcess(Process *plist, void (*dumpSchedParamsFn)(Process *pid));

#endif
