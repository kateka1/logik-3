#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

/* Структура узла на основе методички с добавлением поля priority */
struct node
{
    char inf[256];      // полезная информация
    int priority;       // приоритет (для приоритетной очереди)
    struct node* next;  // ссылка на следующий элемент 
};

/* Рабочие указатели из методички (используются для активного списка) */
struct node* head = NULL, * last = NULL, * f = NULL;
int dlina = 0;

/* Три отдельных независимых списка:
 * 1) для Стека
 * 2) для Очереди
 * 3) для Приоритетной очереди
 */
struct node* stack_head = NULL, * stack_last = NULL;
struct node* queue_head = NULL, * queue_last = NULL;
struct node* pqueue_head = NULL, * pqueue_last = NULL;

/* Вспомогательные функции для переключения между тремя списками */
void load_list(int type)
{
    if (type == 1) { head = stack_head;  last = stack_last; }
    else if (type == 2) { head = queue_head;  last = queue_last; }
    else if (type == 3) { head = pqueue_head; last = pqueue_last; }
}

void save_list(int type)
{
    if (type == 1) { stack_head = head;  stack_last = last; }
    else if (type == 2) { queue_head = head;  queue_last = last; }
    else if (type == 3) { pqueue_head = head; pqueue_last = last; }
}

/* Проверка на наличие дубликата в текущем активном списке (head) */
int is_duplicate(const char* name)
{
    struct node* curr = head;
    while (curr != NULL)
    {
        if (strcmp(name, curr->inf) == 0)
        {
            return 1; // элемент с таким именем уже есть
        }
        curr = curr->next;
    }
    return 0; // дубликатов нет
}

/* --- ФУНКЦИИ ИЗ МЕТОДИЧЕСКОГО ПОСОБИЯ --- */

/* Функция создания элемента списка (с проверкой на уникальность имени) */
struct node* get_struct(void)
{
    struct node* p = NULL;
    char s[256];

    if ((p = (struct node*)malloc(sizeof(struct node))) == NULL)
    {
        printf("Ошибка при распределении памяти\n");
        exit(1);
    }

    printf("Введите название объекта: ");
    scanf("%255s", s);
    if (*s == 0)
    {
        printf("Запись не была произведена\n");
        free(p);
        return NULL;
    }

    // Проверяем, есть ли уже такой элемент в текущем списке
    if (is_duplicate(s))
    {
        printf("Ошибка: такой элемент уже есть в списке!\n");
        free(p);
        return NULL;
    }

    strcpy(p->inf, s);
    p->priority = 0; // значение по умолчанию для обычных структур
    p->next = NULL;
    return p;
}

/* Последовательное добавление в конец списка (Очередь: Enqueue) */
void spstore(void)
{
    struct node* p = NULL;
    p = get_struct();
    if (head == NULL && p != NULL) // если списка нет, устанавливаем голову
    {
        head = p;
        last = p;
        printf("Элемент добавлен в обычную очередь.\n");
    }
    else if (head != NULL && p != NULL) // список уже есть, вставляем в конец
    {
        last->next = p;
        last = p;
        printf("Элемент добавлен в обычную очередь.\n");
    }
    return;
}

/* Просмотр содержимого списка */
void review(void)
{
    struct node* struc = head;
    if (head == NULL)
    {
        printf("Список пуст\n");
        return;
    }
    while (struc)
    {
        printf("Имя: %-15s | Приоритет: %d\n", struc->inf, struc->priority);
        struc = struc->next;
    }
    return;
}

/* Поиск элемента по содержимому */
struct node* find(char* name)
{
    struct node* struc = head;
    if (head == NULL)
    {
        printf("Список пуст\n");
        return NULL;
    }
    while (struc)
    {
        if (strcmp(name, struc->inf) == 0)
        {
            return struc;
        }
        struc = struc->next;
    }
    printf("Элемент не найден\n");
    return NULL;
}

/* Удаление элемента по содержимому */
void del(char* name)
{
    struct node* struc = head;
    struct node* prev = NULL;
    int flag = 0;

    if (head == NULL)
    {
        printf("Список пуст\n");
        return;
    }

    if (strcmp(name, struc->inf) == 0) // удаляемый элемент — первый
    {
        flag = 1;
        head = struc->next;
        if (head == NULL) last = NULL;
        free(struc);
        struc = head;
        printf("Элемент \"%s\" удален.\n", name);
        return; // так как имена уникальны, можно сразу завершить функцию
    }
    else
    {
        prev = struc;
        struc = struc->next;
    }

    while (struc)
    {
        if (strcmp(name, struc->inf) == 0)
        {
            flag = 1;
            if (struc->next)
            {
                prev->next = struc->next;
                free(struc);
            }
            else
            {
                prev->next = NULL;
                last = prev;
                free(struc);
            }
            printf("Элемент \"%s\" удален.\n", name);
            return;
        }
        else
        {
            prev = struc;
            struc = struc->next;
        }
    }

    if (flag == 0)
    {
        printf("Элемент не найден\n");
    }
}

