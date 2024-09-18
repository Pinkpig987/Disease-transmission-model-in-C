#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* 定義節點的狀態 */
typedef enum {
	SUSCEPTIBLE,   /* 易感染 */
	INFECTED,      /* 已感染 */
	RECOVERED,     /* 已康復 */
	DEAD,          /* 已死亡 */
	VACCINATED     /* 已接種疫苗 */
} State;

/* 定義graph中節點的結構 */
typedef struct Node {
	State state;
	int id;
	int isClosed;    /* 是否在封閉狀態 */
	int recoveryDay; /* 康復的日期，用於追蹤再感染 */
	struct Node* next;
} Node;

/* 定義graph的結構 */
typedef struct Graph {
	int numVertices;
	Node** adjLists;
} Graph;

/* 建立一新節點的函數 */
Node* createNode(int id, State state, int isClosed) {
	Node* newNode = (Node*)malloc(sizeof(Node));
	newNode->id = id;
	newNode->state = state;
	newNode->isClosed = isClosed;
	newNode->recoveryDay = -1;  /* 初始化為-1，表示未康復 */
	newNode->next = NULL;
	return newNode;
}

/* 建立具有'numVertices'頂點的graph的函數 */
Graph* createGraph(int numVertices) {
	Graph* graph = (Graph*)malloc(sizeof(Graph));
	graph->numVertices = numVertices;
	graph->adjLists = (Node**)malloc(numVertices * sizeof(Node*));

	/* 將鄰接表初始化為NULL */
	for (int i = 0; i < numVertices; ++i) {
		graph->adjLists[i] = NULL;
	}

	return graph;
}

/* 向graph添加一個邊的函數 */
void addEdge(Graph* graph, int src, int dest) {
	/* 從src到dest添加一個邊 */
	Node* newNode = createNode(dest, SUSCEPTIBLE, 0);
	newNode->next = graph->adjLists[src];
	graph->adjLists[src] = newNode;

	/* 對於undirected graph，也從dest到src添加一個邊 */
	newNode = createNode(src, SUSCEPTIBLE, 0);
	newNode->next = graph->adjLists[dest];
	graph->adjLists[dest] = newNode;
}

/* 初始化graph節點狀態的函數 */
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

/* 初始化接種疫苗的節點 */
void initializeVaccination(Graph* graph, int* vaccinatedNodes, int vaccinatedCount) {
	for (int i = 0; i < vaccinatedCount; ++i) {
		graph->adjLists[vaccinatedNodes[i]]->state = VACCINATED;
	}
}

/* 模擬疾病傳播的函數，考慮疫苗的有效性 */
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

		/* 計數每個狀態的節點數 */
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

		/* trace所有頂點並更新狀態 */
		for (int i = 0; i < graph->numVertices; ++i) {
			Node* vertex = graph->adjLists[i];
			if (vertex->state == INFECTED) {
				/* 感染相鄰節點 */
				Node* temp = vertex->next;
				while (temp) {
					/* 在 spreadDisease 函數中考慮接種疫苗的節點 */
					if (graph->adjLists[temp->id]->state == SUSCEPTIBLE || graph->adjLists[temp->id]->state == VACCINATED) {
						float currentTransmissionRate = (vertex->isClosed && graph->adjLists[temp->id]->isClosed) ? closedTransmissionRate : transmissionRate;
						if (graph->adjLists[temp->id]->state == VACCINATED) {
							currentTransmissionRate *= (1 - vaccineEfficacy); /* 考慮疫苗有效性 */
						}
						if ((float)rand() / RAND_MAX < currentTransmissionRate) {
							graph->adjLists[temp->id]->state = INFECTED;
						}
					}
					temp = temp->next;
				}
				/* 康復或死亡機率 */
				if ((float)rand() / RAND_MAX < mortalityRate) {
					vertex->state = DEAD;
				}
				else if ((float)rand() / RAND_MAX < recoveryRate) {
					vertex->state = RECOVERED;
					vertex->recoveryDay = days; /* 記錄康復的日期 */
				}
			}
			else if (vertex->state == RECOVERED && vertex->recoveryDay != -1) {
				/* 再感染機率 */
				if (days - vertex->recoveryDay <= 50 && (float)rand() / RAND_MAX < reinfectionRate) {
					vertex->state = INFECTED;
					vertex->recoveryDay = -1;   /* 重置康復日期 */
				}
			}
			/* 在trace節點時考慮 SUSCEPTIBLE 節點轉變為 VACCINATED */
			else if (vertex->state == SUSCEPTIBLE) {
				/* SUSCEPTIBLE 節點變為 VACCINATED */
				if ((float)rand() / RAND_MAX < vaccinationRate) {
					vertex->state = VACCINATED;
				}
			}
		}
		days++;
	}
}

