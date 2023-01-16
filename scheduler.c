#include <stdio.h>
#include <string.h>
#include "scheduler.h"

#define MAX_NUM_SLOT 4

// Slots de registro de escalonadores
SchedInfo *sched_slots[MAX_NUM_SLOT];

/**
 * @brief Funcao para inicializar as informacoes sobre escalonadores
 *
 */
void schedInitSchedInfo(void)
{
	int i;
	// Inicializar slots de registro de escaloandores
	for (i = 0; i < MAX_NUM_SLOT; i++)
		sched_slots[i] = NULL;
}

/**
 * @brief Funcao que retorna as informacoes sobre escalonador de um slot
 *
 * @param slot solt
 * @return SchedInfo* ponteiro para acessar as informacoes
 */
SchedInfo *schedGetSchedInfo(int slot)
{
	if (slot >= 0 && slot <= MAX_NUM_SLOT)
		return sched_slots[slot];
	else
		return NULL;
}

/**
 * @brief Funcao que notifica o algoritmo de escalonamento sobre a mudança de estado de um processo
 *
 * @param p processo
 */
void schedNotifyProcStatusChange(Process *p)
{
	int slot = processGetSchedSlot(p);		// slot do processo
	SchedInfo *sched = sched_slots[slot];	// realoca o ponteiro
	if (sched != NULL)						// se eh valido
		sched->notifyProcStatusChangeFn(p); // notifica
}

/**
 * @brief Funcao que aciona o escalonador de processos, que decide qual algoritmo deve ser usado
 *
 * @param plist processo
 * @return Process* ponteiro para o processo escolhido
 */
Process *schedSchedule(Process *plist)
{
	Process *newp, *oldp; // controle

	// se tiver algum processo em execucao, colocar como pronto
	if (oldp = processGetByStatus(plist, PROC_RUNNING))
	{
		processSetStatus(oldp, PROC_READY);
	}

	// decisao de escalonamento ao algoritmo registrado no primeiro slot
	if (sched_slots[0] == NULL)
		return NULL;
	newp = sched_slots[0]->scheduleFn(plist);

	// Colocar processo escolhido como RUNNING
	if (newp)
	{
		processSetStatus(newp, PROC_RUNNING);
		processAddCpuUsage(newp, 1);
	}

	return newp; // retornar
}

/**
 * @brief Funcao que associa um processo a um algoritmo de escalonamento especifico
 *
 * @param p processo
 * @param params parametros
 * @param slot slot
 * @return int 1 caso o algoritmo de escalonamento seja encontrado e -1, caso contrario
 */
int schedSetScheduler(Process *p, void *params, int slot)
{
	int oldslot;

	if (sched_slots[slot] == NULL) // verifica se slot eh valido
		return -1;

	oldslot = processGetSchedSlot(p);

	if (oldslot >= 0) // libera parametros de escalonamento antigos
		sched_slots[oldslot]->releaseParamsFn(p);

	processSetSchedSlot(p, slot); // associa processo ao slot informado

	processSetSchedParams(p, params); // inicializar parametros de escalonamento com os parametros informados
	return 1;
}

/**
 * @brief Funcao que registra um algoritmo de escalonamento
 *
 * @param sched_info
 * @return int numero do slot ocupado e -1, caso contrario
 */
int schedRegisterScheduler(SchedInfo *sched_info)
{
	int i;

	// laco para encontrar slot livre
	for (i = 0; i < MAX_NUM_SLOT && sched_slots[i] != NULL; i++)
		;
	if (i == MAX_NUM_SLOT)
		return -1;

	sched_slots[i] = sched_info; // atribuir ao slot a estrutura com informacoes do novo escalonador

	return i;
}

/**
 * @brief Funcao que remove do escalonador um algoritmo de escalonamento
 *
 * @param slot slot
 * @param name nome do algoritmo
 * @return int numero do slot removido e -1, caso contrario
 */
int schedUnregisterScheduler(int slot, char *name)
{
	if (!sched_slots[slot] || strcmp(name, sched_slots[slot]->name)) // verifica se slot informado eh valido
		return -1;

	sched_slots[slot] = NULL; // forca valor de slot para NULL
	return slot;
}