/* --- РЕАЛИЗАЦИЯ 1: СТЕК (LIFO) --- */

/* Добавление в вершину стека (Push) */
void stack_push(void)
{
    load_list(1); // загружаем список стека в head, чтобы get_struct проверил дубликаты
    struct node* p = get_struct();
    if (p == NULL) return;

    p->next = stack_head;
    stack_head = p;
    if (stack_last == NULL) stack_last = p;
    printf("Элемент добавлен в вершину стека.\n");
}

/* Извлечение из вершины стека (Pop) */
void stack_pop(void)
{
    if (stack_head == NULL)
    {
        printf("Стек пуст\n");
        return;
    }
    struct node* temp = stack_head;
    printf("Извлечен элемент со стека: %s\n", temp->inf);
    stack_head = stack_head->next;
    if (stack_head == NULL) stack_last = NULL;
    free(temp);
}

/* --- РЕАЛИЗАЦИЯ 2: ОБЫЧНАЯ ОЧЕРЕДЬ (FIFO) --- */

/* Добавление в конец обычной очереди через spstore() из методички */
void queue_enqueue(void)
{
    load_list(2); // загружаем список очереди в head
    spstore();
    save_list(2);
}

/* Извлечение из начала обычной очереди (Dequeue) */
void queue_dequeue(void)
{
    if (queue_head == NULL)
    {
        printf("Очередь пуста\n");
        return;
    }
    struct node* temp = queue_head;
    printf("Извлечен элемент из очереди: %s\n", temp->inf);
    queue_head = queue_head->next;
    if (queue_head == NULL) queue_last = NULL;
    free(temp);
}

/* --- РЕАЛИЗАЦИЯ 3: ПРИОРИТЕТНАЯ ОЧЕРЕДЬ --- */

/* Вставка в приоритетную очередь с учетом приоритета (по убыванию) */
void pqueue_insert(void)
{
    load_list(3); // загружаем список приоритетной очереди в head для проверки дубликатов

    struct node* p = (struct node*)malloc(sizeof(struct node));
    if (p == NULL)
    {
        printf("Ошибка при распределении памяти\n");
        exit(1);
    }

    printf("Введите название объекта: ");
    scanf("%255s", p->inf);

    // Проверяем на дубликат до ввода приоритета
    if (is_duplicate(p->inf))
    {
        printf("Ошибка: такой элемент уже есть в списке!\n");
        free(p);
        return;
    }

    printf("Введите приоритет (чем число больше, тем приоритет выше): ");
    scanf("%d", &p->priority);
    p->next = NULL;

    // Вставка в пустой список или перед головой, если приоритет больше
    if (pqueue_head == NULL || p->priority > pqueue_head->priority)
    {
        p->next = pqueue_head;
        pqueue_head = p;
        if (pqueue_last == NULL) pqueue_last = p;
        printf("Элемент добавлен в приоритетную очередь.\n");
        return;
    }

    // Поиск места вставки
    struct node* curr = pqueue_head;
    while (curr->next != NULL && curr->next->priority >= p->priority)
    {
        curr = curr->next;
    }

    p->next = curr->next;
    curr->next = p;
    if (p->next == NULL) pqueue_last = p;

    printf("Элемент добавлен в приоритетную очередь.\n");
}

/* Извлечение элемента с наивысшим приоритетом из приоритетной очереди */
void pqueue_dequeue(void)
{
    if (pqueue_head == NULL)
    {
        printf("Приоритетная очередь пуста\n");
        return;
    }
    struct node* temp = pqueue_head;
    printf("Извлечен элемент: %s (Приоритет: %d)\n", temp->inf, temp->priority);
    pqueue_head = pqueue_head->next;
    if (pqueue_head == NULL) pqueue_last = NULL;
    free(temp);
}

/* Подсчет элементов с заданным приоритетом */
void pqueue_count_priority(void)
{
    struct node* curr = pqueue_head;
    int priority, count = 0;

    if (pqueue_head == NULL)
    {
        printf("Приоритетная очередь пуста\n");
        return;
    }

    printf("Введите приоритет для подсчета: ");
    if (scanf("%d", &priority) != 1)
    {
        printf("Ошибка ввода приоритета.\n");
        return;
    }

    while (curr != NULL)
    {
        if (curr->priority == priority)
        {
            count++;
        }
        curr = curr->next;
    }

    printf("Количество элементов с приоритетом %d: %d\n",
        priority, count);
}

