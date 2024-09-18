#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* �w�q�`�I�����A */
typedef enum {
	SUSCEPTIBLE,   /* ���P�V */
	INFECTED,      /* �w�P�V */
	RECOVERED,     /* �w�d�_ */
	DEAD           /* �w���` */
} State;

/* �w�qgraph���`�I�����c */
typedef struct Node {
	State state;
	int id;
	int isClosed;    /* �O�_�b�ʳ����A */
	int recoveryDay; /* �d�_������A�Ω�l�ܦA�P�V */
	struct Node* next;
} Node;

/* �w�qgraph�����c */
typedef struct Graph {
	int numVertices;
	Node** adjLists;
} Graph;

/* �إߤ@�s�`�I����� */
Node* createNode(int id, State state, int isClosed) {
	Node* newNode = (Node*)malloc(sizeof(Node));
	newNode->id = id;
	newNode->state = state;
	newNode->isClosed = isClosed;
	newNode->recoveryDay = -1;  /* ��l�Ƭ�-1�A���ܥ��d�_ */
	newNode->next = NULL;
	return newNode;
}

/* �إߨ㦳'numVertices'���I��graph����� */
Graph* createGraph(int numVertices) {
	Graph* graph = (Graph*)malloc(sizeof(Graph));
	graph->numVertices = numVertices;
	graph->adjLists = (Node**)malloc(numVertices * sizeof(Node*));

	/* �N�F������l�Ƭ�NULL */
	for (int i = 0; i < numVertices; ++i) {
		graph->adjLists[i] = NULL;
	}

	return graph;
}

/* �Vgraph�K�[�@���䪺��� */
void addEdge(Graph* graph, int src, int dest) {
	/* �qsrc��dest�K�[�@���� */
	Node* newNode = createNode(dest, SUSCEPTIBLE, 0);
	newNode->next = graph->adjLists[src];
	graph->adjLists[src] = newNode;

	/* ���undirected graph�A�]�qdest��src�K�[�@���� */
	newNode = createNode(src, SUSCEPTIBLE, 0);
	newNode->next = graph->adjLists[dest];
	graph->adjLists[dest] = newNode;
}

/* ��l��graph�`�I���A����� */
void initializeGraph(Graph* graph, int* closedNodes, int closedCount) {
	for (int i = 0; i < graph->numVertices; ++i) {
		int isClosed = 0;

		for (int j = 0; j < closedCount; ++j) {
			if (i == closedNodes[j]) {
				isClosed = 1;
				break;
			}
		}

		graph->adjLists[i] = createNode(i, SUSCEPTIBLE, isClosed);
	}
}

/* �����e�f�Ǽ������ */
void spreadDisease(Graph* graph, int* initialInfectedNodes, int initialInfectedCount, float transmissionRate, float closedTransmissionRate, float recoveryRate, float reinfectionRate, float mortalityRate) {
	for (int i = 0; i < initialInfectedCount; ++i) {
		graph->adjLists[initialInfectedNodes[i]]->state = INFECTED;
	}

	int days = 0;
	int infectedCount = initialInfectedCount;

	while (infectedCount > 0) {
		int susceptibleCount = 0;
		int recoveredCount = 0;
		int deadCount = 0;
		infectedCount = 0;

		/* trace�C�Ӫ��A���`�I�� */
		for (int i = 0; i < graph->numVertices; ++i) {
			Node* vertex = graph->adjLists[i];
			switch (vertex->state) {
			case SUSCEPTIBLE: susceptibleCount++; break;
			case INFECTED: infectedCount++; break;
			case RECOVERED: recoveredCount++; break;
			case DEAD: deadCount++; break;
			}
		}

		printf("Day %d: SUSCEPTIBLE=%d, INFECTED=%d, RECOVERED=%d, DEAD=%d\n", days, susceptibleCount, infectedCount, recoveredCount, deadCount);

		/* trace�Ҧ����I�ç�s���A */
		for (int i = 0; i < graph->numVertices; ++i) {
			Node* vertex = graph->adjLists[i];
			if (vertex->state == INFECTED) {
				/* ���v�P�V�۾F�`�I */
				Node* temp = vertex->next;
				while (temp) {
					if (graph->adjLists[temp->id]->state == SUSCEPTIBLE) {
						float currentTransmissionRate = vertex->isClosed && graph->adjLists[temp->id]->isClosed ? closedTransmissionRate : transmissionRate;
						if ((float)rand() / RAND_MAX < currentTransmissionRate) {
							graph->adjLists[temp->id]->state = INFECTED;
						}
					}
					temp = temp->next;
				}
				/* �d�_�Φ��`���v */
				if ((float)rand() / RAND_MAX < mortalityRate) {
					vertex->state = DEAD;
				}
				else if ((float)rand() / RAND_MAX < recoveryRate) {
					vertex->state = RECOVERED;
					vertex->recoveryDay = days; /* �O���d�_����� */
				}
			}
			else if (vertex->state == RECOVERED && vertex->recoveryDay != -1) {
				/* �A�P�V���v */
				if (days - vertex->recoveryDay <= 50 && (float)rand() / RAND_MAX < reinfectionRate) {
					vertex->state = INFECTED;
					vertex->recoveryDay = -1;   /* ���m�d�_��� */
				}
			}
		}
		days++;
	}
}

int main() {
	srand(time(0));   /* ��l���H���ƥͦ��� */

	int numVertices = 77744;   /* �]�w�`�H��(��������ϤH��) */
	int initialInfectedCount = 100;   /* �]�w��l�P�V�H�� */
	int closedNodesCount = numVertices / 10;   /* ���]10%���`�I�B��ʳ����A */
	int* closedNodes = (int*)malloc(closedNodesCount * sizeof(int));   /* ���t�O����H�s�x�ʳ��`�I */
	int* initialInfectedNodes = (int*)malloc(initialInfectedCount * sizeof(int));   /* ���t�O����H�s�x��l�P�V�`�I */

	/* �H����ܫʳ����A���`�I */
	for (int i = 0; i < closedNodesCount; ++i) {
		closedNodes[i] = rand() % numVertices;
	}

	/* �H����ܪ�l�P�V���`�I */
	for (int i = 0; i < initialInfectedCount; ++i) {
		initialInfectedNodes[i] = rand() % numVertices;
	}

	Graph* graph = createGraph(numVertices);   /* �إߨ㦳���w�`�I�ƪ�graph */
	initializeGraph(graph, closedNodes, closedNodesCount);   /* ��l��graph���`�I���A */

	/* �K�[�@���� (�o�̬��F²�ơA�ڭ̰��]�C�Ӹ`�I�u�P�ּƴX�Ӭ۾F�`�I�s��) */
	for (int i = 0; i < numVertices; ++i) {
		addEdge(graph, i, (i + 1) % numVertices);
		if (i % 2 == 0) {
			addEdge(graph, i, (i + 2) % numVertices);
		}
	}
	 
	float transmissionRate = 0.03;       	
	float closedTransmissionRate = 0.1;  
	float recoveryRate = 0.01;          
	float reinfectionRate = 0.005;      
	float mortalityRate = 0.002;        

	spreadDisease(graph, initialInfectedNodes, initialInfectedCount, transmissionRate,
	 				closedTransmissionRate, recoveryRate, reinfectionRate, mortalityRate); 

	free(closedNodes);           
	free(initialInfectedNodes);  
	
	for (int i = 0; i < numVertices; ++i) {
		Node* node = graph->adjLists[i];
		while (node) {
			Node* temp = node;
			node = node->next;
			free(temp);
		}
	}
	free(graph->adjLists);  
	free(graph);            

	return 0;
}