#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

struct point {
    double x;
    double y;
};

struct quadrilateral {
    struct point v[4];
};

struct triangle {
    struct point v1, v2, v3;
};

struct ListNode {
    struct quadrilateral vertices;
    double area;
    struct ListNode* next;
};

double triangle_area (struct triangle t) {
    double comp1 = t.v1.x * (t.v2.y - t.v3.y);
    double comp2 = t.v2.x * (t.v3.y - t.v1.y);
    double comp3 = t.v3.x * (t.v1.y - t.v2.y);

    double area = (comp1 + comp2 + comp3) / 2;

    return fabs(area);
}

double quad_area (struct quadrilateral q) {

    struct triangle t1 = {q.v[0], q.v[1], q.v[2]};
    struct triangle t2 = {q.v[1], q.v[2], q.v[3]};
    struct triangle t3 = {q.v[2], q.v[3], q.v[0]};
    struct triangle t4 = {q.v[3], q.v[0], q.v[1]};

    double a1 = triangle_area(t1), a2 = triangle_area(t2), a3 = triangle_area(t3);
    double a4 = triangle_area(t4);

    double min = fmin(a1, a2) < fmin(a3, a4) ? fmin(a1, a2) : fmin(a3, a4);
    double max = fmax(a1, a2) > fmax(a3, a4) ? fmax(a1, a2) : fmax(a3, a4);

    return a1 + a2 + a3 + a4 - max - min;

}

struct quadrilateral scan_q () {
    struct point v1, v2, v3, v4;

    printf("Introduceti varfurile patrulaterului: \n");
    printf("v1 : ");
    scanf("%lf%lf", &v1.x, &v1.y);
    printf("v2 : ");
    scanf("%lf%lf", &v2.x, &v2.y);
    printf("v3 : ");
    scanf("%lf%lf", &v3.x, &v3.y);
    printf("v4 : ");
    scanf("%lf%lf", &v4.x, &v4.y);

    struct quadrilateral q = {v1, v2, v3, v4};

    return q;
}

void print_q (struct quadrilateral q) {
    printf("Patrulater cu aria %lf: \n", quad_area(q));
    printf("v1 :(%lf, %lf)\n", q.v[0].x, q.v[0].y);
    printf("v2 :(%lf, %lf)\n", q.v[1].x, q.v[1].y);
    printf("v3 :(%lf, %lf)\n", q.v[2].x, q.v[2].y);
    printf("v4 :(%lf, %lf)\n", q.v[3].x, q.v[3].y);

}

/* -------------------     LABORATORUL 8      ---------------------*/

void printList(struct ListNode* head) {
    while (head) {
        print_q(head->vertices);
        printf("     |    \n");
        printf("     |    \n");
        printf("     |    \n");
        printf("    \\/   \n");
        head = head->next;
    }
    printf("    NIL   \n");
}

void destroyList(struct ListNode** head) {
    struct ListNode* current = *head;
    struct ListNode* next;

    while (current) {
        next = current->next;
        free(current);
        current = next;
    }

    *head = NULL;
}

void insertListNode (struct ListNode** head, struct ListNode* toInsert) {
    struct ListNode* temp = *head;
    double currentArea = toInsert->area;

    if (!*head) {
        *head = toInsert;
    } else if ((*head)->area > currentArea) {
        toInsert->next = temp;
        *head = toInsert;
    } else {
        while (temp->next && temp->next->area < currentArea)
            temp = temp->next;
        // the node will be inserted between temp and temp->next
        toInsert->next = temp->next;
        temp->next = toInsert;
    }
}

void printNodeByPosition (struct ListNode* head, int position) {
    if (!head)
        return;
    while (head && --position)
        head = head->next;

    print_q(head->vertices);

}

void deleteNodeByPosition (struct ListNode** head, int position) {
    position--;
    struct ListNode* temp = *head;
    if (!position) {
        *head = (*head)->next;
    } else {
        while (--position)
            temp = temp->next;

        temp->next = temp->next->next;
    }
}

