#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum type {
  Other,
  Number,
  Add,
  Subtract,
  Multiply,
  Divide,
  OpenParen,
  CloseParen
} type_t;

char* type_name(type_t type) {
  switch (type) {
    case Number:
      return "Number";
    case Add:
      return "Add";
    case Subtract:
      return "Subtract";
    case Multiply:
      return "Multiply";
    case Divide:
      return "Divide";
    case OpenParen:
      return "OpenParen";
    case CloseParen:
      return "CloseParen";
    default:
      return "Invalid";

  }
}

// ---------------------------------------------------------------------------------------------- //
//                                 Definição do nó da lista ligada                                //
// ---------------------------------------------------------------------------------------------- //

typedef struct node {
  type_t type;
  double data;
  struct node *next;
} node_t;

node_t* new_node(type_t type, double data, node_t* next) {
  node_t *node = (node_t*)malloc(sizeof(node_t));

  node -> type = type;
  node -> data = data;
  node -> next = next;

  return node;
}

void print_node(node_t* node) {
  if (node -> type == Number) {
    printf("%5.5f\n", node -> data);
  } else {
    printf("%s\n", type_name(node -> type));
  }
}
// Fim do nó ------------------------------------------------------------------------------------ //

// ---------------------------------------------------------------------------------------------- //
//                                    Definição da lista ligada                                   //
// ---------------------------------------------------------------------------------------------- //
typedef struct list {
  node_t *start, *end;
} list_t;

list_t* new_list() {
  list_t *list = (list_t *)malloc(sizeof(list_t));
  list -> start = NULL;
  list -> end = NULL;
  return list;
}

void drop_list(list_t *list) {
  node_t *current = list -> start;
  while (current) {
    node_t *next = current -> next;
    free(current);
    current = next;
  }
  free(list);
}

void print_list(list_t *list) {
  node_t *current = list -> start;
  while (current) {
    print_node(current);
    current = current -> next;
  }
}

bool is_empty_list(list_t *list) {
  return list -> start == NULL;
}

void push_start(list_t *list, type_t type, double val) {
  node_t *new_start = new_node(type, val, list -> start);
  list -> start = new_start;
  if (list -> end == NULL) {
    list -> end = new_start;
  }
}

void push_end(list_t * list, type_t type, double val) {
  node_t * new_end = new_node(type, val, NULL);

  if (list -> start == NULL){
    list -> start = new_end;
  } else {
    list -> end -> next = new_end;
  }

  list -> end = new_end;
}

double pop_start(list_t * list) {
  if (is_empty_list(list)) {
    return -1;
  }

  double data = list -> start -> data;
  node_t *removed = list -> start;
  list -> start = list -> start -> next;
  free(removed);
  if (list -> start == NULL) {
    list -> end = NULL;
  }
  return data;
}
// Fim da Lista --------------------------------------------------------------------------------- //

// ---------------------------------------------------------------------------------------------- //
//                                       Definição da Pilha                                       //
// ---------------------------------------------------------------------------------------------- //

typedef struct stack {
  list_t *list;
} stack_t;

stack_t* new_stack() {
  stack_t *stack =  (stack_t*)malloc(sizeof(stack_t));
  stack -> list = new_list();
  return stack;
}

void add_stack(stack_t *stack, type_t type, double val) {
  push_start(stack -> list, type, val);
}

double remove_stack(stack_t *stack) {
  return pop_start(stack -> list);
}

int is_stack_empty(stack_t *stack) {
  return is_empty_list(stack -> list);
}

// Fim da pilha --------------------------------------------------------------------------------- //

int main() {
  list_t *list = new_list();

  // 3-1*2+3-1
  push_start(list, Number, 3);
  push_start(list, Subtract, 0);
  push_start(list, Number, 1);
  push_start(list, Multiply, 0);
  push_start(list, Number, 2);
  push_start(list, Add, 0);
  push_start(list, Number, 3);
  push_start(list, Subtract, 0);
  push_start(list, Number, 3);

  print_list(list);
  drop_list(list);
  return 0;
}