/* Очистка одного списка */
void free_list(struct node** h, struct node** l)
{
    struct node* curr = *h;
    while (curr != NULL)
    {
        struct node* tmp = curr;
        curr = curr->next;
        free(tmp);
    }
    *h = NULL;
    *l = NULL;
}

/* Очистка всей памяти всех трех списков при завершении работы */
void free_all(void)
{
    free_list(&stack_head, &stack_last);
    free_list(&queue_head, &queue_last);
    free_list(&pqueue_head, &pqueue_last);
}

/* Выбор списка для общих операций (просмотр, поиск, удаление) */
int choose_list(void)
{
    int type;
    printf("Выберите структуру (1 - Стек, 2 - Очередь, 3 - Приоритетная очередь): ");
    if (scanf("%d", &type) != 1 || type < 1 || type > 3)
    {
        printf("Неверный выбор структуры.\n");
        return 0;
    }
    return type;
}

/* задание функция перемещения введённого элемента в начало стека*/
void stack_move_to_top(void)
{
    char name[256];
    struct node* prev;

    load_list(1);

    if (head == NULL){printf("Стек пуст\n");return;}

    printf("Введите имя элемента для переноса в начало стека: ");
    scanf("%255s", name);

    f = find(name);
    if (f == NULL) return;
    if (f == head)
    {
        printf("Элемент \"%s\" уже находится в начале стека.\n", name);
        return;
    }

    prev = head;
    while (prev->next != f)
        prev = prev->next;

    prev->next = f->next;
    if (f == last) last = prev;

    f->next = head;
    head = f;

    save_list(1);
    printf("Элемент \"%s\" перенесён в начало стека.\n", name);
}

/* --- ГЛАВНАЯ ФУНКЦИЯ И МЕНЮ --- */

int main(void)
{
    setlocale(LC_ALL, "Russian");
    int choice, list_type;
    char search_name[256];

    while (1)
    {
        printf("\n================ МЕНЮ ЛАБОРАТОРНОЙ №3 ================\n");
        printf("Общие операции (для любого из 3 списков):\n");
        printf("  1. Просмотр списка (review)\n");
        printf("  2. Поиск элемента (find)\n");
        printf("  3. Удаление элемента по имени (del)\n");
        printf("Стек (LIFO) [Список №1]:\n");
        printf("  4. Добавить в стек (stack_push)\n");
        printf("  5. Извлечь из стека (stack_pop)\n");
        printf("Очередь (FIFO) [Список №2]:\n");
        printf("  6. Добавить в очередь (spstore)\n");
        printf("  7. Извлечь из очереди (queue_dequeue)\n");
        printf("Приоритетная очередь [Список №3]:\n");
        printf("  8. Добавить с приоритетом (pqueue_insert)\n");
        printf("  9. Извлечь высший приоритет (pqueue_dequeue)\n");
        printf("  10. Найти элемент и переместить его наверх стека(stack_move_to_top)\n");
        printf("  11. Подсчитать элементы с заданным приоритетом\n");
        printf("------------------------------------------------------\n");
        printf("  0. Выход\n");
        printf("Выберите действие: ");

        if (scanf("%d", &choice) != 1) break;

        switch (choice)
        {
        case 1:
            if ((list_type = choose_list()) != 0)
            {
                load_list(list_type);
                review();
            }
            break;
        case 2:
            if ((list_type = choose_list()) != 0)
            {
                load_list(list_type);
                printf("Введите имя для поиска: ");
                scanf("%255s", search_name);
                if (find(search_name) != NULL)
                    printf("Элемент \"%s\" присутствует в выбранном списке.\n", search_name);
            }
            break;
        case 3:
            if ((list_type = choose_list()) != 0)
            {
                load_list(list_type);
                printf("Введите имя для удаления: ");
                scanf("%255s", search_name);
                del(search_name);
                save_list(list_type);
            }
            break;
        case 4:
            stack_push();
            break;
        case 5:
            stack_pop();
            break;
        case 6:
            queue_enqueue();
            break;
        case 7:
            queue_dequeue();
            break;
        case 8:
            pqueue_insert();
            break;
        case 9:
            pqueue_dequeue();
            break;
        case 10:
            stack_move_to_top();
            load_list(1);
           review();
            break;
        case 11:
            pqueue_count_priority();
            break;
        case 0:
            free_all();
            return 0;
        default:
            printf("Неверный ввод, повторите попытку.\n");
        }
    }

    free_all();
    return 0;
}