bool comparePairs(struct point p1, struct point p2) {
    return p1.x > p2.x || p2.x <= p1.x && p1.y > p2.y;
}

bool checkEqualPairs(struct point p1, struct point p2) {
    return p1.x == p2.x && p1.y == p2.y;
}

// sort by x coordinate first, then by y
void sortByIncreasingPairs(struct quadrilateral* q) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 3 - i; j++)
            if (comparePairs(q->v[j], q->v[j + 1])) {
                struct point temp = q->v[j];
                q->v[j] = q->v[j + 1];
                q->v[j + 1] = temp;
            }

}

bool compareQuadrilaterals(struct quadrilateral q1, struct quadrilateral q2) {
    sortByIncreasingPairs(&q1);
    sortByIncreasingPairs(&q2);

    for (int i = 0; i < 4; i++)
        if (!checkEqualPairs(q1.v[i], q2.v[i]))
            return false;
    return true;
}

void deleteNodeByContent (struct ListNode** head, struct quadrilateral q) {
    int currentPosition = 1; // index to head

    struct ListNode* temp = *head;
    if (!*head)
        return;

    while (temp) {
        if (compareQuadrilaterals(temp->vertices, q))
            deleteNodeByPosition(head, currentPosition);

        currentPosition++;
        temp = temp->next;
    }

}

void start() {
    bool flag = true;
    struct ListNode* head = NULL;

    while (flag) {
        printf("Choose one of the following options: \n");
        printf("1. Insert node. \n");
        printf("2. Delete nose at specified index. \n");
        printf("3. Delete node(s) with specified content. \n");
        printf("4. Iterate through list. \n");
        printf("5. Display node at specified position. \n");
        printf("6. Exit. \n");

        int input;
        scanf("%d", &input);

        switch (input) {
            case 1:
            {
                struct quadrilateral q = scan_q();
                struct ListNode node = {q, quad_area(q), NULL};
                insertListNode(&head, &node);
            } break;

            case 2:
            {
                int index;
                printf("Enter index: \n");
                scanf("%d", &index);
                deleteNodeByPosition(&head, index);
            } break;

            case 3:
            {
                struct quadrilateral q = scan_q();
                deleteNodeByContent(&head, q);
            } break;

            case 4: {
                printList(head);
            } break;

            case 5: {
                int index;
                printf("Enter index: \n");
                scanf("%d", &index);
                printNodeByPosition(head, index);
            } break;

            case 6: {
                flag = false;
            }
            default:
                printf("Please enter a valid input. \n");
        }
    }

}


int main () {

    struct quadrilateral q1 = { {{0,0}, {1, 2}, {4,0}, {0, 4} } };
    struct quadrilateral q2 = {{{0,0}, {1, 0}, {1, 1}, {0, 1} } };
    struct quadrilateral q3 = {{{0,0}, {2, 0}, {2, 2}, {0, 2} } };
    struct quadrilateral q4 = { { {0, 0}, {0, 0},{0, 0},{0, 0} } };

    struct quadrilateral q11 =  { {{1, 2}, {4,0}, {0, 4}, {0,0} } };

    struct ListNode n1 = {q1, quad_area(q1), NULL};
    struct ListNode n2 = {q2, quad_area(q2), NULL};
    struct ListNode n3 = {q3, quad_area(q3), NULL};
    struct ListNode n4 = {q4, quad_area(q4), NULL};

    struct ListNode* head = NULL;
    insertListNode(&head, &n1);
    insertListNode(&head, &n2);
    insertListNode(&head, &n3);
    insertListNode(&head, &n4);
    deleteNodeByContent(&head, q11);
    deleteNodeByPosition(&head, 1);

    printList(head);

    printNodeByPosition(head, 2);

    return 0;
}