int main() {
	srand(time(0));   /* 初始化隨機數生成器 */

	int numVertices = 77744;   /* 設定總人數(此表中西區人數) */
	int initialInfectedCount = 100;   /* 設定初始感染人數 */
	int closedNodesCount = numVertices / 10;   /* 假設10%的節點處於封閉狀態 */
	int vaccinatedCount = 10000; /* 假設接種了10000人 */
	int* closedNodes = (int*)malloc(closedNodesCount * sizeof(int));   /* 分配記憶體以存儲封閉節點 */
	int* initialInfectedNodes = (int*)malloc(initialInfectedCount * sizeof(int));   /* 分配記憶體以存儲初始感染節點 */
	int* vaccinatedNodes = (int*)malloc(vaccinatedCount * sizeof(int)); /* 分配記憶體以存儲接種疫苗節點 */

	/* 隨機選擇封閉狀態的節點 */
	for (int i = 0; i < closedNodesCount; ++i) {
		closedNodes[i] = rand() % numVertices;
	}

	/* 隨機選擇初始感染的節點 */
	for (int i = 0; i < initialInfectedCount; ++i) {
		initialInfectedNodes[i] = rand() % numVertices;
	}

	/* 隨機選擇接種疫苗的節點 */
	for (int i = 0; i < vaccinatedCount; ++i) {
		vaccinatedNodes[i] = rand() % numVertices;
	}

	Graph* graph = createGraph(numVertices);   /* 建立具有指定節點數的graph */
	initializeGraph(graph, closedNodes, closedNodesCount);   /* 初始化graph的節點狀態 */
	initializeVaccination(graph, vaccinatedNodes, vaccinatedCount); /* 初始化接種疫苗的節點 */

	/* 添加一些邊 (這裡為了簡化，我們假設每個節點只與少數幾個相鄰節點連接) */
	for (int i = 0; i < numVertices; ++i) {
		addEdge(graph, i, (i + 1) % numVertices);
		if (i % 2 == 0) {
			addEdge(graph, i, (i + 2) % numVertices);
		}
	}

	float transmissionRate = 0.03;       /* 進行疾病傳播的初始感染率 */
	float closedTransmissionRate = 0.1;  /* 封閉狀態的感染率(更高) */
	float recoveryRate = 0.01;           /* 康復率為1% */
	float reinfectionRate = 0.005;       /* 再感染率為0.5% */
	float mortalityRate = 0.002;         /* 病死率為0.2% */
	float vaccineEfficacy = 0.95;        /* 設置疫苗有效性為95% */
	float vaccinationRate = 0.01;        /* 易感染節點變為接種疫苗節點的機率為1% */

	spreadDisease(graph, initialInfectedNodes, initialInfectedCount, transmissionRate, closedTransmissionRate, recoveryRate, reinfectionRate, mortalityRate, vaccineEfficacy, vaccinationRate); /* 模擬疾病傳播 */

	free(closedNodes);           /* 釋放封閉節點記憶體 */
	free(initialInfectedNodes);  /* 釋放初始感染節點記憶體 */
	free(vaccinatedNodes);       /* 釋放接種疫苗節點記憶體 */

	/* 釋放graph的內存 */
	for (int i = 0; i < numVertices; ++i) {
		Node* node = graph->adjLists[i];
		while (node) {
			Node* temp = node;
			node = node->next;
			free(temp);
		}
	}
	free(graph->adjLists);  /* 釋放graph的鄰接表 */
	free(graph);            /* 釋放graph的結構 */

	return 0;
}