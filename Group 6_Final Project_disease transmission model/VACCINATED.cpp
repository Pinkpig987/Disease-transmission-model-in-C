#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* �w�q�`�I�����A */
typedef enum {
	SUSCEPTIBLE,   /* ���P�V */
	INFECTED,      /* �w�P�V */
	RECOVERED,     /* �w�d�_ */
	DEAD,          /* �w���` */
	VACCINATED     /* �w���ج̭] */
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
	newNode->recoveryDay = -1;  /* ��l�Ƭ�-1�A��ܥ��d�_ */
	newNode->next = NULL;
	return newNode;
}

/* �إߨ㦳'numVertices'���I��graph����� */
Graph* createGraph(int numVertices) {
	Graph* graph = (Graph*)malloc(sizeof(Graph));
	graph->numVertices = numVertices;
	graph->adjLists = (Node**)malloc(numVertices * sizeof(Node*));

	/* �N�F�����l�Ƭ�NULL */
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

/* ��l�Ʊ��ج̭]���`�I */
void initializeVaccination(Graph* graph, int* vaccinatedNodes, int vaccinatedCount) {
	for (int i = 0; i < vaccinatedCount; ++i) {
		graph->adjLists[vaccinatedNodes[i]]->state = VACCINATED;
	}
}

/* �����e�f�Ǽ�����ơA�Ҽ{�̭]�����ĩ� */
void spreadDisease(Graph* graph, int* initialInfectedNodes, int initialInfectedCount, float transmissionRate, float closedTransmissionRate,
	float recoveryRate, float reinfectionRate, float mortalityRate, float vaccineEfficacy, float vaccinationRate) {
	for (int i = 0; i < initialInfectedCount; ++i) {
		graph->adjLists[initialInfectedNodes[i]]->state = INFECTED;
	}

	int days = 0;
	int infectedCount = initialInfectedCount;

	while (infectedCount > 0) {
		int susceptibleCount = 0;
		int recoveredCount = 0;
		int deadCount = 0;
		int vaccinatedCount = 0;
		infectedCount = 0;

		/* �p�ƨC�Ӫ��A���`�I�� */
		for (int i = 0; i < graph->numVertices; ++i) {
			Node* vertex = graph->adjLists[i];
			switch (vertex->state) {
			case SUSCEPTIBLE: susceptibleCount++; break;
			case INFECTED: infectedCount++; break;
			case RECOVERED: recoveredCount++; break;
			case DEAD: deadCount++; break;
			case VACCINATED: vaccinatedCount++; break;
			}
		}

		printf("Day %d: SUSCEPTIBLE=%d, INFECTED=%d, RECOVERED=%d, DEAD=%d, VACCINATED=%d\n", days, susceptibleCount, infectedCount, recoveredCount, deadCount, vaccinatedCount);

		/* trace�Ҧ����I�ç�s���A */
		for (int i = 0; i < graph->numVertices; ++i) {
			Node* vertex = graph->adjLists[i];
			if (vertex->state == INFECTED) {
				/* �P�V�۾F�`�I */
				Node* temp = vertex->next;
				while (temp) {
					/* �b spreadDisease ��Ƥ��Ҽ{���ج̭]���`�I */
					if (graph->adjLists[temp->id]->state == SUSCEPTIBLE || graph->adjLists[temp->id]->state == VACCINATED) {
						float currentTransmissionRate = (vertex->isClosed && graph->adjLists[temp->id]->isClosed) ? closedTransmissionRate : transmissionRate;
						if (graph->adjLists[temp->id]->state == VACCINATED) {
							currentTransmissionRate *= (1 - vaccineEfficacy); /* �Ҽ{�̭]���ĩ� */
						}
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
			/* �btrace�`�I�ɦҼ{ SUSCEPTIBLE �`�I���ܬ� VACCINATED */
			else if (vertex->state == SUSCEPTIBLE) {
				/* SUSCEPTIBLE �`�I�ܬ� VACCINATED */
				if ((float)rand() / RAND_MAX < vaccinationRate) {
					vertex->state = VACCINATED;
				}
			}
		}
		days++;
	}
}

int main() {
	srand(time(0));   /* ��l���H���ƥͦ��� */

	int numVertices = 77744;   /* �]�w�`�H��(������ϤH��) */
	int initialInfectedCount = 100;   /* �]�w��l�P�V�H�� */
	int closedNodesCount = numVertices / 10;   /* ���]10%���`�I�B��ʳ����A */
	int vaccinatedCount = 10000; /* ���]���ؤF10000�H */
	int* closedNodes = (int*)malloc(closedNodesCount * sizeof(int));   /* ���t�O����H�s�x�ʳ��`�I */
	int* initialInfectedNodes = (int*)malloc(initialInfectedCount * sizeof(int));   /* ���t�O����H�s�x��l�P�V�`�I */
	int* vaccinatedNodes = (int*)malloc(vaccinatedCount * sizeof(int)); /* ���t�O����H�s�x���ج̭]�`�I */

	/* �H����ܫʳ����A���`�I */
	for (int i = 0; i < closedNodesCount; ++i) {
		closedNodes[i] = rand() % numVertices;
	}

	/* �H����ܪ�l�P�V���`�I */
	for (int i = 0; i < initialInfectedCount; ++i) {
		initialInfectedNodes[i] = rand() % numVertices;
	}

	/* �H����ܱ��ج̭]���`�I */
	for (int i = 0; i < vaccinatedCount; ++i) {
		vaccinatedNodes[i] = rand() % numVertices;
	}

	Graph* graph = createGraph(numVertices);   /* �إߨ㦳���w�`�I�ƪ�graph */
	initializeGraph(graph, closedNodes, closedNodesCount);   /* ��l��graph���`�I���A */
	initializeVaccination(graph, vaccinatedNodes, vaccinatedCount); /* ��l�Ʊ��ج̭]���`�I */

	/* �K�[�@���� (�o�̬��F²�ơA�ڭ̰��]�C�Ӹ`�I�u�P�ּƴX�Ӭ۾F�`�I�s��) */
	for (int i = 0; i < numVertices; ++i) {
		addEdge(graph, i, (i + 1) % numVertices);
		if (i % 2 == 0) {
			addEdge(graph, i, (i + 2) % numVertices);
		}
	}

	float transmissionRate = 0.03;       /* �i��e�f�Ǽ�����l�P�V�v */
	float closedTransmissionRate = 0.1;  /* �ʳ����A���P�V�v(��) */
	float recoveryRate = 0.01;           /* �d�_�v��1% */
	float reinfectionRate = 0.005;       /* �A�P�V�v��0.5% */
	float mortalityRate = 0.002;         /* �f���v��0.2% */
	float vaccineEfficacy = 0.95;        /* �]�m�̭]���ĩʬ�95% */
	float vaccinationRate = 0.01;        /* ���P�V�`�I�ܬ����ج̭]�`�I�����v��1% */

	spreadDisease(graph, initialInfectedNodes, initialInfectedCount, transmissionRate, closedTransmissionRate, recoveryRate, reinfectionRate, mortalityRate, vaccineEfficacy, vaccinationRate); /* �����e�f�Ǽ� */

	free(closedNodes);           /* ����ʳ��`�I�O���� */
	free(initialInfectedNodes);  /* �����l�P�V�`�I�O���� */
	free(vaccinatedNodes);       /* ���񱵺ج̭]�`�I�O���� */

	/* ����graph�����s */
	for (int i = 0; i < numVertices; ++i) {
		Node* node = graph->adjLists[i];
		while (node) {
			Node* temp = node;
			node = node->next;
			free(temp);
		}
	}
	free(graph->adjLists);  /* ����graph���F���� */
	free(graph);            /* ����graph�����c */

	return 0;
}