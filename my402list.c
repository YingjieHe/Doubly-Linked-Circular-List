#include <stdio.h>
#include <stdlib.h>

#include "cs402.h"
#include "my402list.h"

int  My402ListLength(My402List *pList){
	return pList->num_members;
}

int  My402ListEmpty(My402List *pList){
	return pList->num_members == 0;
}

int  My402ListAppend(My402List *pList, void *obj){
	My402ListElem *elem = (My402ListElem *)malloc(sizeof(My402ListElem));
	if(elem == NULL) return FALSE;
	elem->obj = obj;
	if(My402ListEmpty(pList)){
		pList->anchor.next = elem;
		pList->anchor.prev = elem;
		elem->prev = &pList->anchor;
		elem->next = &pList->anchor;
	}
	else{
		elem->prev = pList->anchor.prev;
		elem->next = &pList->anchor;
		pList->anchor.prev->next = elem;
		pList->anchor.prev = elem;
	}
	pList->num_members++;
	return TRUE;
}
int  My402ListPrepend(My402List *pList, void *obj){
	My402ListElem *elem = (My402ListElem *)malloc(sizeof(My402ListElem));
	if(elem == NULL) return FALSE;
	elem->obj = obj;
	if(My402ListEmpty(pList)){
		pList->anchor.next = elem;
		pList->anchor.prev = elem;
		elem->prev = &pList->anchor;
		elem->next = &pList->anchor;
	}
	else{
		elem->prev = &pList->anchor;
		elem->next = pList->anchor.next;
		pList->anchor.next->prev = elem;
		pList->anchor.next = elem;
	}
	pList->num_members++;
	return TRUE;
}
void My402ListUnlink(My402List *pList, My402ListElem *elem){
	if(elem == NULL || My402ListEmpty(pList)) return;
	elem->prev->next = elem->next;
	elem->next->prev = elem->prev;
	free(elem);
	pList->num_members--;
}
void My402ListUnlinkAll(My402List *pList){
	My402ListElem *elem = NULL;
	for(elem = My402ListFirst(pList); elem != NULL; elem = My402ListNext(pList, elem)){
    	My402ListUnlink(pList, elem);
    }
	pList->num_members = 0;
}
int  My402ListInsertAfter(My402List *pList, void *obj, My402ListElem *elem){
	if(elem == NULL) return My402ListAppend(pList, obj);
	else{
		My402ListElem *temp = (My402ListElem *)malloc(sizeof(My402ListElem));
		if(temp == NULL) return FALSE;
		temp->obj = obj;
		temp->next = elem->next;
		temp->prev = elem;
		elem->next->prev = temp;
		elem->next = temp;
	}
	pList->num_members++;
	return TRUE;
}
int  My402ListInsertBefore(My402List *pList, void *obj, My402ListElem *elem){
	if(elem == NULL) return My402ListPrepend(pList, obj);
	else{
		My402ListElem *temp = (My402ListElem *)malloc(sizeof(My402ListElem));
		if(temp == NULL) return FALSE;
		temp->obj = obj;
		temp->next = elem;
		temp->prev = elem->prev;
		elem->prev->next = temp;
		elem->prev = temp;
	}
	pList->num_members++;
	return TRUE;
}

My402ListElem *My402ListFirst(My402List *pList){
	if(My402ListEmpty(pList)){
		return NULL;
	}
	else{
		return pList->anchor.next;
	}
}
My402ListElem *My402ListLast(My402List *pList){
	if(My402ListEmpty(pList)){
		return NULL;
	}
	else{
		return pList->anchor.prev;
	}
}
My402ListElem *My402ListNext(My402List *pList, My402ListElem *elem){
	if(pList->anchor.prev == elem){
		return NULL;
	}
	else return elem->next;
}
My402ListElem *My402ListPrev(My402List *pList, My402ListElem *elem){
	if(pList->anchor.next == elem){
		return NULL;
	}
	else return elem->prev;
}

My402ListElem *My402ListFind(My402List *pList, void *obj){
	My402ListElem *elem = NULL;
    for (elem = My402ListFirst(pList); elem != NULL; elem = My402ListNext(pList, elem)){
    	if(elem->obj == obj) return elem;
    }
    return NULL;
}

int My402ListInit(My402List *pList){
	pList->num_members = 0;
	pList->anchor.prev = NULL;
	pList->anchor.next = NULL;
	pList->anchor.obj = NULL;
	return TRUE;
}
